
/*
  ESP8266 Servo Controller
    
  Download ESPAsyncWebServer library :
  https://github.com/me-no-dev/ESPAsyncWebServer/archive/master.zip

  Download ESPAsyncTCP library :
  https://github.com/me-no-dev/ESPAsyncTCP/archive/master.zip

  Download ESP8266 Filesystem Uploader :
  https://github.com/esp8266/arduino-esp8266fs-plugin/releases/download/0.5.0/ESP8266FS-0.5.0.zip

  29.12.2022 0.09.00 - migration to Fury ( no code changes)
*/

#define SERVO-ON // outcomment to debug W/O servo library ( for web-server ) 
//#define DEBUG

// ModeMcu 1.0 (ESP12E) 4MB ( FS 2MB - 1024OTA )
// CH34x driver / set port speed to 115200
// ---------------------------------------------------------------
// Chip is ESP8266EX
// Features: WiFi
// Crystal is 26MHz
// MAC: 44:17:93:1f:bb:54
// Uploading stub...
// Running stub...
// Stub running...
// Configuring flash size...
// Auto-detected Flash size: 4MB

// Import libraries 
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "trajectory.h"
#include "lobot.h"

// const char *ssid = "TamNET_Guest";  // replace with your SSID
// const char *password = "BeMyGuest"; // replace with your Password

const char *ssid = "TamNET2";  // replace with your SSID
const char *password = "Nintendo"; // replace with your Password

//const char *ssid = "Free Mercure Stoller Zuerich";  // replace with your SSID
//const char *password = ""; // replace with your Password

const uint8_t servo1_default = 90;
const uint8_t servo2_default = 90;
const uint8_t servo3_default = 90; 
const uint8_t servo4_default = 90;
const uint8_t servo5_default = 90;

// Trajectory objects / Lamp setup
//                          maxV  acD   dcD
//------------------------------------------      
Trajectory servoTrajectory5(800,  150,  150);
Trajectory servoTrajectory4(400,  150,  150 );
Trajectory servoTrajectory3(200,  40,   40 );
Trajectory servoTrajectory2(200,  40,   40);
Trajectory servoTrajectory1(400,  150,  150);

// The controller will be updated at a rate of 10Hz
#define UPDATE_FREQUENCY 10
#define UPDATE_TIME (1000 / UPDATE_FREQUENCY)
unsigned long updateTimer = 0;

#define ID1 1
#define ID2 2
#define ID3 3
#define ID4 4
#define ID5 5
#define ANGLE_LOW 0
#define ANGLE_MAX 180
#define SERVO_LOW 0
#define SERVO_MAX 1000
#define INIT_TIME 2000

// Create Server instance
AsyncWebServer server(80);
const uint8_t lightPin  = D2; 

