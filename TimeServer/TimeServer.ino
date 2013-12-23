#include <Wire.h>

String timeOffsetStr; // Variable to hold time
char dataFromClient[10];  // Variable to hold data from client

volatile unsigned long clientTime;  // Variable to hold timestamp of client

const int syncStatusPin = 11;  // Pin that sync server status is output on

void setup()
{
  timeOffsetStr.reserve(10); // Reserve 10 bytes for unsigned long time
  Wire.begin(0x50); // Setup I2C slave address as 0x50     
  Wire.onRequest(requestEvent); // Register event for requested data
  Wire.onReceive(receiveEvent); // Register event for incomming data
  pinMode(syncStatusPin, OUTPUT);  // setup pin 11 as output
  digitalWrite(syncStatusPin, HIGH);  // set it to high
}

void loop() {
 // Do nothing
}

void receiveEvent(int bytes){
  digitalWrite(syncStatusPin, LOW); // let other arduinos know that we're busy
  int index = 0;  // index of char array
  while(Wire.available())   // while more bytes avail
  { 
    char c = Wire.read();    // receive a byte as character
    dataFromClient[index++] = c;  // store byte in char array
    
  }
  clientTime = atol(dataFromClient);  // convert char array to unsigned long
}

void requestEvent()
{
  timeOffsetStr = String(micros() - clientTime);  // convert timestamp to string
  volatile int timeLength = timeOffsetStr.length();  // find out how many digits are in the current time
  // If not 10 digits (max digit length for unsigned long), add leading zeros
  // This is to keep the time of I2C transfers consistent between subsequent read/writes
  if(timeLength == 1){
    timeOffsetStr = "000000000" + timeOffsetStr;
  }
  else if(timeLength == 2){
    timeOffsetStr = "00000000" + timeOffsetStr;
  }
  else if(timeLength == 3){
    timeOffsetStr = "0000000" + timeOffsetStr;
  }
  else if(timeLength == 4){
    timeOffsetStr = "000000" + timeOffsetStr;
  }
  else if(timeLength == 5){
    timeOffsetStr = "00000" + timeOffsetStr;
  }
  else if(timeLength == 6){
    timeOffsetStr = "0000" + timeOffsetStr;
  }
  else if(timeLength == 7){
    timeOffsetStr = "000" + timeOffsetStr;
  }
  else if(timeLength == 8){
    timeOffsetStr = "00" + timeOffsetStr;
  }
  else if(timeLength == 9){
    timeOffsetStr = "0" + timeOffsetStr;
  }
  Wire.print(timeOffsetStr); // respond with padded timestamp
  
  digitalWrite(syncStatusPin, HIGH); // let other arduinos know that we're clear
}
