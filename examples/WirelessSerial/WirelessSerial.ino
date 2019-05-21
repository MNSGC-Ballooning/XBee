/*Program to create a "wireless Serial" connection between two Arduinos using a pair of XBees.
  User input to the Serial monitor is routed through the XBee to the other Arduino,
  which prints the incoming data to *its* Serial monitor
  Code for both Arduinos is contained in this sketch; to change which you compile for,
  uncomment the appropriate macro (lines 15-16).
*/

//XBee library
#include <XBee.h>
//SoftwareSerial library - needed for Uno
#include <SoftwareSerial.h>

//Compiler Macros - Uncomment the first when uploading to the first arduino,
//and the second when uploading to the other
//#define Arduino_0
//#define Arduino_1

//Define (or create) the serial connection used by the XBee - choose one
//SoftwareSerial (Only option on Uno)
SoftwareSerial ss = SoftwareSerial(2,3);//Connect on pins 2-3
#define XBee_Serial ss
//HardwareSerial (Mega, Teensy, etc.)
//#define XBee_Serial Serial1

//Create XBee object with serial reference
XBee xBee = XBee(&XBee_Serial);

//Select a 4-digit hexadecimal network ID, shared by both XBees.
//May need to change from default if other networks are in close proximity to avoid interference
String networkID = "A12B";

void setup() {
  //Open serial connection to computer
  Serial.begin(115200);
  //Open serial communication with XBee
  XBee_Serial.begin(XBEE_BAUD);
  
  //Change XBee settings to pair them
  xBee.enterATmode();
  xBee.atCommand("ATID" + networkID);
  #ifdef Arduino_0
  xBee.atCommand("ATMY0");
  xBee.atCommand("ATDL1");
  #endif
  #ifdef Arduino_1
  xBee.atCommand("ATMY1");
  xBee.atCommand("ATDL0");
  #endif
  xBee.exitATmode();
}

void loop() {
  //Pipe data from Serial monitor to XBee and vice versa
  while (xBee.available())
    Serial.write(xBee.read());
  while (Serial.available())
    xBee.write(Serial.read());
}
