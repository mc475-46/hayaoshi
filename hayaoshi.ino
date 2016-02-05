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
	VarSpeedServo servo;
	bool is_locked;

	BuzzerButton(){}

	BuzzerButton(byte o_pin, byte i_pin){
		servo.attach(o_pin);
		servo.write(0, 64, false);

		pinMode(i_pin, INPUT);
		input_pin = i_pin;

		is_pressed = false;
		pressed_time = 0;
		locked = false;
	}

	bool update(){
		if(digitalRead(input_pin) == HIGH){
			if(!locked){
				if(is_pressed && millis() - pressed_time > CHATTERING_TIME){
						answer(this);
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

	inline void reset(){
		is_pressed = false;
		pressed_time = 0;
		locked = false;
	}
};

BuzzerButton* answering_respondent = NULL;
BuzzerButton buttons[RESPONDENTS];
byte frame_count;

byte answer(BuzzerButton* bb){
	answering_respondent = bb;
	for(byte i = 0; i < RESPONDENTS; ++i){
		buttons[i].reset();
		buttons[i].locked = true;
	}

	bb->servo.write(SERVO_ANGLE, 0, false);
	return 0;
}


void setup() {
	pinMode(RESET_PORT, INPUT_PULLUP);
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
	}
	if(digitalRead(RESET_PORT) == LOW){
		if(answering_respondent){
			answering_respondent->servo.write(0, 32, false);
			answering_respondent = NULL;
		}
	}
}
