/*
  ESP8266 Servo Controller
    
  Download ESPAsyncWebServer library :
  https://github.com/me-no-dev/ESPAsyncWebServer/archive/master.zip

  Download ESPAsyncTCP library :
  https://github.com/me-no-dev/ESPAsyncTCP/archive/master.zip

  Download ESP8266 Filesystem Uploader :
  https://github.com/esp8266/arduino-esp8266fs-plugin/releases/download/0.5.0/ESP8266FS-0.5.0.zip
  
*/

// Import libraries 
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Servo.h>


// const char *ssid = "TamNET_Guest";  // replace with your SSID
// const char *password = "BeMyGuest"; // replace with your Password

const char *ssid = "TamNET2";  // replace with your SSID
const char *password = "Nintendo"; // replace with your Password

const uint8_t servoPin1 = D1;
const uint8_t servoPin2 = D5;
const uint8_t servoPin3 = D6;
const uint8_t servoPin4 = D7;
const uint8_t servoPin5 = D8;

/* Create Servo Object */
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;

// Create Server instance
AsyncWebServer server(80);
void setup()
{
  Serial.begin(115200);
  // Attach Servo, start SPIFFS and Connect to WiFi

  // assign
  servo1.attach(servoPin1);
  servo2.attach(servoPin2);
  servo3.attach(servoPin3);
  servo4.attach(servoPin4);
  servo5.attach(servoPin5);

  // defaults
  servo1.write(120);
  servo2.write(90);
  servo3.write(0);
  servo4.write(90);
  servo5.write(5);

  if (!SPIFFS.begin())  
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  WiFi.begin(ssid, password);
  Serial.println("Luxo jr. 0.04.00");
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
    servo1.write(angle.toInt());
    request->send(200);
  });

  server.on("/servo/2", HTTP_POST, [](AsyncWebServerRequest *request) {
    String angle = request->arg("angle");
    Serial.println("Servo 2: " + angle + "°" );
    servo2.write(angle.toInt());
    request->send(200);
  });

  server.on("/servo/3", HTTP_POST, [](AsyncWebServerRequest *request) {
    String angle = request->arg("angle");
    Serial.println("Servo 3: " + angle + "°" );
    servo3.write(angle.toInt());
    request->send(200);
  });

  server.on("/servo/4", HTTP_POST, [](AsyncWebServerRequest *request) {
    String angle = request->arg("angle");
    Serial.println("Servo 4: " + angle + "°" );
    servo4.write(angle.toInt());
    request->send(200);
  });

  server.on("/servo/5", HTTP_POST, [](AsyncWebServerRequest *request) {
    String angle = request->arg("angle");
    Serial.println("Servo 5: " + angle + "°" );
    servo5.write(angle.toInt());
    request->send(200);
  });

  // Send Favicon / image
  server.serveStatic("/favicon.ico", SPIFFS, "/favicon.ico");
  server.serveStatic("/luxo.png", SPIFFS, "/luxo.png");

  // Begin Server
  server.begin();
}
void loop()
{
}
