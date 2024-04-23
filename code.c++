#include <Wire.h>
#include "RTClib.h"

// RTC module setup
RTC_DS3231 rtc;

const int pressureSensorPins[] = {A0, A1, A2}; // Pressure sensor pins
DateTime classStartTimes[3];  // Store start times for each class
bool checksDone[3] = {false, false, false};  // Flags to ensure each check is done once
const int speakerPin = 9; // Speaker pin

void setup() {
  Serial.begin(9600);
  rtc.begin();
  
  pinMode(speakerPin, OUTPUT); // Initialize speaker pin as output

  // Initialize sensor pins
  for (int i = 0; i < 3; i++) {
    pinMode(pressureSensorPins[i], INPUT);
  }

  // Example start times (You would set these based on actual class start times)
  classStartTimes[0] = rtc.now();  // Current time, for testing purposes
  classStartTimes[1] = rtc.now();  // Plus some offset if necessary
  classStartTimes[2] = rtc.now();  // Plus some offset if necessary
}

void loop() {
  DateTime now = rtc.now();
  
  for (int i = 0; i < 3; i++) {
    if (!checksDone[i] && now.unixtime() > classStartTimes[i].unixtime() + 300) { // 300 seconds after class starts
      checksDone[i] = true; // Set the flag to true to avoid repeated checks
      int sensorValue = analogRead(pressureSensorPins[i]);
      if (sensorValue < threshold) {  // Threshold to be determined based on your sensor calibration
        beep(i + 1);  // Beep n times where n is the sensor number (1, 2, or 3)
        delay(2000); // wait for 2 seconds between slots
      }
    }
  }

  delay(10000);  // Delay for reducing processing load
}

void beep(int count) {
  for (int i = 0; i < count; i++) {
    tone(speakerPin, 1000, 200); // Generate a 1000Hz tone for 200ms
    delay(600); // Wait for 600ms between beeps
  }
  noTone(speakerPin); // Ensure no tone is being played after the sequence
}
