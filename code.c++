#include <Wire.h>
#include "RTClib.h"
#include <SoftwareSerial.h>
#include "DFRobotDFPlayerMini.h"

// RTC and MP3 module setup
RTC_DS3231 rtc;
SoftwareSerial mySoftwareSerial(10, 11); // RX, TX for DFPlayer
DFRobotDFPlayerMini myDFPlayer;

const int pressureSensorPin = A0; // Only one pressure sensor pin
DateTime classStartTime;  // Store start time for the class
bool checkDone = false;  // Flag to ensure the check is done once

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

  // Initialize sensor pin
  pinMode(pressureSensorPin, INPUT);

  // Example start time (You would set this based on the actual class start time)
  classStartTime = rtc.now();  // Set to current time for testing purposes
}

void loop() {
  DateTime now = rtc.now();
  
  if (!checkDone && now.unixtime() > classStartTime.unixtime() + 300) { // 300 seconds after class starts
    checkDone = true; // Set the flag to true to avoid repeated checks
    int sensorValue = analogRead(pressureSensorPin);
    if (sensorValue < threshold) {  // Threshold to be determined based on your sensor calibration
      myDFPlayer.play(1);  // Play the specific MP3 file (001.mp3)
    }
  }

  delay(10000);
}
