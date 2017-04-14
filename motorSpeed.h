// motorSpeed.h

#ifndef _MOTORSPEED_h
#define _MOTORSPEED_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#define FORWARD 0
#define BACKWARD 1
#define STOP 2
#define BRAKE 3
#define STEP 1.07
class MOTOR {
public:
	MOTOR(uint8_t ENA, uint8_t IN1,uint8_t IN2);//���캯��
	void setSpeed(uint8_t speed, uint8_t state);//��ȡ��ǰ�ٶ�
private:
	uint8_t ena,in1,in2;
};

#endif

