#ifndef RDM6300_h
#define RDM6300_h

#include <Arduino.h>
#include <SoftwareSerial.h>

class RDM6300 {
  public:
    RDM6300(HardwareSerial& serial);
    RDM6300(SoftwareSerial& serial);
    long read();
  private:
    Stream* _serial;
};

RDM6300::RDM6300(HardwareSerial& serial) {
  _serial = &serial;
}

RDM6300::RDM6300(SoftwareSerial& serial) {
  _serial = &serial;
}

long RDM6300::read() {
  uint8_t header[2];
  uint8_t data[8];
  uint8_t checksum;
  
  // Wait for data to be available
  while (_serial->available() < 10) {
    delay(1);
  }
  
  // Read header and data bytes
  _serial->readBytes(header, 2);
  _serial->readBytes(data, 8);
  
  // Calculate checksum
  checksum = header[0] ^ header[1];
  for (int i = 0; i < 8; i++) {
    checksum ^= data[i];
  }
  
  // Verify checksum
  if (checksum == 0) {
    long value = 0;
    for (int i = 0; i < 8; i++) {
      value = (value << 8) | data[i];
    }
    return value;
  } else {
    return -1;
  }
}

#endif