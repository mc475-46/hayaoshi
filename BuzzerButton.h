#include <Arduino.h>
#include <VarSpeedServo.h>
typedef unsigned long m_secs;

class BuzzerButton{
private:
	byte input_pin;
	bool is_pressed;
	m_secs pressed_time;
	VarSpeedServo servo;
	bool locked;

	byte answer();
	inline void reset();
public:
	BuzzerButton();
	BuzzerButton(byte,byte);

	bool update();
	void reset_servo();
};

