#include "receiver.h"

Receiver::Receiver(int receivePin):
  receivePin(receivePin), receiveBufferIndex(0), dataLength(0) { }

void Receiver::startReceive() {
  setState(ReceiverState::waitingForMessageHigh);
  attachInterrupt(receivePin, &Receiver::handleRadioInput, this, CHANGE);
}

void Receiver::stopReceive() {
  detachInterrupt(receivePin);
}

void Receiver::setState(ReceiverState state) {
  this->state = state;
  // if (state == ReceiverState::waitingForMessageHigh) {
  //   Serial.println("Waiting for message HIGH");
  // } else if (state == ReceiverState::waitingForMessageLow) {
  //   Serial.println("Waiting for message LOW");
  // } else if (state == ReceiverState::waitingForFirstHalfOfBit) {
  //   Serial.println("Waiting for FIRST half of bit");
  // } else if (state == ReceiverState::waitingForSecondHalfOfBit) {
  //   Serial.println("Waiting for SECOND half of bit");
  // }
}

int Receiver::availableData() {
  return dataLength;
}

const bool* Receiver::getBuffer() {
  return receiveBuffer;
}

const bool* Receiver::getData() {
  dataLength = 0;
  return data;
}

const Message Receiver::getMessage() {
  int messageInt(0);
  for (int i(31); i >= 0; --i) {
    if (data[31 - i]) {
      unsigned int bitMask = 1 << i;
      messageInt |= bitMask;
    }
  }

  dataLength = 0;
  return Message(messageInt);
}

void Receiver::addBitToReceiveBuffer(int bit) {
  if (receiveBufferIndex < RECEIVE_BUFFER_LENGTH) {
    receiveBuffer[receiveBufferIndex++] = bit;
  } else {
    abortCurrentReceive();
    setState(ReceiverState::waitingForMessageHigh);
  }
}

void Receiver::abortCurrentReceive() {
  receiveBufferIndex = 0;
}

void Receiver::completeCurrentReceive() {
  for (size_t i(0); i < receiveBufferIndex; i += 2) {
    if (receiveBuffer[i] == 0 && receiveBuffer[i+1] == 1) {
      data[dataLength++] = 0;
    } else if (receiveBuffer[i] == 1 && receiveBuffer[i+1] == 0) {
      data[dataLength++] = 1;
    } else {
      // Malformed data in buffer, abort
      receiveBufferIndex = 0;
      dataLength = 0;
      return;
    }
  }
  receiveBufferIndex = 0;
}

void Receiver::handleRadioInput() {
  DigitalRead read(micros(), -digitalRead(receivePin));
  CommunicationBit receivedBit(read.receivedAt - lastRead.receivedAt, lastRead.value);

  // if (bitsLength < MESSAGE_BUFFER_LENGTH) {
  //   bits[bitsLength++] = receivedBit;
  // }

  if (state == ReceiverState::waitingForMessageHigh) {
    if (Receiver::matchesDuration(receivedBit.duration, TIME_UNIT, TIME_UNIT_TOLERANCE) && receivedBit.value == 1) {
      setState(ReceiverState::waitingForMessageLow);
    }
  } else if (state == ReceiverState::waitingForMessageLow) {
    if (Receiver::matchesDuration(receivedBit.duration, START_BIT_DURATION, START_BIT_TOLERANCE) && receivedBit.value == 0) {
      setState(ReceiverState::waitingForFirstHalfOfBit);
    } else {
      setState(ReceiverState::waitingForMessageHigh);
    }
  } else if (state == ReceiverState::waitingForFirstHalfOfBit) {
    if (Receiver::matchesDuration(receivedBit.duration, TIME_UNIT, TIME_UNIT_TOLERANCE) && receivedBit.value == 1) {
      setState(ReceiverState::waitingForSecondHalfOfBit);
    } else {
      abortCurrentReceive();
      setState(ReceiverState::waitingForMessageHigh);
    }
  } else if (state == ReceiverState::waitingForSecondHalfOfBit) {
    if (Receiver::matchesDuration(receivedBit.duration, TIME_UNIT * 5, TIME_UNIT_TOLERANCE) && receivedBit.value == 0) {
      addBitToReceiveBuffer(1);
      setState(ReceiverState::waitingForFirstHalfOfBit);
    } else if (Receiver::matchesDuration(receivedBit.duration, TIME_UNIT, TIME_UNIT_TOLERANCE) && receivedBit.value == 0) {
      addBitToReceiveBuffer(0);
      setState(ReceiverState::waitingForFirstHalfOfBit);
    } else if (receivedBit.duration > STOP_BIT_DURATION && receivedBit.value == 0) {
      completeCurrentReceive();
      setState(ReceiverState::waitingForMessageHigh);
    } else {
      abortCurrentReceive();
      setState(ReceiverState::waitingForMessageHigh);
    }
  }

  lastRead = read;
}
