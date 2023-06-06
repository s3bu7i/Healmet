#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_MPU6050.h>
#include <Ultrasonic.h>
#include <TinyGPS++.h>
#include <FastLED.h>

// Define pin numbers
#define SDA_PIN 21
#define SCL_PIN 22
#define TRIGGER_PIN 15
#define ECHO_PIN 14
#define LED_PIN 4
#define BUZZER_PIN 5       // Add this line

// Initialize objects
Adafruit_MPU6050 mpu;
Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN);
TinyGPSPlus gps;
CRGB leds[64];

// Constants for LED animation
const uint8_t numLeds = 64;
const uint8_t animationDelay = 50;

// Variables for LED animation
uint8_t animationStep = 0;

// Function to show the perform action animation on the LED
void showPerformAction() {
  animationStep++;
  for (int i = 0; i < numLeds; i++) {
    leds[i] = CRGB::White;
  }
  
  FastLED.show();
  delay(animationDelay);
}

// Function to show the stop animation on the LED
void showStop() {
  animationStep++;
  for (int i = 0; i < numLeds; i++) {
    leds[i] = CRGB::White;
  }
  
  FastLED.show();
  delay(animationDelay);
}

// Function to show the right arrow animation on the LED
void showRightArrow() {
  animationStep++;
  for (int i = 0; i < numLeds; i++) {
    leds[i] = CRGB::Black;
  }
  
  for (int i = 0; i < animationStep % (numLeds / 2); i++) {
    leds[i] = CRGB::White;
  }
  
  FastLED.show();
  delay(animationDelay);
}

// Function to show the left arrow animation on the LED
void showLeftArrow() {
  animationStep++;
  for (int i = 0; i < numLeds; i++) {
    leds[i] = CRGB::Black;
  }
  
  for (int i = numLeds - 1; i > numLeds - 1 - (animationStep % (numLeds / 2)); i--) {
    leds[i] = CRGB::White;
  }
  
  FastLED.show();
  delay(animationDelay);
}

// Function to show the lightning effect animation on the LED
void showLightningEffect() {
  for (int i = 0; i < numLeds; i++) {
    leds[i] = CRGB::White;
  }
  
  FastLED.show();
  delay(animationDelay);
  
  for (int i = 0; i < numLeds; i++) {
    leds[i] = CRGB::Black;
  }
  
  FastLED.show();
  delay(animationDelay);
}

// Function to activate the buzzer for a beep sound
void activateBuzzer() {
  tone(BUZZER_PIN, 1000, 100);  // Adjust the frequency and duration as needed
  delay(100);                   // Delay between beeps
}

void setup() {
  Wire.begin(SDA_PIN, SCL_PIN);
  mpu.begin();
  
  Serial.begin(9600);
  Serial1.begin(9600, SERIAL_8N1, GPS_TX_PIN);

  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, numLeds);

  pinMode(BUZZER_PIN, OUTPUT);  // Set the buzzer pin as an output
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
  
  bool turnRight = accel.acceleration.x > 0;
  bool turnLeft = accel.acceleration.x < 0;
  bool performAction = accel.acceleration.z < 0;

  // Update LED animation based on gyroscope data
  if (turnRight) {
    showRightArrow();
  } else if (turnLeft) {
    showLeftArrow();
  } else if (performAction) {
    showPerformAction();
  } else {
    showStop();
  }

  // Check for emergency stop
  if (distance < 10.0) {
    // Activate emergency stop
    showLightningEffect();
  }

  // Check distance range for buzzer activation
  if (distance >= 20.0 && distance <= 30.0) {
    activateBuzzer();
  }

  // Print sensor data, distance, and rotation values
  Serial.print("Accel: X=");
  Serial.print(accel.acceleration.x);
  Serial.print(" Y=");
  Serial.print(accel.acceleration.y);
  Serial.print(" Z=");
  Serial.println(accel.acceleration.z);

  Serial.print("Gyro: X=");
  Serial.print(gyro.gyro.x);
  Serial.print(" Y=");
  Serial.print(gyro.gyro.y);
  Serial.print(" Z=");
  Serial.println(gyro.gyro.z);

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Print GPS coordinates
  Serial.print("Latitude: ");
  Serial.println(lat, 6);
  Serial.print("Longitude: ");
  Serial.println(lon, 6);

  delay(500);
}
