#include "BuzzerButton.h"
#define RESPONDENTS 4
#define CHATTERING_TIME 20
#define SERVO_ANGLE 85

extern BuzzerButton* answering_respondent;
extern BuzzerButton buttons[RESPONDENTS];

BuzzerButton::BuzzerButton(){}
BuzzerButton::BuzzerButton(byte i_pin, byte o_pin){
	servo.attach(o_pin);
	reset_servo();

	pinMode(i_pin, INPUT_PULLUP);
	input_pin = i_pin;

	is_pressed = false;
	pressed_time = 0;
	locked = false;
}

bool BuzzerButton::update(){
	if(digitalRead(input_pin) == HIGH){
		if(!locked){
			if(is_pressed && millis() - pressed_time > CHATTERING_TIME){
				answer();
				return true;
			}
			if(!is_pressed){
				is_pressed = true;
				pressed_time = millis();
			}
		}
	} else {
		reset();
	}
	return false;
}

inline void BuzzerButton::reset(){
	is_pressed = false;
	pressed_time = 0;
	locked = false;
}

byte BuzzerButton::answer(){
	answering_respondent = this;
	for(byte i = 0; i < RESPONDENTS; ++i){
		buttons[i].reset();
		buttons[i].locked = true;
	}

	this->servo.write(SERVO_ANGLE, 0, false);
	return 0;
}

void BuzzerButton::reset_servo(){
	servo.write(0, 32, false);
}
