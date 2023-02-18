#include "RDM6300.h"

RDM6300::RDM6300(int rxPin, int txPin) : _serial(rxPin, txPin) {}

void RDM6300::begin() {
    _serial.begin(9600);
}

bool RDM6300::available() {
    return _serial.available() >= 12;
}

void RDM6300::read(unsigned long &tag) {
    if (available()) {
        _serial.read(); // discard start byte
        tag = 0;
        for (int i = 0; i < 5; i++) {
            tag <<= 8;
            tag |= _serial.read();
        }
        _serial.read(); // discard checksum byte
    }
}