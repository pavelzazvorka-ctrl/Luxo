//#include <LobotServoController.h>

/*
  ESP8266 Servo Controller
    
  Download ESPAsyncWebServer library :
  https://github.com/me-no-dev/ESPAsyncWebServer/archive/master.zip

  Download ESPAsyncTCP library :
  https://github.com/me-no-dev/ESPAsyncTCP/archive/master.zip

  Download ESP8266 Filesystem Uploader :
  https://github.com/esp8266/arduino-esp8266fs-plugin/releases/download/0.5.0/ESP8266FS-0.5.0.zip
  
*/

// ModeMcu 1.0 (ESP12E) 4MB ( FS 2MB - 1024OTA )

// Import libraries 
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "trajectory.h"
#include <Servo.h>


// const char *ssid = "TamNET_Guest";  // replace with your SSID
// const char *password = "BeMyGuest"; // replace with your Password

const char *ssid = "TamNET2";  // replace with your SSID
const char *password = "Nintendo"; // replace with your Password

const uint8_t servoPin1 = D6; // Svitidlo nahoru dolu (5)
const uint8_t servoPin2 = D7; // Svitidlo leva prava (90)
const uint8_t servoPin3 = D8; // vrsek (90)
const uint8_t servoPin4 = D1; // spodek (90)
const uint8_t servoPin5 = D5; // otaceni do stran (90)

const uint8_t servo1_default = 5;
const uint8_t servo2_default = 90;
const uint8_t servo3_default = 90; 
const uint8_t servo4_default = 90;
const uint8_t servo5_default = 90;

// Trajectory objects
Trajectory servoTrajectory1(500,  350,  350);
Trajectory servoTrajectory2(500,  350,  350);
Trajectory servoTrajectory3(500,  350,  350);
Trajectory servoTrajectory4(500,  250,  250);
Trajectory servoTrajectory5(500,  250,  250);

/* Create Servo Object */
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;

// The controller will be updated at a rate of 100Hz
#define UPDATE_FREQUENCY 200
#define UPDATE_TIME (1000 / UPDATE_FREQUENCY)
unsigned long updateTimer = 0;

// Create Server instance
AsyncWebServer server(80);

void setup()
{
  Serial.begin(57600);
  // Attach Servo, start SPIFFS and Connect to WiFi

  // assign
  servo1.attach(servoPin1);
  servo2.attach(servoPin2);
  servo3.attach(servoPin3);
  servo4.attach(servoPin4);
  servo5.attach(servoPin5);

  // defaults
  servo1.write(servo1_default);
  servo2.write(servo2_default);
  servo3.write(servo3_default);
  servo4.write(servo4_default);
  servo5.write(servo5_default);

  servoTrajectory1.reset(servo1_default);
  servoTrajectory2.reset(servo2_default);
  servoTrajectory3.reset(servo3_default);
  servoTrajectory4.reset(servo4_default);
  servoTrajectory5.reset(servo5_default);

  if (!SPIFFS.begin())  
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  WiFi.begin(ssid, password);
  Serial.println("Luxo jr. 0.06.00");
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

  // Send home page from SPIFFS
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("Send index.html.");
    request->send(SPIFFS, "/index.html", "text/html");
  });
  
  // Receive Angle from client and process it 
  
  server.on("/servo/1", HTTP_POST, [](AsyncWebServerRequest *request) {
    String angle = request->arg("angle");
    Serial.println("Servo 1: " + angle + "°" );
    servoTrajectory1.setTargetPos(angle.toInt());
    request->send(200);
  });

  server.on("/servo/2", HTTP_POST, [](AsyncWebServerRequest *request) {
    String angle = request->arg("angle");
    Serial.println("Servo 2: " + angle + "°" );
    servoTrajectory2.setTargetPos(angle.toInt());
    request->send(200);
  });

  server.on("/servo/3", HTTP_POST, [](AsyncWebServerRequest *request) {
    String angle = request->arg("angle");
    Serial.println("Servo 3: " + angle + "°" );
    servoTrajectory3.setTargetPos(angle.toInt());
    request->send(200);
  });

  server.on("/servo/4", HTTP_POST, [](AsyncWebServerRequest *request) {
    String angle = request->arg("angle");
    Serial.println("Servo 4: " + angle + "°" );
    servoTrajectory4.setTargetPos(angle.toInt());
    request->send(200);
  });

  server.on("/servo/5", HTTP_POST, [](AsyncWebServerRequest *request) {
    String angle = request->arg("angle");
    Serial.println("Servo 5: " + angle + "°" );
    servoTrajectory5.setTargetPos(angle.toInt());
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

    // Update the controllers
    float currentAngle1 = servoTrajectory1.update();
    servo1.write(round(currentAngle1));
    float currentAngle2 = servoTrajectory2.update();
    servo2.write(round(currentAngle2));
    float currentAngle3 = servoTrajectory3.update();
    servo3.write(round(currentAngle3));
    float currentAngle4 = servoTrajectory4.update();
    servo4.write(round(currentAngle4));
    float currentAngle5 = servoTrajectory5.update();
    servo5.write(round(currentAngle5));

    /*
    Serial.print("Servos: ");
    Serial.print( round(currentAngle1 ));
    Serial.print("° ");
    Serial.print( round(currentAngle2 ));
    Serial.print("° ");
    Serial.print( round(currentAngle3 ));
    Serial.print("° ");
    Serial.print( round(currentAngle4 ));
    Serial.print("° ");
    Serial.print( round(currentAngle5 ));
    Serial.println("° ");
    */
    
    /**
     * For more precise servo control, you could use writeMicroseconds.
     * The min and max PWM pulse widths which correspond to the 0° and 180°
     * positions needs to be inserted for MIN_PWM and MAX_PWM.
     */
    //myservo.writeMicroseconds(map(currentAngle, 0, 180, MIN_PWM, MAX_PWM));
  }
}
