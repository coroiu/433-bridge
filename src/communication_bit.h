struct CommunicationBit {
  unsigned long duration;
  bool value;
  CommunicationBit(): duration(0), value(false) { }
  CommunicationBit(long duration, bool value): duration(duration), value(value) { }
};
