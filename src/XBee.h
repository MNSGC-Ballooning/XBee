#ifndef XBee_h
#define XBee_h

#include <Arduino.h>

class XBee : public Stream {
  public:
    //Constructor
    XBee(Stream* port);
    //Virtual Stream functions
    int available() {return port->available();}
    int peek() {return port->peek();}
    int read() {return port->read();}
    //Virtual Print functions
    int availableForWrite() {return port->availableForWrite();}
    void flush() {port->flush();}
    size_t write(byte b) {return port->write(b);}
    //AT commands
    bool enterATmode();
    bool exitATmode();
    String atCommand(String command);
  private:
    //Connection used to communicate with XBee (eg. Serial1, SoftwareSerial, etc.)
    Stream* port;
};

#endif