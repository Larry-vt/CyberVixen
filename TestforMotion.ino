#include <Wire.h>
#include <MPU6050.h>

MPU6050 sensor;
int16_t ax, ay, az; // accelerometer readings
int16_t ax_offset, ay_offset, az_offset; // accelerometer offset values
int LED = 9;
int fadeAmount = 20;
int brightness = 0;

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
}

void loop() {
  // Read raw data from the sensor
  sensor.getAcceleration(&ax, &ay, &az);
  
  // Subtract offset values from accelerometer readings
  ax -= ax_offset;
  ay -= ay_offset;
  az -= az_offset;
  
  // Detect jaw movement
  if (ay > 1500) {
    Serial.println("Jaw open");
    Serial.println(ay);
    // gradually increase LED brightness
    brightness += fadeAmount;
    if (brightness > 255) {
      brightness = 255;
    }
  } 
  else {
    Serial.println("Jaw closed");
    // gradually decrease LED brightness
    brightness -= fadeAmount;
    if (brightness < 0) {
      brightness = 0;
    }
  }
  // set LED brightness
  analogWrite(LED, brightness);
  
  // Wait a short period before reading again
  delay(15);
}

