#include <Wire.h>

String timeValue; // Variable to hold time
char dataFromClient[10];

volatile unsigned long clientTime;
void setup()
{
  timeValue.reserve(10); // Reserve 10 bytes for unsigned long time
  //clientTime.reserve(10); // Reserve 10 bytes for unsigned long time
  Wire.begin(0x50); // Setup I2C slave address as 0x50     
  Wire.onRequest(requestEvent); // Register event
  Wire.onReceive(receiveEvent); // Register event
  pinMode(11, OUTPUT);  // setup pin 11 as output
  digitalWrite(11, HIGH);  // set it to high
}

void loop() {
 // Do nothing
}
void receiveEvent(int bytes){
  digitalWrite(11, LOW); // let other arduinos know that we're busy
  
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

  
  timeValue = String(micros() - clientTime);  // convert timestamp to string
  volatile int timeLength = timeValue.length();  // find out how many digits are in the current time
  // If not 10 digits (max digit length for unsigned long), add leading zeros
  // This is to keep the time of I2C transfers consistent between subsequent read/writes
  if(timeLength == 1){
    timeValue = "000000000" + timeValue;
  }
  else if(timeLength == 2){
    timeValue = "00000000" + timeValue;
  }
  else if(timeLength == 3){
    timeValue = "0000000" + timeValue;
  }
  else if(timeLength == 4){
    timeValue = "000000" + timeValue;
  }
  else if(timeLength == 5){
    timeValue = "00000" + timeValue;
  }
  else if(timeLength == 6){
    timeValue = "0000" + timeValue;
  }
  else if(timeLength == 7){
    timeValue = "000" + timeValue;
  }
  else if(timeLength == 8){
    timeValue = "00" + timeValue;
  }
  else if(timeLength == 9){
    timeValue = "0" + timeValue;
  }
  Wire.print(timeValue); // respond with padded timestamp
  
  digitalWrite(11, HIGH); // let other arduinos know that we're clear
}
