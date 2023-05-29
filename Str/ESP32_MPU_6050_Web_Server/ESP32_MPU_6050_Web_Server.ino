#include <WiFi.h>
#include <WebServer.h>
#include "webpageCode.h"
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

WebServer server(80);
const char* ssid = "Lumos";
const char* password = "Kh@z@rF4";

Adafruit_MPU6050 mpu;

void webpage()
{
  sensors_event_t event;
  mpu.getEvent(&event);
  
  String htmlCode = webpageCode;
  htmlCode.replace("{{gyroX}}", String(event.gyro.x));
  htmlCode.replace("{{gyroY}}", String(event.gyro.y));
  htmlCode.replace("{{gyroZ}}", String(event.gyro.z));
  htmlCode.replace("{{accelX}}", String(event.acceleration.x));
  htmlCode.replace("{{accelY}}", String(event.acceleration.y));
  htmlCode.replace("{{accelZ}}", String(event.acceleration.z));
  
  server.send(200, "text/html", htmlCode);
}

void setup()
{
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  Wire.begin();
  mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G);
  
  server.on("/", webpage);
  server.begin();
}

void loop()
{
  server.handleClient();
}
