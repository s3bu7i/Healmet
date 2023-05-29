// kodu yoxlamaq lazimdir canli sekilde
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_MPU6050.h>
#include <Ultrasonic.h>
#include <TinyGPS++.h>
#include <FastLED.h>

// Define pin numbers
#define SDA_PIN 12
#define SCL_PIN 13
#define TRIGGER_PIN 15
#define ECHO_PIN 14
#define LED_PIN 4
#define GPS_TX_PIN 3

// Initialize objects
Adafruit_MPU6050 mpu;
Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN);
TinyGPSPlus gps;
CRGB leds[1];

void setup() {
  Wire.begin(SDA_PIN, SCL_PIN);

  mpu.begin();
  
  Serial.begin(9600);
  Serial1.begin(9600, SERIAL_8N1, GPS_TX_PIN);

  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, 1);
}

void loop() {
  sensors_event_t accel, gyro, temp;
  mpu.getEvent(&accel, &gyro, &temp);

  float distance = ultrasonic.read() / 10.0; // Convert to cm

  float lat, lon;
  while (Serial1.available()) {
    if (gps.encode(Serial1.read())) {
      lat = gps.location.lat();
      lon = gps.location.lng();
    }
  }

  // Perform your logic to determine direction based on sensor data

  // Example logic: If X acceleration is positive, turn right
  bool turnRight = accel.acceleration.x > 0;

  // Example logic: If Y acceleration is negative, turn left
  bool turnLeft = accel.acceleration.y < 0;

  // Example logic: If Z acceleration is negative, perform some other action
  bool performAction = accel.acceleration.z < 0;

  // Get rotation values
  float rotationX = gyro.gyro.x;
  float rotationY = gyro.gyro.y;
  float rotationZ = gyro.gyro.z;

  // Update LED based on direction
  if (turnRight) {
    leds[0] = CRGB::Green;
  } else if (turnLeft) {
    leds[0] = CRGB::Red;
  } else if (performAction) {
    leds[0] = CRGB::Blue;
  } else {
    leds[0] = CRGB::Black;
  }
  FastLED.show();

  // Print sensor data, distance, and rotation values
  Serial.print("Accel: X=");
  Serial.print(accel.acceleration.x);
  Serial.print(" Y=");
  Serial.print(accel.acceleration.y);
  Serial.print(" Z=");
  Serial.println(accel.acceleration.z);

  Serial.print("Gyro: X=");
  Serial.print(rotationX);
  Serial.print(" Y=");
  Serial.print(rotationY);
  Serial.print(" Z=");
  Serial.println(rotationZ);

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Print GPS coordinates
  Serial.print("Latitude: ");
  Serial.println(lat, 6);
  Serial.print("Longitude: ");
  Serial.println(lon, 6);

  delay(100);
}


