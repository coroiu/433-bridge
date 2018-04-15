/*
* Project 433-bridge
* Description:
* Author:
* Date:
*/
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#include "receiver.h"

#define OLED_RESET D4

Adafruit_SSD1306 display(OLED_RESET);
// RCSwitch mySwitch = RCSwitch();
int ledPin = D7;
int inputPin = D3;
int count = 0;

Receiver receiver(inputPin);

volatile int inputPinState = 0;
volatile long timeOfLastChange = 0;
volatile long timeSinceLastChange = 0;
volatile long longestTimeWithoutChange = 0;
volatile long timeOnDisplay = -1;

void setup() {
  Serial.begin(9600);
  Wire.setSpeed(CLOCK_SPEED_400KHZ);
  //Wire.setSpeed(800000L);

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  // init done
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);

  display.println("Hellow!");
  display.display(); // show splashscreen
  delay(2000);
  display.clearDisplay();   // clears the screen and buffer
  delay(2000);

  Spark.publish("433", "Listening");

  pinMode(inputPin, INPUT);
  // mySwitch.enableReceive(inputPin);
  // attachInterrupt(inputPin, handleRadioInput, CHANGE);
  pinMode(ledPin, OUTPUT);

  display.setCursor(0,0);
  display.println("Listening");
  display.display();

  receiver.startReceive();
}

void loop() {
  int availableData(0);
  if (availableData = receiver.availableData()) {
    digitalWrite(ledPin, 1);

    Message message = receiver.getMessage();
    noInterrupts();
    display.clearDisplay();
    display.setCursor(0,0);

    display.print("Transmitter id: ");
    display.println(message.transmitterId);

    display.print("Unit code: ");
    display.println(message.unitCode);

    display.print("Group command: ");
    display.println(message.isGroupCommand);

    display.print("Command: ");
    display.println(message.command);

    display.display();
    interrupts();

    // const bool* bits = receiver.getBuffer();
    // for (size_t i = 0; i < RECEIVE_BUFFER_LENGTH; ++i) {
    //   Serial.print(bits[i]);
    // }
    // Serial.println();
    //
    const bool* bits = receiver.getData();
    for (size_t i = 0; i < availableData; ++i) {
      Serial.print(bits[i]);
    }
    Serial.println();
  } else {
    digitalWrite(ledPin, 0);
  }
  // digitalWrite(ledPin, receiver.getLastReceived().value);

  // if (longestTimeWithoutChange < timeSinceLastChange) {
  //   longestTimeWithoutChange = timeSinceLastChange;
  // }
  //
  // if (timeOnDisplay != longestTimeWithoutChange) {
  //   noInterrupts();
  //   display.clearDisplay();
  //   display.setCursor(0,0);
  //   display.println(longestTimeWithoutChange);
  //   display.display();
  //   timeOnDisplay = longestTimeWithoutChange;
  //   interrupts();
  // }

  // display.clearDisplay();
  // display.setCursor(0,0);
  // noInterrupts();
  // display.println(inputPinState);
  // display.display();
  // interrupts();
}

void handleRadioInput() {
  // digitalWrite(ledPin, 1);
  inputPinState = digitalRead(inputPin);
  long time = millis();
  timeSinceLastChange = millis() - timeOfLastChange;
  timeOfLastChange = time;
}
