//TODO:
//Encryption
//Extend range (Mesh Network)
#include <LoRa.h> // include LoRa library
#include <EEPROM.h>
#include <Wire.h>
#include "RTClib.h"
#include "src/LoRaMess.h"
#include "src/CheckTime.h"

//#define USE_DEBUG //Used for debugging stuff
#define EXPERIMENTAL //Use for enabling experimental features that have not been tested properly.
#define RTC;
#define Bluetooth Serial1
long frequency = 0;
int dayAdress = 1;

void setup() {
  ableToSend = false;
  #ifndef EXPERIMENTAL
#ifdef USE_DEBUG
  Serial.begin(9600);  // initialize serial
#endif
  Bluetooth.begin(9600);
#endif

  while (!Serial);
  while (!Bluetooth);

#ifdef EXPERIMENTAL
    if (!LoRa.begin(frequency)) { //EXPERIMENTAL DONT USE FOR NOW NOT TESTED. Country frequency detection. Depend on country (433,866.915 Mhz)
    Bluetooth.println("Cannot enable LoRa. Check connections.");

    while (true); // if failed, do nothing
}


#else
  if (!LoRa.begin(866E6)) {    // initialize radio at 866 MHz

#ifdef USE_DEBUG
    Serial.println("Cannot enable LoRa. Check connections.");
#endif
    Bluetooth.println("Cannot enable LoRa. Check connections.");


    while (true); // if failed, do nothing
  }
#endif

#ifdef USE_DEBUG
  Serial.println("Welcome to LowRadMess (DEBUG)");
#endif
  Bluetooth.println("Welcome to LowRadMess");

  startRTC();
  updateTime();
  if(EEPROM.read(dayAdress) != checkDay()) {
  EEPROM.write(dayAdress, checkDay());
  ableToSend = true;
  }

}

void loop() {
  updateTime();

  if (Bluetooth.available()) {
    String message = "";
#ifdef EXPERIMENTAL
    // read message from bluetooth
    while (Bluetooth.available()) {
      message += Bluetooth.readString() + 100;
    }
#else
// read message from bluetooth
while (Bluetooth.available()) {
  message += Bluetooth.readString();
}
#endif

#ifdef USE_DEBUG
    Serial.println("You: " + message);
#endif
    Bluetooth.println("You: " + message);
    sendMessage(message);

  }

  // parse for a packet, and call onReceive with the result:
  onReceive(LoRa.parsePacket());
}
