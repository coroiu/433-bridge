#ifndef DIGITAL_READ_H
#define DIGITAL_READ_H

struct DigitalRead {
  unsigned long receivedAt;
  bool value;
  DigitalRead(): receivedAt(0), value(false) { }
  DigitalRead(long receivedAt, bool value): receivedAt(receivedAt), value(value) { }
};

#endif
