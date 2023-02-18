#ifndef RDM6300_h
#define RDM6300_h

#include <SoftwareSerial.h>

class RDM6300 {
public:
    RDM6300(int rxPin, int txPin);

    void begin();

    bool available();

    void read(unsigned long &tag);

private:
    SoftwareSerial _serial;
};

#endif