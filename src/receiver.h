#ifndef RECEIVER_H
#define RECEIVER_H

#include <cmath>

#if defined(SPARK)
    #include "application.h"
#endif

#include "constants.h"
#include "message.h"
#include "digital_read.h"
#include "communication_bit.h"
#include "receiver_state.h"

using namespace std;

class Receiver {
  static inline bool matchesDuration(unsigned long a, unsigned long b, unsigned long tolerance) {
    return std::abs((signed long) a - (signed long) b) < tolerance;
  }

  int receivePin;
  ReceiverState state;
  DigitalRead lastRead;
  CommunicationBit lastReceivedBit;

  bool receiveBuffer[RECEIVE_BUFFER_LENGTH];
  size_t receiveBufferIndex;

  bool data[MESSAGE_BUFFER_LENGTH];
  size_t dataLength;

  void handleRadioInput();
  void abortCurrentReceive();
  void completeCurrentReceive();
  void addBitToReceiveBuffer(int bit);
  void setState(ReceiverState state);

public:
  Receiver(int receivePin);
  void startReceive();
  void stopReceive();
  // CommunicationBit getLastReceived();
  int availableData();
  const bool* getBuffer();
  const bool* getData();
  const Message getMessage();
};

#endif
