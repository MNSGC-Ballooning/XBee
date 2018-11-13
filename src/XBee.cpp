#include "XBee.h"

XBee::XBee(Stream* port):
  port(port) {
}

bool XBee::enterATmode() {
  String response;
  for (byte i = 0; response.equals("") && (i < 10); i++) {
    print("+++");
    delay(1000);
    response = readStringUntil('\r');
  }
  return response.equals("OK");
}

bool XBee::exitATmode() {
  String response = atCommand("ATWR");
  atCommand("ATCN");
  return response.equals("OK");
}

String XBee::atCommand(String command) {
  print(command);
  write('\r');
  return readStringUntil('\r');
}
