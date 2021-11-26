#include <Wire.h>
#include "MCP342x.h"

// 0x68 is the default address for all MCP342x devices
uint8_t address = 0x68;
MCP342x adc = MCP342x(address);

// ADC Configuration settings
MCP342x::Config config1(MCP342x::channel1, MCP342x::oneShot,
           MCP342x::resolution18, MCP342x::gain1);
MCP342x::Config config2(MCP342x::channel2, MCP342x::oneShot,
           MCP342x::resolution18, MCP342x::gain1);

// Configuration/status read back from the ADC
MCP342x::Config status;

// Inidicate if a new conversion should be started
bool startConversion = false;


void setup(void)
{
  Serial.begin(9600);
  Wire.begin();

  Serial.println("Begun Serial communications");/****** Debug Messege please delete when not required ******/

  // Reset devices
  MCP342x::generalCallReset();
  delay(1); // MC342x needs 300us to settle

  // Check device present
  Wire.requestFrom(address, (uint8_t)1);
  if (!Wire.available()) {
    Serial.print("No device found at address ");
    Serial.println(address, HEX);
    while (1)
      ;
  }

  adc.configure(config1);

  // First time loop() is called start a conversion
  startConversion = true;
}

void loop(void)
{
  //Serial.println("ADC Test Channel 1");/****** Debug Messege please delete when not required ******/
  
  long value = 0;
  uint8_t err;

  if (startConversion) {
    //Serial.println("Convert");
    //adc.configure(config);
    err = adc.convert(config1);
    if (err) {
      Serial.print("Convert error: ");
      Serial.println(err);
    }
    startConversion = false;
  }

  delay(100); // wait for conversion to occur
  
  err = adc.read(value, status);
  if (!err && status.isReady()) { 
    // For debugging purposes print the return value.
    Serial.print("Light: ");
    Serial.println(value);
    //Serial.print("Config: 0x");
    //Serial.println((int)config1, HEX);
    //Serial.print("Convert error: ");
    //Serial.println(err);
    startConversion = true;
  }

  delay(1000);

/************************** Read channel 2 ***********************/
   // Serial.println("ADC Test Channel 2");/****** Debug Messege please delete when not required ******/
  
  value = 0;
  err = 0;

  if (startConversion) {
    //Serial.println("Convert");
    err = adc.convert(config2);
    if (err) {
      Serial.print("Convert error: ");
      Serial.println(err);
    }
    startConversion = false;
  }

  delay(100); // wait for conversion to occur
  
  err = adc.read(value, status);
  if (!err && status.isReady()) { 
    // For debugging purposes print the return value.
    Serial.print("Moisture: ");
    Serial.println(value);
    //Serial.print("Config: 0x");
    //Serial.println((int)config2, HEX);
    //Serial.print("Convert error: ");
   // Serial.println(err);
    startConversion = true;
  }
}
