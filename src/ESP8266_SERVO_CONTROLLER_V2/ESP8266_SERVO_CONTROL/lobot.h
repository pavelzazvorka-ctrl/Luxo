#ifndef lobot_h
#define lobot_h

void LobotSerialServoMove(HardwareSerial &SerialX, uint8_t id, int16_t position, uint16_t time);
void LobotSerialServoStopMove(HardwareSerial &SerialX, uint8_t id);
void LobotSerialServoSetID(HardwareSerial &SerialX, uint8_t oldID, uint8_t newID);
void LobotSerialServoSetMode(HardwareSerial &SerialX, uint8_t id, uint8_t Mode, int16_t Speed);
void LobotSerialServoLoad(HardwareSerial &SerialX, uint8_t id);
void LobotSerialServoUnload(HardwareSerial &SerialX, uint8_t id);
int LobotSerialServoReceiveHandle(HardwareSerial &SerialX, byte *ret);
int LobotSerialServoReadPosition(HardwareSerial &SerialX, uint8_t id);
int LobotSerialServoReadVin(HardwareSerial &SerialX, uint8_t id);

/*
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000);
}

#define ID1   3
#define ID2   2

void loop() {
  // put your main code here, to run repeatedly:
  LobotSerialServoMove(Serial, ID1, 000, 0);
  delay(1000);
  LobotSerialServoMove(Serial, ID1, 1000, 0);
  delay(1000);
  LobotSerialServoMove(Serial, ID1, 0, 2000);
  delay(3000);
  LobotSerialServoMove(Serial, ID1, 1000, 2000);
  delay(3000);
  LobotSerialServoMove(Serial, ID1, 500, 2000);
  delay(5000);

}
*/
#endif