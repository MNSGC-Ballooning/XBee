/*Program to send regular TMP temp sensor readings via XBee using the SWAMP Comms Relay Protocol.
  Uses the RelayXBee subclass of the XBee library
 */

 //XBee library
 #include <RelayXBee.h>
//SoftwareSerial library - needed for Uno
#include <SoftwareSerial.h>

//Temp sensor pin
#define TMP_pin A0;

//Define (or create) the serial connection used by the XBee - choose one
//SoftwareSerial (Only option on Uno)
SoftwareSerial ss = SoftwareSerial(2,3);//Connect on pins 2-3
#define XBee_Serial ss
//HardwareSerial (Mega, Teensy, etc.)
//#define XBee_Serial Serial1

//Define payload ID - 2-4 characters, A-Z 0-9 only
String xBee_ID = "PL1"; //e.g. "Payload 1"

//Create XBee object with serial reference
RelayXBee xBee = RelayXBee(&XBee_Serial, xBee_ID);



void setup() {
  //Declare temp sensor input pin
  pinMode(TMP_pin, INPUT);

  //Open serial communication with XBee
  XBee_Serial.begin(XBEE_BAUD);

  //Configure XBee to transmit to a relay on stack A
  xBee.init('A');

  //Change cooldown before repeat transmission is accepted - default is 10s if this isn't called
  xBee.setCooldown(5); //e.g. change from default 10s to 5s.
}

void loop() {
  
}

//function to read the temp sensor and return the current temp in C
float getTemp() {
  return (analogRead(TMP_pin)*(5.0/1024)-.5)/.01;
}