// 
// 
// 

#include "motorSpeed.h"
/*uint16_t pulseCount;
void speedMonitor(void)
{
	pulseCount++;
}*/

MOTOR::MOTOR(uint8_t ENA, uint8_t IN1, uint8_t IN2)//¹¹Ôìº¯Êý
{
	ena = ENA;
	in1 = IN1;
	in2 = IN2;
	pinMode(ena, OUTPUT);
	pinMode(in1, OUTPUT);
	pinMode(in2, OUTPUT);
}

/*uint16_t MOTOR::getPos()
{
	return STEP*pulseCount;
}*/

void MOTOR::setSpeed(uint8_t speed, uint8_t state)
{
	switch (state)
	{
	case FORWARD:
		digitalWrite(in1, LOW);
		digitalWrite(in2, HIGH);
		break;
	case BACKWARD:
		digitalWrite(in2, LOW);
		digitalWrite(in1, HIGH);
		break;
	case STOP:
		digitalWrite(in2, HIGH);
		digitalWrite(in1, HIGH);
		break;
	case BRAKE:analogWrite(ena, LOW);
		break;
	default:
		break;
	}
	analogWrite(ena, speed);
}

