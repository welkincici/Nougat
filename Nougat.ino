#include <Servo.h>
#include "motorSpeed.h"
#include "posture.c"
#define ANG 57.32
#define CLOSE 50
#define NEAR 150
#define UNIT 400
#define BEGIN 999999

#define AIM 11
#define DETECT 10
#define COLLECT1 9
#define COLLECT2 8
#define PULSE 49

MOTOR motor(12, 47, 46);	//定义电机对象
Servo aim;
Servo detect;
Servo collect1;
Servo collect2;

//地图
const int ROUTINE[18][2] = {
	{ 600,3000 },{ 2400,2800 },
	{ 3100,2400 } ,{ 2900,1000 },
	{ 1000,1200 },{ 1200,2400 },
	{ 2000,4000 }
};
//unsigned long myclock;
extern int driveFlag;
int lineFlag;
float lineLocation[2], lineAngle;
int sequence = 0;//第几个点
int destination[2] = { 600,3000 };
float location[2] = { 20,3200 };//初始坐标 
float dismodPre = BEGIN;
float locationSample[2];

/*车头往右偏angle减小
舵机往右偏增大，中间角度为80,左右极限为45-115
超声波和舵机相反*/

void speedMonitor(void)		//测速中断函数
{
	location[0] += STEP*cosx*driveFlag;
	location[1] += STEP*sinx*driveFlag;
}

void countline(void)
{
	lineLocation[0] = location[0];
	lineLocation[1] = location[1];
	lineAngle = angle;
	lineFlag = 1;
}

void setup() {

  Serial2.begin(9600);  
  Serial3.begin(38400);
  Serial.begin(9600);

  aim.attach(AIM);
  detect.attach(DETECT);
  collect1.attach(COLLECT1);
  collect2.attach(COLLECT2);
 
  attachInterrupt(0, speedMonitor, RISING);//霍尔编码器A相，上升沿触发，2
  attachInterrupt(1, countline, RISING);//3

  collect1.write(140);
  collect2.write(30);

  int i;
  for (i = 0; i < 5; i++)
  {
	  delay(5);
	  posture();
	  ANGLE += angle;
  }
}

void loop() {

	posture();

	if (lineFlag == 1)
	{
		if (abs((int)lineLocation[1] % 400 - 200) < abs((int)lineLocation[0] % 400 - 200))
		{
			location[0] += (int)(lineLocation[0] / 400 + 0.5)*UNIT - lineLocation[0];
		}
		else
		{
			location[1] += (int)(lineLocation[1] / 400 + 0.5)*UNIT - lineLocation[1];
		}
		lineFlag = 0;
	}
	
	float distance[2];
	float dismod;
	float direct;
	
	distance[0] = destination[0] - location[0];
	distance[1] = destination[1] - location[1];

	dismod = abs(distance[0]) + abs(distance[1]);

	if (dismod < dismodPre)
	{
		dismodPre = dismod;
	}
    else if (dismod<CLOSE ||(dismod - dismodPre > CLOSE && dismod <400))
	{
		sequence++;
		destination[0] = ROUTINE[sequence][0];
		destination[1] = ROUTINE[sequence][1];
		distance[0] = destination[0] - location[0];
		distance[1] = destination[1] - location[1];
		dismodPre = BEGIN;
	}
	
	direct = -atan2(distance[1], distance[0])*ANG + angle*ANG + 80;
	
	if (direct > 260)
		direct = direct - 360;
	else if (direct < -100)
		direct = direct + 360;

	if (direct > 115)
		direct = 115;
	else if (direct < 45)
		direct = 45;

	if (dismod > NEAR)
	{
		aim.write(direct);
	}
	detect.write(170 - direct);
	
	if (!millis() % 1000)
	{
		locationSample[0] = location[0];
		locationSample[1] = location[1];
	}

	if (driveFlag && abs(locationSample[0] - location[0]) < 50 && abs(locationSample[1] - location[1])<50)
	{
		motor.setSpeed(255, 2 - driveFlag);
	}
	else
		switch (sequence)
		{
		case 0:
			if (millis() > 500)
			{
				collect1.write(25);
				collect2.write(145);
			}
			motor.setSpeed(255, FORWARD);
			break;
		case 2:
			motor.setSpeed(255, STOP);
			while (1)
			{
				delay(500);
			}
			break;
		default:
			motor.setSpeed(255, FORWARD);
			break;
		}

	/*while (Serial3.available())
	{
	switch (Serial3.read())
	{
	case 'f':
	motor.setSpeed(200, FORWARD);
	aim.write(80);
	break;
	case 'l':
	motor.setSpeed(200, FORWARD);
	aim.write(60);
	break;
	case 'r':
	motor.setSpeed(200, FORWARD);
	aim.write(100);
	break;
	case 'b':
	motor.setSpeed(200, BACKWARD);
	default:
	break;
	}
	myclock = millis();
	}

	if (millis() - myclock > 200)
	{
	motor.setSpeed(200, STOP);
	}*/
}
