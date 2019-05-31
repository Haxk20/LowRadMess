//TODO:
//Encryption
#include <LoRa.h> // include LoRa library
//#define USE_DEBUG //Used for debugging stuff
#define Bluetooth Serial1
byte localAddress = 0xBB;     // address of this device
byte destination = 0xFF;      // destination to send to

void setup() {
#ifdef USE_DEBUG
  Serial.begin(9600);  // initialize serial
#endif
  Bluetooth.begin(9600);


  while (!Serial);
  while (!Bluetooth);

  if (!LoRa.begin(866E6)) {    // initialize radio at 866 MHz

#ifdef USE_DEBUG
    Serial.println("LORA::ERROR >> 404");
#endif
    Bluetooth.println("LORA::ERROR >> 404");


    while (true); // if failed, do nothing
  }


#ifdef USE_DEBUG
  Serial.println("Welcome to LowRadMess (DEBUG)");
#endif
  Bluetooth.println("Welcome to LowRadMess");
}

void loop() {
  if (Bluetooth.available()) {
    String message = "";

    // read message from bluetooth
    while (Bluetooth.available()) {
      message += Bluetooth.readString();
    }

#ifdef USE_DEBUG
    Serial.println("You: " + message);
#endif
    Bluetooth.println("You: " + message);

    sendMessage(message);
  }

  // parse for a packet, and call onReceive with the result:
  onReceive(LoRa.parsePacket());
}

void sendMessage(String outgoing) {
  LoRa.beginPacket();                   // start packet
  LoRa.write(destination);              // add destination address
  LoRa.write(localAddress);             // add sender address
  LoRa.write(outgoing.length());        // add payload length
  LoRa.print(outgoing);                 // add payload
  LoRa.endPacket();                     // finish packet and send it
}

void onReceive(int packetSize) {
  if (packetSize == 0) return;          // if there's no packet, return

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
    incoming += (char)LoRa.read();
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
