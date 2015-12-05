#include <stdbool.h>
#include <VarSpeedServo.h>
#define CHATTERING_TIME 20
#define PORT_START 22
#define RESET_PORT 30
#define RESPONDENTS 4
#define SERVO_TEST_PORT 52
#define SERVO_ANGLE 85

typedef unsigned long m_secs;

class BuzzerButton;
byte answer(BuzzerButton*);

class BuzzerButton{
private:
  byte input_pin;
  bool is_pressed;
  m_secs pressed_time;

public:
  byte output_pin;

  BuzzerButton(){}

  BuzzerButton(byte o_pin, byte i_pin){
    pinMode(o_pin, OUTPUT);
    pinMode(i_pin, INPUT_PULLUP);
    output_pin = o_pin;
    input_pin = i_pin;
    is_pressed = false;
    pressed_time = 0;
  }

  bool update(){
    if(digitalRead(input_pin) == LOW){
      if(is_pressed && millis() - pressed_time > CHATTERING_TIME){
          answer(this);
          return true;
      }
      if(!is_pressed){  
        is_pressed = true;
        pressed_time = millis();
      }
    } else {
      reset();
    }
    return false;
  }  

  inline void reset(){
    is_pressed = false;
    pressed_time = 0;
  }
};

BuzzerButton* answering_respondent = NULL;
BuzzerButton buttons[RESPONDENTS];
byte frame_count;
VarSpeedServo servo;

byte answer(BuzzerButton* bb){
  answering_respondent = bb;
  for(byte i = 0; i < RESPONDENTS; ++i){
    buttons[i].reset();
  }

  servo.write(SERVO_ANGLE, 0, false);

  return 0;
}


void setup() {
  pinMode(RESET_PORT, INPUT_PULLUP);
  servo.attach(52);
  servo.write(0, 64, false);
  
  for(byte i=0; i<RESPONDENTS; ++i){
    buttons[i] = BuzzerButton(PORT_START + i, PORT_START + RESPONDENTS + i);
  }
  frame_count = 0;
}

void loop() {
  if(!answering_respondent){
    for(byte i = frame_count; i < RESPONDENTS + frame_count; ++i){
      if(buttons[i % RESPONDENTS].update()) break;
    }
    frame_count = (frame_count + 1) % RESPONDENTS;
  } else {
    digitalWrite(answering_respondent->output_pin, HIGH);
  }
  if(digitalRead(RESET_PORT) == LOW){
    digitalWrite(answering_respondent->output_pin, LOW);
    answering_respondent = NULL;
    servo.write(0, 32, false);
  }
}
