#include <Wire.h>

char dataFromTimeServer[10]; 
unsigned long timeFromTimeServer;

void setup() {
  Serial.begin(9600);
  syncTime(); // Sync time with TimeServer
}

void loop() {
 
}

void syncTime(){
  Wire.begin();  // Initialize I2c
  Wire.requestFrom(0x50, 10);  // Request 10 
  int index = 0;
  while(Wire.available())    
  { 
    char c = Wire.read();    // receive a byte as character
    dataFromTimeServer[index++] = c;
    
  }
  timeFromTimeServer = atol(dataFromTimeServer);
  Serial.println(timeFromTimeServer);
} 
