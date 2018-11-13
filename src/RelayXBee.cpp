#include "RelayXBee.h"

RelayXBee::RelayXBee(Stream* port, String id, char stack):
  XBee(port), id(id) {
  switch (stack) {
    case 'A':
      netID = "AAAA";
      break;
    case 'B':
      netID = "BBBB";
      break;
    case 'C':
      netID = "CCCC";
      break;
    default:
      netID = "";
  }
}

//Call during setup to configure radio settings
void RelayXBee::init() {
  enterATmode();
  atCommand("ATID"+netID);
  atCommand("ATDL0");
  atCommand("ATMY1");
  exitATmode();
}

//sets cooldown between repeat commands
void RelayXBee::setCooldown(byte cooldown) {
  this->cooldown = cooldown;
}

void RelayXBee::send(String message) {
  print(id);
  write(';');
  print(message);
  write('!');
  println();
}

//sends GPS string in format ground station accepts
void RelayXBee::sendGPS(byte hour, byte minute, byte second,
  float lat, float lon, float alt, byte sats) {
  print(id);
  write(';');
  print(hour);
  write(':');
  print(minute);
  write(':');
  print(second);
  write(';');
  print(lat,4);
  write(';');
  print(lon,4);
  write(';');
  print(alt,1);
  write(';');
  print(sats);
}

//parses incoming XBee transmissions for valid commands with XBee's id
//and returns just the command as a string if found
String RelayXBee::receive() {
  String command = readStringUntil('!');
  //Since relay transmits commands repeatedly, return an empty string if command is a repeat
  if (command.equals(lastCom) && (millis() - comTime < cooldown * 1000))
    return "";
  byte split = command.indexOf('?');  //question mark separates id from command
  if (!(command.substring(0, split)).equals(id))
    return "";  //returns empty string if wrong id
  lastCom = command;  //once a valid command is received, save it to avoid repeats...
  comTime = millis(); //...for a short time
  acknowledge();      //inform relay that command was received
  //return just the command portion as a string
  return (command.substring(split + 1, command.length()));
}

//used to tell the ground station that the transmission was received
void RelayXBee::acknowledge() {
  println(id);
}