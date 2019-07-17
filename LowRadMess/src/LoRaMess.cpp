#include <LoRa.h>
#include <EEPROM.h>
#include "LoRaMess.h"
byte localAddress = 0xBB;
byte destination = 0xEB; //FliptheFlop and Kat named it like that.
int messageCount = 0;
int EEPROMadress = 0;
bool ableToSend;

void readMessage() {
  // read packet header bytes:
  int recipient = LoRa.read();          // recipient address
  byte sender = LoRa.read();            // sender address
  byte incomingLength = LoRa.read();    // incoming msg length

  String incoming = "";

  // if the recipient isn't this device or broadcast,
  //Needs much better implementation but it will do for now.
  if (recipient != localAddress && recipient != destination) {

#ifdef USE_DEBUG
    Serial.println("This message is not for me.");
#endif

    return;                             // skip rest of function
  }

  while (LoRa.available()) {
    incoming += (char)LoRa.read() - 100;
  }

  if (incomingLength != incoming.length()) {   // check length for error

#ifdef USE_DEBUG
    Serial.println("error: message length does not match length");
#endif

    return;                             // skip rest of function
  }

  // if message is for this device, or broadcast, print details:
#ifdef USE_DEBUG
  Serial.println("Received from: 0x" + String(sender, HEX));
  Serial.println("Sent to: 0x" + String(recipient, HEX));
  Serial.println("Message length: " + String(incomingLength));
  Serial.println("Message: " + incoming);
  Serial.println();
#endif
  Bluetooth.println("Message: " + incoming);
  Bluetooth.println();
}

void sendMessage(String outgoing) {
  if (ableToSend) {
  if (EEPROM.read(EEPROMadress) < 10) {
  LoRa.beginPacket();                   // start packet
  LoRa.write(destination);              // add destination address
  LoRa.write(localAddress);             // add sender address
  LoRa.write(outgoing.length());        // add payload length
  LoRa.print(outgoing);                 // add payload
  LoRa.endPacket();                     // finish packet and send it
  messageCount++;
  EEPROM.write(EEPROMadress, messageCount);
}
}
else {
  Bluetooth.println("You exceeded number of messages per day (10)");
}
}

void onReceive(int packetSize) {
  if (packetSize == 0) return;          // if there's no packet, return

  readMessage();
}
