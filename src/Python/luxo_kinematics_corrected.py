"""Luxo Jr. calibrated *provisional* 5-axis model. Units: mm, degrees.

Servo 2 and 3 set ABSOLUTE arm angles independently (parallelogram linkage).
Servo 4 and 5 set shade direction independently of both arms.
Servo 1 rotates the entire assembly about the vertical Z axis.
No hardware communication.
"""
from dataclasses import dataclass
import numpy as np
from scipy.optimize import least_squares

DEG = np.pi / 180.0
LIMITS = np.array([[0, 180], [95, 140], [60, 133], [45, 113], [0, 180]], dtype=float)
REFERENCE = np.array([90., 114., 97., 57., 97.])

@dataclass(frozen=True)
class Calibration:
    lower_length: float = 230.0
    upper_length: float = 175.0
    shoulder_height: float = 100.0
    horizontal_extension: float = 85.0  # always horizontal, points left at reference
    shade_axis_offset: float = 55.0  # axis of bulb is left of servo 4 at reference
    bulb_from_axis_5: float = 75.0  # approximate along light direction
    lower_vertical_servo: float = 114.0
    lower_gain: float = 1.0  # provisional
    upper_horizontal_servo: float = 97.0
    upper_gain: float = 1.0  # provisional
    shade_pitch_zero: float = 57.0
    shade_pitch_gain: float = -1.5  # verified sign: 57 -> 87 raises light direction
    shade_side_zero: float = 97.0
    shade_side_gain: float = -9.0 / 7.0  # corrected sign, checked on physical lamp
    base_zero: float = 90.0
    base_gain: float = -1.0  # verified: 0 counterclockwise, 180 clockwise, top view

CAL = Calibration()


def _rz(angle):
    c, s = np.cos(angle), np.sin(angle)
    return np.array([[c, -s, 0.], [s, c, 0.], [0., 0., 1.]])


def forward(q, cal=CAL):
    """Return (7x3 points, unit ray): base, shoulder, elbow, upper_end, axis4, shade_axis, bulb.

    Local Y is defined by the model; physical left/right is calibrated.
    The wrist offset direction and bulb offset are still provisional.
    """
    q = np.asarray(q, dtype=float)
    if q.shape != (5,):
        raise ValueError('Expected five servo angles')
    yaw = (q[0] - cal.base_zero) * cal.base_gain * DEG
    lower = (90.0 + (q[1] - cal.lower_vertical_servo) * cal.lower_gain) * DEG
    upper = (180.0 + (q[2] - cal.upper_horizontal_servo) * cal.upper_gain) * DEG
    pitch = (q[3] - cal.shade_pitch_zero) * cal.shade_pitch_gain * DEG
    side = (q[4] - cal.shade_side_zero) * cal.shade_side_gain * DEG
    R = _rz(yaw)
    origin = np.zeros(3)
    shoulder = np.array([0., 0., cal.shoulder_height])
    elbow = shoulder + R @ np.array([cal.lower_length * np.cos(lower), 0., cal.lower_length * np.sin(lower)])
    upper_end = elbow + R @ np.array([cal.upper_length * np.cos(upper), 0., cal.upper_length * np.sin(upper)])
    # The rigid extension remains horizontal regardless of the upper arm angle.
    axis4 = upper_end + R @ np.array([-cal.horizontal_extension, 0., 0.])
    # Direction decoupled from both arm angles.
    local_dir = np.array([np.sin(pitch) * np.cos(side), np.sin(side), -np.cos(pitch) * np.cos(side)])
    ray = R @ local_dir
    ray /= np.linalg.norm(ray)
    # Servo 4 rotates the shade's eccentric mounting point about its pitch axis.
    # Reference offset is 55 mm to the left. Servo 5 changes ray direction
    # without moving the mounting point (approximation to be verified).
    # Pitch sign is calibrated from the shade direction: local pitch positive
    # corresponds to the offset rotating toward positive Z.
    axis5 = axis4 + R @ np.array([-cal.shade_axis_offset * np.cos(pitch),
                                  0., cal.shade_axis_offset * np.sin(pitch)])
    bulb = axis5 + cal.bulb_from_axis_5 * ray
    return np.array([origin, shoulder, elbow, upper_end, axis4, axis5, bulb]), ray


def tabletop_hit(q, cal=CAL):
    points, ray = forward(q, cal)
    bulb = points[-1]
    if ray[2] >= -1e-9 or bulb[2] <= 0:
        return None
    distance = -bulb[2] / ray[2]
    return bulb + distance * ray if distance >= 0 else None


def tilt_from_vertical(ray):
    return float(np.degrees(np.arccos(np.clip(-ray[2], -1, 1))))


def _residual(q, target, tilt, q_start, cal):
    points, ray = forward(q, cal)
    bulb = points[-1]
    # Smooth ray/plane residual even when the ray points upward: prevents
    # optimizer from getting stuck at the discontinuity of tabletop_hit().
    dz = min(ray[2], -0.05)
    t = max(0.0, -bulb[2] / dz)
    projected = bulb + t * ray
    xy_error = (projected[:2] - target[:2]) / 100.0
    vertical_penalty = max(0.0, ray[2] + 0.05) * 15.0
    height_penalty = max(0.0, -bulb[2]) / 100.0
    actual_tilt = tilt_from_vertical(ray)
    return np.r_[xy_error * 8., (actual_tilt - tilt) / 15.0,
                 vertical_penalty, height_penalty, (q - q_start) / 100.0 * 0.12]


def inverse(target_xy, tilt, start, cal=CAL, random_starts=10):
    """Constrained numeric IK. Returns (angles, error_mm, tilt_error_deg, hit_or_None).

    The target is NOT guaranteed reachable. Always inspect returned errors.
    """
    target = np.asarray(target_xy, float)
    start = np.clip(np.asarray(start, float), LIMITS[:, 0], LIMITS[:, 1])
    rng = np.random.default_rng(2026)
    seeds = [start, REFERENCE] + [rng.uniform(LIMITS[:, 0], LIMITS[:, 1]) for _ in range(random_starts)]
    best = None
    for seed in seeds:
        result = least_squares(_residual, seed, bounds=(LIMITS[:, 0], LIMITS[:, 1]),
                               args=(target, float(tilt), start, cal), max_nfev=230)
        hit = tabletop_hit(result.x, cal)
        _, ray = forward(result.x, cal)
        error = float(np.linalg.norm(hit[:2] - target[:2])) if hit is not None else float('inf')
        tilt_error = abs(tilt_from_vertical(ray) - tilt)
        score = error + tilt_error * 5 + np.linalg.norm((result.x - start) / 180) * 0.5
        if best is None or score < best[0]:
            best = (score, result.x, error, tilt_error, hit)
    return best[1], best[2], best[3], best[4]
