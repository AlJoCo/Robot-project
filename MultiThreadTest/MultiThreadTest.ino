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

/****** Global Variables ******/
int buttonState = 0;  // variable for reading the pushbutton status
bool toggle = 0;
bool latch = 1;

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

/****** Function: Get Humidity Measurement from SHT21 ******/

void Humiditytest()
{
  Serial.println("Humidity Test");/****** Debug Messege please delete when not required ******/

  Serial.print("Humidity(%RH): ");
  Serial.print(SHT2x.GetHumidity());
  Serial.print("\nTemperature(C): ");
  Serial.println(SHT2x.GetTemperature());
}

/****** Function: Get Temperature Measurement from SHT21 ******/

void ADCRead()
{
  //Serial.println("ADC Test Channel 1");/****** Debug Messege please delete when not required ******/

  long value = 0;
  uint8_t err;

  //  if (startConversion) {
  //    //Serial.println("Convert");
  //    err = adc.convert(adc.channel2, adc.oneShot,
  //           adc.resolution18, adc.gain1);
  //    if (err) {
  //      Serial.print("Convert error: ");
  //      Serial.println(err);
  //    }
  //    startConversion = false;
  //  }
  //
  //  delay(10); // wait for conversion to occur
  //
  //  err = adc.read(value, status);
  //  if (!err && status.isReady()) {
  //    // For debugging purposes print the return value.
  //    Serial.print("Light: ");
  //    Serial.println(value);
  //    Serial.print("Config: 0x");
  //    Serial.println((int)config1, HEX);
  //    Serial.print("Convert error: ");
  //    Serial.println(err);
  //    startConversion = true;
  //  }
  //  else
  //  {
  //    Serial.println("data not ready1");
  //  }
  //*************************** Convert and read Tests ****************************/

  err = adc.convertAndRead(adc.channel1, adc.oneShot, adc.resolution18, adc.gain2, 1000, value, status);
  if (!err && status.isReady()) {
    // For debugging purposes print the return value.
    Serial.print("Light: ");
    Serial.println(value);
    Serial.print("Config: 0x");
    Serial.println((int)config1, HEX);
    Serial.print("Convert error: ");
    Serial.println(err);
    startConversion = true;
  }
  else
  {
    Serial.println("data not ready1");
  }

  delay(100);

  err = adc.convertAndRead(adc.channel2, adc.oneShot, adc.resolution18, adc.gain1, 1000, value, status);
  if (!err && status.isReady()) {
    // For debugging purposes print the return value.
    Serial.print("Moisture: ");
    Serial.println(value);
    Serial.print("Config: 0x");
    Serial.println((int)config1, HEX);
    Serial.print("Convert error: ");
    Serial.println(err);
    startConversion = true;
  }
  else
  {
    Serial.println("data not ready1");
  }

  ///************************** Read channel 2 ***********************/
  //   // Serial.println("ADC Test Channel 2");/****** Debug Messege please delete when not required ******/
  //
  //  value = 0;
  //  err = 0;
  //
  //  if (startConversion) {
  //    //Serial.println("Convert");
  //    err = adc.convert(adc.channel2, adc.oneShot,
  //           adc.resolution18, adc.gain1);
  //    if (err) {
  //      Serial.print("Convert error: ");
  //      Serial.println(err);
  //    }
  //    startConversion = false;
  //  }
  //
  //  delay(10); // wait for conversion to occur
  //
  //  err = adc.read(value, status);
  //  if (!err && status.isReady()) {
  //    // For debugging purposes print the return value.
  //    Serial.print("Moisture: ");
  //    Serial.println(value);
  //    Serial.print("Config: 0x");
  //    Serial.println((int)config2, HEX);
  //    Serial.print("Convert error: ");
  //    Serial.println(err);
  //    startConversion = true;
  //  }
  //  else
  //  {
  //    Serial.println("data not ready2");
  //  }
}

/****** Function: Read Button Value include debounce ******/

bool Read_Button()
{
  buttonState = digitalRead(USR_BUTTON);

  if (buttonState == HIGH)
  {
    delay(5);
    buttonState = digitalRead(USR_BUTTON);
    if (buttonState == HIGH)
    {
      //Serial.println("Button HIGH");/****** Debug Messege please delete when not required ******/
      return HIGH;
    }
  }
  //Serial.println("Button LOW");/****** Debug Messege please delete when not required ******/
  return LOW;
}

/****** Threading Setup ******/

TimedAction ADCThread = TimedAction(1000, ADCRead);
TimedAction humidityThread = TimedAction(1000, Humiditytest);

/****** Arduino Setup Function ******/

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
  if (!Wire.available())
  {
    Serial.print("No device found at address ");
    Serial.println(address, HEX);
    while (1)
    {
      ;
    }
  }

  adc.configure(config2);

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

  // First time loop() is called start a conversion
  startConversion = true;
}

/****** Main Loop ******/

void loop(void) {

  ADCThread.check();
  delay(400);
  humidityThread.check();

  if (Read_Button())
  {
    digitalWrite(step_mot1, HIGH);
    delay(1);
    digitalWrite(step_mot1, LOW);
    delay(1);
  }
}
