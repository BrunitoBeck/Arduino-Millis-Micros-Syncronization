#include <Wire.h>

char dataFromTimeServer[10]; // char array to hold I2C data from TimeServer
unsigned long timeFromTimeServer = 0;  // unsigned long to hold time from TimeServer

unsigned long currentSyncedTime = 0;
unsigned long timeOfLastSync = 0;

const int buttonPin = 10;

String timeValue; // Variable to hold time
void setup() {
  timeValue.reserve(10);
  
  Serial.begin(9600);
  
  syncTime(); // Sync time with TimeServer
  
  digitalWrite(buttonPin,HIGH);  // enable pull up resitor
  attachInterrupt(buttonPin, buttonPress, FALLING);  // register interrup on FALLING state
  
  digitalWrite(11,HIGH);  // enable pull up resitor
  pinMode(11, INPUT);  // set it up as an input
}


void loop() {
 if(currentSyncedTime!=0){
   Serial.println(currentSyncedTime);
   currentSyncedTime=0;
 }
 // Sync every 5 seconds if Time server is avail
 if((millis() - timeOfLastSync) > 5000 && digitalRead(11) != LOW){ 
   syncTime();
 }

}
// Run when falling
void buttonPress(){
  // Record timestamp in reference frame of TimeServer
  currentSyncedTime = timeFromTimeServer + micros();
}

// Method to read timestamp from central TimeServer over I2C
void syncTime(){
    //Serial.print("SYNCING...");
  Wire.begin();  // Initialize I2c  
  Wire.beginTransmission(0x50);
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
  Wire.endTransmission();

  Wire.requestFrom(0x50, 10);  // Request 10 bytes
  int index = 0;  // index of char array
  while(Wire.available())   // while more bytes avail
  { 
    char c = Wire.read();    // receive a byte as character
    dataFromTimeServer[index++] = c;  // store byte in char array
    
  }
  timeFromTimeServer = atol(dataFromTimeServer);  // convert char array to unsigned long
  
  timeOfLastSync = millis();  // update time of last sync
}  
