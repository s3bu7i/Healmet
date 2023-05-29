#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_MPU6050.h>
#include <NewPing.h>
#include <TinyGPS++.h>

#define SDA 12
#define SCL 13
#define TRIG_PIN 15
#define ECHO_PIN 14
#define LED_PIN 4
#define GPS_TX 3

Adafruit_MPU6050 mpu;
NewPing sonar(TRIG_PIN, ECHO_PIN);
TinyGPSPlus gps;

void setup() {
  Serial.begin(9600);
  Wire.begin(SDA, SCL);

  if (!mpu.begin()) {
    Serial.println("Failed to initialize MPU6050!");
    while (1);
  }

  pinMode(LED_PIN, INPUT);
  pinMode(GPS_TX, INPUT);
}

void loop() {
  sensors_event_t accelEvent, gyroEvent, tempEvent;
  mpu.getEvent(&accelEvent, &gyroEvent, &tempEvent);

  float ax = accelEvent.acceleration.x;
  float ay = accelEvent.acceleration.y;
  float az = accelEvent.acceleration.z;

  float gx = gyroEvent.gyro.x;
  float gy = gyroEvent.gyro.y;
  float gz = gyroEvent.gyro.z;

  Serial.print("Acceleration: ");
  Serial.print(ax);
  Serial.print(", ");
  Serial.print(ay);
  Serial.print(", ");
  Serial.println(az);

  Serial.print("Rotation: ");
  Serial.print(gx);
  Serial.print(", ");
  Serial.print(gy);
  Serial.print(", ");
  Serial.println(gz);

  unsigned int distance = sonar.ping_cm();
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  while (Serial.available() > 0) {
    gps.encode(Serial.read());
  }

  if (gps.location.isUpdated()) {
    Serial.print("Latitude: ");
    Serial.println(gps.location.lat(), 6);
    Serial.print("Longitude: ");
    Serial.println(gps.location.lng(), 6);
  }

  if (digitalRead(LED_PIN) == HIGH) {
    Serial.println("LED is on - stop the car");
  } else {
    if (distance < 50) {
      Serial.println("Obstacle detected - stop the car");
    } else {
      Serial.println("Clear path ahead - continue driving");
    }
  }

  delay(500);
}