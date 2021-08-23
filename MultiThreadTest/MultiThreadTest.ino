#include <Wire.h>
#include "MCP342x.h"
#include "SHT2x.h"
#include "TimedAction.h"


#define ENABLE_MOT 22
#define SLEEP_MOT 23

#define dir_mot1 24
#define step_mot1 25

#define dir_mot2 26
#define step_mot2 27

#define dir_mot3 28
#define step_mot3 29

#define dir_mot4 30
#define step_mot4 31

#define dir_mot5 32
#define step_mot5 33

#define USR_BUTTON 34

  int buttonState = 0;         // variable for reading the pushbutton status
  bool toggle = 0;
  bool latch = 1;

// 0x68 is the default address for all MCP342x devices
uint8_t address = 0x68;
MCP342x adc = MCP342x(address);

// Configuration settings
MCP342x::Config config(MCP342x::channel1, MCP342x::oneShot,
		       MCP342x::resolution18, MCP342x::gain1);

// Configuration/status read back from the ADC
MCP342x::Config status;

// Inidicate if a new conversion should be started
bool startConversion = false;

void Humiditytest(){
  Serial.print("Humidity(%RH): ");
  Serial.print(SHT2x.GetHumidity());
  Serial.print("     Temperature(C): ");
  Serial.println(SHT2x.GetTemperature());
}

void Temperaturetest(){
  long value = 0;
  uint8_t err;

  if (startConversion) {
    Serial.println("Convert");
    err = adc.convert(config);
    if (err) {
      Serial.print("Convert error: ");
      Serial.println(err);
    }
    startConversion = false;
  }
  
  err = adc.read(value, status);
  if (!err && status.isReady()) { 
    // For debugging purposes print the return value.
    Serial.print("Value: ");
    Serial.println(value);
    Serial.print("Config: 0x");
    Serial.println((int)config, HEX);
    Serial.print("Convert error: ");
    Serial.println(err);
    startConversion = true;
  }
    
}

TimedAction temperatureThread = TimedAction(6000,Temperaturetest);
TimedAction humidityThread = TimedAction(3000,Humiditytest);

void setup(void)
{
  Serial.begin(9600);
  Wire.begin();

  // Enable power for MCP342x (needed for FL100 shield only)
  pinMode(9, OUTPUT);
  digitalWrite(9, HIGH);
    
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
  pinMode(ENABLE_MOT, OUTPUT);
  pinMode(SLEEP_MOT, OUTPUT);
  digitalWrite(ENABLE_MOT, LOW);
  digitalWrite(SLEEP_MOT, HIGH);
  
  pinMode(dir_mot1, OUTPUT);
  pinMode(step_mot1, OUTPUT);
  digitalWrite(dir_mot1, HIGH);
  
  pinMode(dir_mot2, OUTPUT);
  pinMode(step_mot2, OUTPUT);
  digitalWrite(dir_mot2, HIGH);
  
  pinMode(dir_mot3, OUTPUT);
  pinMode(step_mot3, OUTPUT);
  digitalWrite(dir_mot3, HIGH);
  
  pinMode(dir_mot4, OUTPUT);
  pinMode(step_mot4, OUTPUT);
  digitalWrite(dir_mot4, HIGH);

  pinMode(dir_mot5, OUTPUT);
  pinMode(step_mot5, OUTPUT);
  digitalWrite(dir_mot5, HIGH);

  pinMode(USR_BUTTON, INPUT);
  }

  // First time loop() is called start a conversion
  startConversion = true;
}

void Read_Button()
{
    buttonState = digitalRead(USR_BUTTON);
      // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
    if (buttonState == HIGH && latch == 1)
    {
      delay(5);
      buttonState = digitalRead(USR_BUTTON);
      if(buttonState == HIGH)
      {
        if(toggle == 1) 
        {
          digitalWrite(dir_mot1, HIGH);
          toggle = 0;
        }
        else
        {
          digitalWrite(dir_mot1, LOW);
          toggle = 1;
        }
        latch = 0;
      }
    }
    else if (buttonState == LOW)
    {
      latch = 1;
    }
}

void loop(void) {

  temperatureThread.check();
  humidityThread.check();

  while(1)
  {
    Read_Button();
      digitalWrite(step_mot1, HIGH);   
      delay(1);                      
      digitalWrite(step_mot1, LOW);    
      delay(1);        
  }

  temperatureThread.check();
  humidityThread.check();

}