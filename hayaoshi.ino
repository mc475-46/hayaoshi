#include <VarSpeedServo.h>
#include "BuzzerButton.h"

#define PORT_START 22
#define RESET_PORT 30
#define RESPONDENTS 4
#define SERVO_TEST_PORT 52

BuzzerButton* answering_respondent = NULL;
BuzzerButton buttons[RESPONDENTS];
byte frame_count;

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
			answering_respondent->reset_servo();
			answering_respondent = NULL;
		}
	}
}
