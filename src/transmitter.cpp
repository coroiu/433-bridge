#include "transmitter.h"

Transmitter::Transmitter(int transmitPin): transmitPin(transmitPin) { };

bool* Transmitter::intToBitsArray(int number) {
  bool* bits = new bool[32];
  for (size_t i(0); i != 31; ++i) {
    bits[i] = (number >> i) & 1;
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

  }
}

void Transmitter::transmitBits(bool *bits, size_t length) {

}

void Transmitter::transmitBit(bool bit) {
  
}
