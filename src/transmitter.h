#ifndef TRANSMITTER_H
#define TRANSMITTER_H

#include <cmath>

#if defined(SPARK)
    #include "application.h"
#endif

#include "constants.h"
#include "message.h"

using namespace std;

class Transmitter {
  int transmitPin;

  bool* intToBitsArray(int number); // returned length always 32
  inline void transmitBit(bool bit);
  inline void transmitBits(bool* bits, size_t length);
public:
  Transmitter(int transmitPin);
  void transmit(Message message);
};

#endif
