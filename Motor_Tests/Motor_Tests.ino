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

// the setup function runs once when you press reset or power the board
void setup()
{
  // initialize digital pin LED_BUILTIN as an output.
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

// the loop function runs over and over again forever
void loop() 
{
  while(1)
  {
    Read_Button();
      digitalWrite(step_mot1, HIGH);   
      delay(1);                      
      digitalWrite(step_mot1, LOW);    
      delay(1);        
  }
}
