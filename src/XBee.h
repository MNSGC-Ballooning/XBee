#ifndef XBee_h
#define XBee_h

#include <Arduino.h>

class XBee {
  public:
    XBee(Stream* port);
    int available();
    void flush();
    byte peek();
    void print(String str);
    void println(String str);
    byte read();
    int readBytes(byte* buffer, int length);
    int readBytesUntil(char character, byte* buffer, int length);
    String readString();
    String readStringUntil(char terminator);
    void setTimeout(long time);
    void write(byte b);
    void write(byte* buffer, int length);
    
    bool enterATmode();
    bool exitATmode();
    String atCommand(String command);
  private:
    Stream* port;
};

#endif