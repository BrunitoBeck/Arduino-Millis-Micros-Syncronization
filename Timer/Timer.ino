void setup() {
  syncTime(); // Sync time with TimeServer
}

void loop() {
 
}

void syncTime(){
  Wire.begin();  // Initialize I2c
  
  Wire.requestFrom(0x50, 10);  // Request 10 
  
  
} 
