// motorSpeed.h

#ifndef _MOTORSPEED_h
#define _MOTORSPEED_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#define FORWARD 1
#define BACKWARD 3
#define STOP 0
#define BRAKE 2
#define STEP 0.64
class MOTOR {
public:
	MOTOR(uint8_t ENA, uint8_t IN1,uint8_t IN2);//构造函数
	void setSpeed(uint8_t speed, uint8_t state);//获取当前速度
private:
	uint8_t ena,in1,in2;
};

#endif

