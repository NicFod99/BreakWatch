#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_I2CDevice.h>

Adafruit_MPU6050 mpu;

const int soundPin = 34; // Analog pin connected to MAX9814 OUT

// Object
struct Object {
  String name;          // Name of the object tested
  int healthPoints;     // Initial life given, in this case 5 Years (1000HP each), is scaled in 5 min.
  float yearsRemaining; // Date showing proportionally how much time is rimaining.
};

Object TestObject = {"TestObject", 300, 3.0}; // Initialize with 300 HP and 3 years

unsigned long lastPrintTime = 0; // Variable used to check if 3 seconds are passed between prints.

void setup() {
  Serial.begin(115200); // I decided to choose a baudrate of 115200 which works well with analog signals.

  // Initialize MPU6050
  if (!mpu.begin(0x68)) { // 0x68 I2C used, AD0 to GND needed
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G); // The accelerometer can measure accelerations up to ±8 times the acceleration due to gravity (g).
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);      // This means the gyroscope can measure rotational speeds up to ±500 degrees per second. 
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);   // Determines the cutoff frequency for noise reduction. Lower bandwidth settings filter out 
                                                //  more noise but also slow the sensor's response to rapid changes.

  // Initialize analog input for MAX9814
  analogReadResolution(12); // ESP32 has 12-bit ADC those are the number of bits used to represent the analog input signal in digital form. 

  // Print the CSV header
  Serial.println("Vibration Level,Vibration Intensity,Sound Level,Sound Intensity,TestObject HP,Years Remaining,Total damage dealt");
}

void loop() {
  // Read accelerometer data from MPU6050
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Calculate the vibration intensity as the magnitude of the acceleration vector for X and Y axes
  float vibrationIntensity = sqrt(sq(a.acceleration.x) + sq(a.acceleration.y));
  // Map the vibration intensity to a scale from 0 to 10
  int vibrationLevel = map(vibrationIntensity, 0, 6, 0, 10);
  vibrationLevel = constrain(vibrationLevel, 0, 6); // Ensure the value is between 0 and 10

  // Read the sound level from MAX9814
  int soundValue = analogRead(soundPin);
  // Map the sound value to a scale from 0 to 10
  int soundLevel = map(soundValue, 1400, 2000, 0, 10); // 12-bit ADC range is 0 to 4095
  soundLevel = constrain(soundLevel, 0, 10); // Ensure the value is between 0 and 10

  // Reduce health points based on vibration and sound levels
  int total = vibrationLevel + soundLevel;
  TestObject.healthPoints -= total;

  // Ensure health points do not go below 0
  if (TestObject.healthPoints < 0) {
    TestObject.healthPoints = 0;
    Serial.print("Object is dead");
  }

  // Calculate the remaining years proportionally
  TestObject.yearsRemaining = 5.0 * TestObject.healthPoints / 5000.0;

  // Print the results every 3 seconds
  unsigned long currentTime = millis();
  if (currentTime - lastPrintTime >= 3000) {
    Serial.print(vibrationLevel);
    Serial.print(";");
    Serial.print(vibrationIntensity);
    Serial.print(";");
    Serial.print(soundLevel);
    Serial.print(";");
    Serial.print(soundValue);
    Serial.print(";");
    Serial.print(TestObject.healthPoints);
    Serial.print(";");
    Serial.print(TestObject.yearsRemaining, 2);
    Serial.print(";");
    Serial.println(total);
    lastPrintTime = currentTime;
  }

  if (total == 0) {
    TestObject.healthPoints -= 1;
  }

  delay(1000); // Wait for 600 milliseconds before next measurement
}
