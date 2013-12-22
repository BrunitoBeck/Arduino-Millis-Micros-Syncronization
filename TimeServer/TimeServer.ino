#include <Wire.h>

String timeValue; // Variable to hold time

void setup()
{
  timeValue.reserve(10); // Reserve 10 bytes for unsigned long time
  Wire.begin(0x50); // Setup I2C slave address as 0x50     
  Wire.onRequest(requestEvent); // Register event
}

void loop() {
 // Do nothing
}

void requestEvent()
{
  timeValue = String(micros());  // convert timestamp to string
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
}
