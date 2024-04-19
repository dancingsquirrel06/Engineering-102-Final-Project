#include <Wire.h>
#include "RTClib.h"
#include <SoftwareSerial.h>
#include "DFRobotDFPlayerMini.h"

// RTC and MP3 module setup
RTC_DS3231 rtc;
SoftwareSerial mySoftwareSerial(10, 11); // RX, TX for DFPlayer
DFRobotDFPlayerMini myDFPlayer;

const int pressureSensorPins[] = {A0, A1, A2}; // Pressure sensor pins
DateTime classStartTimes[3];  // Store start times for each class
bool checksDone[3] = {false, false, false};  // Flags to ensure each check is done once

void setup() {
  Serial.begin(9600);
  mySoftwareSerial.begin(9600);
  rtc.begin();

  // Initialize the MP3 player
  if (!myDFPlayer.begin(mySoftwareSerial)) {
    Serial.println("MP3 player initialization failed!");
    while (true);
  }
  myDFPlayer.volume(10);

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
        myDFPlayer.play(i + 1);  // Play specific MP3 file (001.mp3, 002.mp3, 003.mp3)
      }
    }
  }

  delay(10000);  // Delay for reducing processing load
}
