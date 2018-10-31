#include "XBee.h"

XBee::XBee(Stream* port):
  port(port) {
}

int XBee::available() {
  return port->available();
}

void XBee::flush() {
  port->flush();
}

byte XBee::peek() {
  return port->peek();
}

void XBee::print(String str) {
  char* buffer = new char[str.length()+1];
  str.toCharArray(buffer, str.length()+1);
  port->write(buffer, str.length()+1);
  delete[] buffer;
}

void XBee::println(String str) {
  char* buffer = new char[str.length()+1];
  str.toCharArray(buffer, str.length()+1);
  port->write(buffer, str.length()+1);
  port->write('\n');
  delete[] buffer;
}

byte XBee::read() {
  return port->read();
}

int XBee::readBytes(byte* buffer, int length) {
  return port->readBytes(buffer, length);
}

int XBee::readBytesUntil(char character, byte* buffer, int length) {
  return port->readBytesUntil(character, buffer, length);
}

String XBee::readString() {
  return port->readString();
}

String XBee::readStringUntil(char terminator) {
  return port->readStringUntil(terminator);
}

void XBee::setTimeout(long time) {
  port->setTimeout(time);
}

void XBee::write(byte b) {
  port->write(b);
}

void XBee::write(byte* buffer, int length) {
  port->write(buffer, length);
}

bool XBee::enterATmode() {
  String response;
  for (byte i = 0; response.equals("") && (i < 10); i++) {
    print("+++");
    delay(1000);
    response = port->readStringUntil('\r');
  }
  return response.equals("OK");
}

bool XBee::exitATmode() {
  String response = atCommand("ATWR");
  atCommand("ATCN");
  return response.equals("OK");
}

String XBee::atCommand(String command) {
  port->print(command);
  port->write('\r');
  return port->readStringUntil('\r');
}
