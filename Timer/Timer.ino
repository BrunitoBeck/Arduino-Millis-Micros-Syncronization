#include <Wire.h>

char dataFromTimeServer[10]; // char array to hold I2C data from TimeServer
unsigned long timeOffset = 0;  // unsigned long to hold time from TimeServer

unsigned long currentSyncedTime = 0;
unsigned long timeOfLastSync = 0;

const int buttonPin = 10;
const int syncStatusPin = 11;

void setup() {
  
  Serial.begin(9600);
  
  syncTime(); // Sync time with TimeServer
  
  digitalWrite(buttonPin,HIGH);  // enable pull up resitor
  attachInterrupt(buttonPin, buttonPress, FALLING);  // register interrup on FALLING state
  
  digitalWrite(syncStatusPin,HIGH);  // enable pull up resitor
  pinMode(syncStatusPin, INPUT);  // set it up as an input
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
  currentSyncedTime = timeOffset + micros();
}

// Method to read timestamp from central TimeServer over I2C
void syncTime(){
    //Serial.print("SYNCING...");
  Wire.begin();  // Initialize I2c  
  
  Wire.beginTransmission(0x50);  // begin communication with TimeServer
  String timeStr = String(micros());  // convert timestamp to string
  
  volatile int timeLength = timeStr.length();  // find out how many digits are in the current time
  // If not 10 digits (max digit length for unsigned long), add leading zeros
  // This is to keep the time of I2C transfers consistent between subsequent read/writes
  if(timeLength == 1){
    timeStr = "000000000" + timeStr;
  }
  else if(timeLength == 2){
    timeStr = "00000000" + timeStr;
  }
  else if(timeLength == 3){
    timeStr = "0000000" + timeStr;
  }
  else if(timeLength == 4){
    timeStr = "000000" + timeStr;
  }
  else if(timeLength == 5){
    timeStr = "00000" + timeStr;
  }
  else if(timeLength == 6){
    timeStr = "0000" + timeStr;
  }
  else if(timeLength == 7){
    timeStr = "000" + timeStr;
  }
  else if(timeLength == 8){
    timeStr = "00" + timeStr;
  }
  else if(timeLength == 9){
    timeStr = "0" + timeStr;
  }
  
  Wire.print(timeStr); // Send padded timestamp to TimeServer
  Wire.endTransmission(); // End comm.

  Wire.requestFrom(0x50, 10);  // Request 10 bytes (timestamp) from TimeServer
  int index = 0;  // index of char array
  while(Wire.available())   // while more bytes avail
  { 
    char c = Wire.read();    // receive a byte as character
    dataFromTimeServer[index++] = c;  // store byte in char array
    
  }
  timeOffset = atol(dataFromTimeServer);  // convert char array to unsigned long
  timeOfLastSync = millis();  // update time of last sync
}  
