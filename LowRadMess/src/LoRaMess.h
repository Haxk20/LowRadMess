#ifndef READMESSAGE_H
  #define READMESSAGE_H
  #define Bluetooth Serial1
extern byte localAddress; // = 0xBB;     // address of this device
extern byte destination; // = 0xFF;      // destination to send to
extern int messageCount;
extern bool ableToSend;
void readMessage();
void sendMessage( String);
void onReceive( int);
#endif
