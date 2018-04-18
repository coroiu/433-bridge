/*
* Project 433-bridge
* Description:
* Author:
* Date:
*/
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#include "receiver.h"
#include "transmitter.h"

#define OLED_RESET D4
#define LED_PIN D7
#define INPUT_PIN D3
#define TRANSMIT_PIN D4
#define CLOUD_FUNCTION_DELIMITER ","

Adafruit_SSD1306 display(OLED_RESET);
// RCSwitch mySwitch = RCSwitch();
int count = 0;

Receiver receiver(INPUT_PIN);
Transmitter transmitter(TRANSMIT_PIN);

volatile int inputPinState = 0;
volatile long timeOfLastChange = 0;
volatile long timeSinceLastChange = 0;
volatile long longestTimeWithoutChange = 0;
volatile long timeOnDisplay = -1;

void setup() {
  Serial.begin(9600);
  Wire.setSpeed(CLOCK_SPEED_400KHZ);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x64)
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);

  display.println("Hellow!");
  display.display(); // show splashscreen
  delay(2000);
  display.clearDisplay();   // clears the screen and buffer
  delay(2000);

  Particle.publish("433", "Listening");
  Particle.function("transmit", transmit);

  pinMode(INPUT_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(TRANSMIT_PIN, OUTPUT);
  digitalWrite(TRANSMIT_PIN, 0);

  display.setCursor(0,0);
  display.println("Listening");
  display.display();

  receiver.startReceive();
}

void loop() {
  int availableData(0);
  if (availableData = receiver.availableData()) {
    digitalWrite(LED_PIN, 1);

    Message message = receiver.getMessage();
    noInterrupts();
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("Received: ");
    display.println();
    printMessageToDisplay(message);
    display.display();
    interrupts();

    const bool* bits = receiver.getData();
    for (size_t i = 0; i < availableData; ++i) {
      Serial.print(bits[i]);
    }
    Serial.println();
  } else {
    digitalWrite(LED_PIN, 0);
  }

}

int transmit(String command) {
  int* values = parseCommand(command);
  Message message(
    values[0], // Transmitted id,
    values[1], // Unit code
    values[2], // isGroupCommand
    values[3]  // Command
  );

  receiver.stopReceive();
  transmitter.transmit(message);

  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Transmitted: ");
  display.println();
  printMessageToDisplay(message);
  display.display();

  receiver.startReceive();

  delete values;
}

int* parseCommand(String command) {
  // Begin black magic supplied by @mdma at:
  // https://community.spark.io/t/gpio-control-via-gui/6310/7
  char charBuf[63];
  strncpy(charBuf, command.c_str(), sizeof(charBuf));
  const int value_count = 4;  // the maximum number of values
  int* values = new int[value_count];    // store the values here

  char string[63];
  strcpy(string, charBuf);  // the string to split
  int idx = 0;
  for (char* pt=strtok(string, CLOUD_FUNCTION_DELIMITER); pt && idx < value_count; pt=strtok(NULL, CLOUD_FUNCTION_DELIMITER)) {
     values[idx++] = atoi(pt);
  }
  return values;
}

void printMessageToDisplay(Message message) {
  display.print("Transmitter id: ");
  display.println(message.transmitterId);

  display.print("Unit code: ");
  display.println(message.unitCode);

  display.print("Group command: ");
  display.println(message.isGroupCommand);

  display.print("Command: ");
  display.println(message.command);
}
