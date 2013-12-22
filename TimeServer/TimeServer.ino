void setup()
{
  Wire.begin(0x50); // Setup I2C slave address as 0x50     
  Wire.onRequest(requestEvent); // Register event
}

void loop() {
 // Do nothing
}

void requestEvent()
{
  Wire.write(micros());
}
