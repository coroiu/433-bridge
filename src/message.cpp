#include "message.h"

Message::Message(int messageInt) {
  transmitterId  = (messageInt & TRANSMITTER_ID_MASK) >> TRANSMITTER_ID_SHIFT,
  unitCode       = ((~messageInt) & UNIT_CODE_MASK) >> UNIT_CODE_SHIFT;
  // The shifts below could actually be skipped because of the
  // implicit conversion to bool that does a > 1 check.
  isGroupCommand = (messageInt & GROUP_COMMAND_MASK) >> GROUP_COMMAND_SHIFT;
  command        = (messageInt & COMMAND_MASK) >> COMMAND_SHIFT;
}

int Message::toInt() {
  int toReturn =
    (transmitterId << TRANSMITTER_ID_SHIFT) |
    (((~unitCode) << UNIT_CODE_SHIFT) & UNIT_CODE_MASK) |
    ((int) isGroupCommand << GROUP_COMMAND_SHIFT) |
    ((int) command << COMMAND_SHIFT);
  Serial.println(toReturn);
  //return 1594817437;
  return toReturn;
}
