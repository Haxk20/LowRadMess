//TODO:
//Encryption
#include <LoRa.h>
#define Bluetooth Serial1
#define USE_DEBUG
void setup() {
  Bluetooth.begin(9600);
  #ifdef USE_DEBUG
  Serial.begin(9600);
  #endif
  Bluetooth.println("Welcome to LowRadMess");
#ifdef USE_DEBUG
  Serial.println("Welcome to LowRadMess");
#endif
  if (!LoRa.begin(866E6)) {
    Bluetooth.println("LORA::ERROR >> 404");
#ifdef USE_DEBUG
    Serial.println("LORA::ERROR >> 404");
#endif
    while (true) {
      delay(1000);
    }
  }
}

void loop() {
  // is bluetooth port ready for reading
  if (Bluetooth.available()) {
    String message = "";

    // read message from bluetooth
    while (Bluetooth.available()) {
      message += Bluetooth.readString();
    }

    Bluetooth.println("You: " + message);
#ifdef USE_DEBUG
    Serial.println("You: " + message);
#endif

    // start packet write to it and close it and reset message
    LoRa.beginPacket();
    LoRa.print(message);
    LoRa.endPacket();
  }

  onReceive(LoRa.parsePacket());
}

void onReceive(int packetSize) {
  if (packetSize == 0) return;
  String report = "";

  // read message from lora
  while (LoRa.available()) {
    report += (char)LoRa.read();
  }
#ifdef USE_DEBUG
  Serial.println("Received: " + report);
#endif
  Bluetooth.println("Received: " + report);
}

