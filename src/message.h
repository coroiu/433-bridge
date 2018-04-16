#define TRANSMITTER_ID_MASK 0b11111111111111111111111111000000
#define GROUP_COMMAND_MASK  0b00000000000000000000000000100000
#define COMMAND_MASK        0b00000000000000000000000000010000
#define UNIT_CODE_MASK      0b00000000000000000000000000001111

#ifndef MESSAGE_H
#define MESSAGE_H

struct Message {
  unsigned int transmitterId, unitCode;
  bool isGroupCommand, command;

  Message(unsigned int transmitterId, unsigned int unitCode, bool isGroupCommand, bool command):
    transmitterId(transmitterId), unitCode(unitCode),
    isGroupCommand(isGroupCommand), command (command) { }
};

#endif
