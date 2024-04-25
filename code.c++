#include <Wire.h>
#include "RTClib.h"
#include <SoftwareSerial.h>
#include "DFRobotDFPlayerMini.h"

// RTC and MP3 module setup
RTC_DS3231 rtc;
SoftwareSerial mySoftwareSerial(10, 11); // RX, TX for DFPlayer
DFRobotDFPlayerMini myDFPlayer;

const int pressureSensorPin = A0; // Only one pressure sensor pin
bool alreadyChecked = false; // Flag to avoid multiple checks at the same minute

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
}

void loop() {
  DateTime now = rtc.now(); // Get the current date and time
  
  // Check if today is Tuesday (2) or Thursday (4) and the time is 9:35 AM
  if ((now.dayOfTheWeek() == 2 || now.dayOfTheWeek() == 4) && now.hour() == 9 && now.minute() == 40) {
    if (!alreadyChecked) {
      alreadyChecked = true; // Set flag to true after the check
      int sensorValue = analogRead(pressureSensorPin);
      if (sensorValue < threshold) {  // Assuming a threshold to determine if a phone is present
        myDFPlayer.play(1);  // Play the specific MP3 file (001.mp3)
      }
    }
  } else {
    alreadyChecked = false; // Reset flag if it is not the correct time
  }

  delay(10000);  // Delay for reducing processing load
}
