# XBee

This Arduino library is intended for Series 1 XBees not operating in API mode.
It provides functions to easily configure the radio settings via AT commands, in addition to sending and receiving data.
A subclass is also provided which encapsulates the SWAMP Comms Relay protocol for payloads.

## Use in code
### Object creation
This library defines the class `XBee` to drive the radio.
The constructor has the form
```cpp
XBee(&port);
```
where `port` is the `Stream` variable for the connection used by the XBee
(e.g. `Serial1` on a Mega or any `SoftwareSerial` object already created).
Note the preceding `&` - this is necessary as the constructor expects a pointer to the Stream object.
Thus, for an XBee attached to Serial 1, the constructor would look like
```cpp
XBee(&Serial1);
```

### Initialization
Before using any other functions of the XBee object, the port used to interface with the XBee must have its `begin()` function called during `setup()`:
```cpp
Serial1.begin(XBEE_BAUD); //If connected over Serial1
ss.begin(XBEE_BAUD); //If using a SoftwareSerial connection named ss
```
`XBEE_BAUD` is a constant defining the default baudrate XBees operate at for easy reference.
It has the value 9600.

### Configuring the XBee
Although the XBee settings can be set using external software (e.g. XCTU),
it is often more convenient to overwrite settings using the microcontroller so that any module can be plugged in and guaranteed to work.
This is done through the use of the following three functions (assume `xbee` is an object of type `XBee`):
```cpp
xbee.enterATmode();
String response = xbee.atCommand(command);
xbee.exitATmode();
```
`enterATmode()` must be called before attempting to write any settings,
as this tells the XBee that incoming data is configuration data rather than something to transmit.
It returns a boolean `true` if the XBee recognizes the instruction, `false` otherwise.
However, the latter case is very rare, so this value can typically be ignored.

Similarly, `exitATmode()` informs the XBee that configuration is finished and it should return to transmitting incoming data.
Note that the XBee automatically exits AT mode after 10 seconds of inactivity should this function fail to be called.
However, `exitATmode()` also writes the new settings to memory so that they are preserved the next time the radio powers on.
If the XBee exits via timeout, the new settings are lost when it is turned off.
Like `enterATmode()`, this function returns a boolean indicating success, but as failure is rare it can generally be ignored.

`atCommand()` is the function which actually sends settings changes to the XBee.
It takes a String `command` to send which corresponds to the setting change desired.
Typical AT commands include
- ATID: the network ID of the XBee.
All XBees intending to communicate with one another must share the same network ID.
- ATMY: the address of this XBee ("My address").
Only other XBee units with this value as their DL address will be able to send transmissions to this XBee.
- ATDL: the address to transmit to ("Destination").
Only XBee units with this value as their MY address will be able to receive transmissions from this XBee.

If these commands are sent in isolation, the XBee will respond with the current value.
For example, if the XBee's network ID was 1234, then `xbee.atCommand("ATID");` would return "1234" as a String.
To change a setting, append the desired value to the setting to change; the XBee will respond "OK" if the change is successful.
(As usual, failure is rare and this acknowledgement can be ignored.)
For example, to set the ID to 4321, use `xbee.atCommand("ATID4321");`

Typically, settings changes should be done once somewhere in `setup()`,
but if changes need to be made on the fly there is nothing preventing these functions from being called in `loop()`.

### Sending and Receiving Data
The `XBee` class extends `Stream` (the parent class of both Serial objects and `SoftwareSerial`.
Therefore, all of the member functions of `Stream` are defined and available to any `XBee` object.
For example, the functions
```cpp
xbee.write('c');
xbee.print("Hello World");
xbee.println();
xbee.available();
xbee.read();
xbee.readStringUntil('\n');
```
all should work as expected.

Effectively, the XBees function as a wireless Serial connection.
Any data sent out one XBee via write functions is available to the receiving XBee through read functions.

## Relay Mode
The `XBee` class defines basic functions necessary to use XBee radios in a variety of systems.
This library also defines a subclass, `RelayXBee', which adds capabilities specific to the SWAMP comms relay system.
This class should only be used with the SWAMP system, as it adds a more complex addressing protocol allowing the central relay to talk to multiple end devices.

### Object Creation
As with its parent class, the `RelayXBee` constructor requires a reference to a `Stream` object used to interface with the XBee.
In addition, it takes a user-supplied String that defines a payload ID:
```cpp
XBee(&Serial1, id);
```
This should generally be 2-4 characters, A-Z and 0-9 only.
For example, the infamous VentArrow project might use the following for its constructor:
```cpp
RelayXBee xbee = RelayXBee(&Serial1, "VA");
```

### Initialization
In addition to calling the appropriate `begin()` method, the `RelayXBee` object has its own initialization method:
```cpp
xbee.init();
```
This in turn calls the appropriate `atCommand()` functions to configure the XBee to talk to the SWAMP relay.
By default, this leaves the network ATID unchanged.
If the balloon stack (A, B, or C) that the payload will fly on is known, this can be specified with an optional parameter:
```cpp
xbee.init('B'); //Set the network ID to match the SWAMP relay on stack B
```

### Sending and Receiving Data
While all of the print and read functions from the `XBee` class are still available, these should generally be avoided.
Instead, use the new `send()` and `receive()` functions, which ensure only messages intended for this payload are actaully processed:
```cpp
xbee.send(data);
String message = xbee.receive();
```
`data` is a string which can contain sensor data or status messages.
`receive()` returns a string containing any message addressed to this payload,
which typically command some sort of action or request specific data for transmission.

Note that due to the nature of how SWAMP operates, the same message will be ignored if received multiple times in a row.
This is done to avoid repeated actions as a result of repeat transmissions, which are typically used to ensure data reaches the payload.
By default, 10 seconds must pass before the same message is considered "new";
this cooldown can be changed with the function
```cpp
xbee.setCooldown(5); //This would reduce the cooldown to 5 seconds
```

Lastly, the ground station associated with SWAMP expects payload GPS data in a certain format.
To ensure this is done correctly, a separate `sendGPS()` function is used which expects raw numeric values for relavent gps data:
```cpp
xbee.sendGPS(hour, min, sec, lat, lon, alt, sats);
```