#ifndef RelayXBee_h
#define RelayXBee_h

#include "XBee.h"

class RelayXBee : public XBee {
  public:
    //Constructor
    RelayXBee(Stream* port, String id, char stack = 0);
    //Setup functions
    void init();
    void setCooldown(byte Cooldown);
    //Data I/O
    void send(String message);
    void sendGPS(byte hour, byte minute, byte second, float lat, float lon, float alt, byte sats);
    String receive();
  private:
    String id, lastCom, netID;
    byte cooldown = 10; //default time before accepting repeat transmissions is 10s
    unsigned long comTime;
    void acknowledge();
};

#endif