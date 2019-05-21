#ifndef RelayXBee_h
#define RelayXBee_h

#include "XBee.h"

class RelayXBee : public XBee {
  public:
    //Constructor
    RelayXBee(Stream* port, String id);
    //Setup functions
    void init(char stack=0);
    void setCooldown(byte Cooldown);
    //Data I/O
    void send(String message);
    void sendGPS(byte hour, byte minute, byte second, float lat, float lon, float alt, byte sats);
    String receive();
  private:
    String id, lastCom;
    byte cooldown = 10; //default time before accepting repeat transmissions is 10s
    unsigned long comTime;
    void acknowledge();
};

#endif