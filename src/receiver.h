// #include <stdlib.h>
#include <cmath>

#if defined(SPARK)
    #include "application.h"
#endif

#include "message.h"
#include "digital_read.h"
#include "communication_bit.h"
#include "receiver_state.h"

#define TIME_UNIT 250 // ~319
#define TIME_UNIT_TOLERANCE TIME_UNIT / 2 // 319 - 250 = 69
#define START_BIT_DURATION TIME_UNIT * 10 // 2641
#define START_BIT_TOLERANCE TIME_UNIT
#define STOP_BIT_DURATION 9000
#define MESSAGE_BUFFER_LENGTH 32
#define RECEIVE_BUFFER_LENGTH MESSAGE_BUFFER_LENGTH * 2

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
