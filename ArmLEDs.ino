#include <Wire.h>
#include <MPU6050.h>
#include <Adafruit_NeoPixel.h>

MPU6050 sensor;
int16_t ax, ay, az; // accelerometer readings
int16_t ax_offset, ay_offset, az_offset; // accelerometer offset values
int LED = 9;
int bright = 0;
int fade = 5;

#define PIN 6 // Pin for Neopixel data
#define NUM_LEDS 6 // Number of Neopixels
Adafruit_NeoPixel strip(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);
  Wire.begin();
  sensor.initialize();
  while (!Serial);
  Serial.println("Calibrating accelerometer...");
  delay(1000); // wait for sensor to stabilize
  sensor.CalibrateAccel();
  sensor.getAcceleration(&ax_offset, &ay_offset, &az_offset); // get accelerometer offset values
  Serial.println("Done.");
  pinMode(LED, OUTPUT);
  strip.begin();
}

void loop() {
  // Read raw data from the sensor
  sensor.getAcceleration(&ax, &ay, &az);
  
  // Subtract offset values from accelerometer readings
  ax -= ax_offset;
  ay -= ay_offset;
  az -= az_offset;
  
  // Detect jaw movement
  if (ay > 2000) {
    Serial.println("Jaw open");
    Serial.println(ay);
    colorWipe(strip.Color(255, 255, 255), 50); // Neopixels turn white
    delay(1000);
    colorChase(strip.Color(255, 0, 0), 50); // Neopixels chase red
  } 
  else {
    Serial.println("Jaw closed");
    analogWrite(LED, 0);
    Serial.println(ay);
  }

  // Wait a short period before reading again
  delay(15);
}

// Neopixel functions
void colorWipe(uint32_t color, int wait) {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
    strip.show();
    delay(wait);
  }
}

void colorChase(uint32_t color, int wait) {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
    strip.show();
    delay(wait);
    strip.setPixelColor(i, 0);
  }
  strip.show();
}