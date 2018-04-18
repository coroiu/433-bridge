#define TRANSMITTER_ID_MASK 0b11111111111111111111111111000000
#define GROUP_COMMAND_MASK  0b00000000000000000000000000100000
#define COMMAND_MASK        0b00000000000000000000000000010000
#define UNIT_CODE_MASK      0b00000000000000000000000000001111
#define TRANSMITTER_ID_SHIFT 6
#define GROUP_COMMAND_SHIFT 5
#define COMMAND_SHIFT 4
#define UNIT_CODE_SHIFT 0

#ifndef MESSAGE_H
#define MESSAGE_H

#if defined(SPARK)
    #include "application.h"
#endif

struct Message {
  unsigned int transmitterId, unitCode;
  bool isGroupCommand, command;

  Message(int fromInt);
  Message(unsigned int transmitterId, unsigned int unitCode, bool isGroupCommand, bool command):
    transmitterId(transmitterId), unitCode(unitCode),
    isGroupCommand(isGroupCommand), command (command) { }
  int toInt();
};

#endif