void setup()
{
  pinMode(lightPin, OUTPUT);
  Serial.begin(115200);
  Serial.print("Luxo jr. 0.90.02 LX");

#ifdef SERVO-ON  
  Serial.println("-ON");
  // move to defaults
  LobotSerialServoMove(Serial, ID1, map(servo1_default, ANGLE_LOW, ANGLE_MAX, SERVO_LOW, SERVO_MAX), INIT_TIME);
  LobotSerialServoMove(Serial, ID2, map(servo2_default, ANGLE_LOW, ANGLE_MAX, SERVO_LOW, SERVO_MAX), INIT_TIME);
  LobotSerialServoMove(Serial, ID3, map(servo3_default, ANGLE_LOW, ANGLE_MAX, SERVO_LOW, SERVO_MAX), INIT_TIME);
  LobotSerialServoMove(Serial, ID4, map(servo4_default, ANGLE_LOW, ANGLE_MAX, SERVO_LOW, SERVO_MAX), INIT_TIME);
  LobotSerialServoMove(Serial, ID5, map(servo5_default, ANGLE_LOW, ANGLE_MAX, SERVO_LOW, SERVO_MAX), INIT_TIME);
  delay(2000);
  
  servoTrajectory1.reset(servo1_default);
  servoTrajectory2.reset(servo2_default);
  servoTrajectory3.reset(servo3_default);
  servoTrajectory4.reset(servo4_default);
  servoTrajectory5.reset(servo5_default);
#else  
  Serial.println("-OFF");
#endif  
      
  // start SPIFFS and Connect to WiFi
  if (!SPIFFS.begin())  
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi..");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.print("\nConnected to the WiFi network: ");
  Serial.print(WiFi.SSID());
  Serial.print(" IP address:");
  Serial.println(WiFi.localIP());

  // No 1K pull down, lets controll that by input/output switch 
  Serial.println("light: ON");
  pinMode(lightPin, INPUT);

  // Send home page from SPIFFS
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("Send index.html.");
    request->send(SPIFFS, "/index.html", "text/html");
  });
  
  // Receive Angle from client and process it 
  
  server.on("/servo/1", HTTP_POST, [](AsyncWebServerRequest *request) {
    String angle = request->arg("angle");
#ifdef DEBUG    
    Serial.println("Servo 1: " + angle + "°" );
#endif    
    servoTrajectory1.setTargetPos(angle.toInt());
    request->send(200);
  });

  server.on("/servo/2", HTTP_POST, [](AsyncWebServerRequest *request) {    
    String angle = request->arg("angle");
#ifdef DEBUG        
    Serial.println("Servo 2: " + angle + "°" );
#endif        
    servoTrajectory2.setTargetPos(angle.toInt());
    request->send(200);
  });

  server.on("/servo/3", HTTP_POST, [](AsyncWebServerRequest *request) {
    String angle = request->arg("angle");
#ifdef DEBUG        
    Serial.println("Servo 3: " + angle + "°" );
#endif        
    servoTrajectory3.setTargetPos(angle.toInt());
    request->send(200);
  });

  server.on("/servo/4", HTTP_POST, [](AsyncWebServerRequest *request) {
    String angle = request->arg("angle");
#ifdef DEBUG        
    Serial.println("Servo 4: " + angle + "°" );
#endif        
    servoTrajectory4.setTargetPos(angle.toInt());
    request->send(200);
  });

  server.on("/servo/5", HTTP_POST, [](AsyncWebServerRequest *request) {
    String angle = request->arg("angle");
#ifdef DEBUG        
    Serial.println("Servo 5: " + angle + "°" );
#endif        
    servoTrajectory5.setTargetPos(angle.toInt());
    request->send(200);
  });

  server.on("/light", HTTP_POST, [](AsyncWebServerRequest *request) {
    String light = request->arg("light");
#ifdef DEBUG        
    Serial.println("light: " + light );
#endif        
    if (light.toInt()>0)
    {
      pinMode(lightPin, INPUT);
      //digitalWrite(lightPin,HIGH);
    }
    else
    {
      pinMode(lightPin, OUTPUT);      
      //digitalWrite(lightPin,LOW);
    }
    request->send(200);
  });

  server.on("/status", HTTP_POST, [](AsyncWebServerRequest *request) {
    if ( servoTrajectory1.ready() & servoTrajectory2.ready() & servoTrajectory3.ready() & servoTrajectory4.ready() & servoTrajectory5.ready() )
    {
      request->send(200);
    }
    else
    {
      request->send(202);
    }
  });

  // Send Favicon / image
  server.serveStatic("/favicon.ico", SPIFFS, "/favicon.ico");
  server.serveStatic("/luxo.png", SPIFFS, "/luxo.png");

  // Begin Server
  server.begin();
}

void loop() {

  // Update the servo position at regular intervals
  if (millis() - updateTimer >= UPDATE_TIME) {
    updateTimer += UPDATE_TIME;

#ifdef SERVO-ON

    // Update the controllers
    float currentAngle1 = servoTrajectory1.update();
    LobotSerialServoMove(Serial, ID1, map(round(currentAngle1), ANGLE_LOW, ANGLE_MAX, SERVO_LOW, SERVO_MAX), UPDATE_TIME);
    float currentAngle2 = servoTrajectory2.update();
    LobotSerialServoMove(Serial, ID2, map(round(currentAngle2), ANGLE_LOW, ANGLE_MAX, SERVO_LOW, SERVO_MAX), UPDATE_TIME);
    float currentAngle3 = servoTrajectory3.update();
    LobotSerialServoMove(Serial, ID3, map(round(currentAngle3), ANGLE_LOW, ANGLE_MAX, SERVO_LOW, SERVO_MAX), UPDATE_TIME);
    float currentAngle4 = servoTrajectory4.update();
    LobotSerialServoMove(Serial, ID4, map(round(currentAngle4), ANGLE_LOW, ANGLE_MAX, SERVO_LOW, SERVO_MAX), UPDATE_TIME);
    float currentAngle5 = servoTrajectory5.update();
    LobotSerialServoMove(Serial, ID5, map(round(currentAngle5), ANGLE_LOW, ANGLE_MAX, SERVO_LOW, SERVO_MAX), UPDATE_TIME);
    
#endif    
  }
}
