#include "transmitter.h"

Transmitter::Transmitter(int transmitPin): transmitPin(transmitPin) { };

bool* Transmitter::intToBitsArray(int number) {
  bool* bits = new bool[32];
  for (size_t i(0); i != 32; ++i) {
    bits[31 - i] = (number >> i) & 1;
  }
  return bits;
}

void Transmitter::transmit(Message message) {
  for (size_t i(0); i != N_REPEAT_TRANSMISSION; ++i) {
    // Send start sequence
    digitalWrite(transmitPin, 1);
    delayMicroseconds(TIME_UNIT);
    digitalWrite(transmitPin, 0);
    delayMicroseconds(START_BIT_DURATION);

    // Send data
    bool *bits = intToBitsArray(message.toInt());
    transmitBits(bits, 32);
    delete bits;

    // Send stop sequence
    digitalWrite(transmitPin, 1);
    delayMicroseconds(TIME_UNIT);
    digitalWrite(transmitPin, 0);
    delayMicroseconds(STOP_BIT_DURATION);
  }
}

void Transmitter::transmitBits(bool *bits, size_t length) {
  for (size_t i(0); i != length; ++i) {
    transmitBit(bits[i]);
  }
}

void Transmitter::transmitBit(bool bit) {
  if (bit) {
    // Transmit 1
    digitalWrite(transmitPin, 1);
    delayMicroseconds(TIME_UNIT);
    digitalWrite(transmitPin, 0);
    delayMicroseconds(TIME_UNIT * 5);

    digitalWrite(transmitPin, 1);
    delayMicroseconds(TIME_UNIT);
    digitalWrite(transmitPin, 0);
    delayMicroseconds(TIME_UNIT);
  } else {
    // Transmit 0
    digitalWrite(transmitPin, 1);
    delayMicroseconds(TIME_UNIT);
    digitalWrite(transmitPin, 0);
    delayMicroseconds(TIME_UNIT);

    digitalWrite(transmitPin, 1);
    delayMicroseconds(TIME_UNIT);
    digitalWrite(transmitPin, 0);
    delayMicroseconds(TIME_UNIT * 5);
  }
}
