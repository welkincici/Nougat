#include <Servo.h>
#include "motorSpeed.h"
#include "posture.c"
#define ANG 57.32
#define CLOSE 50
#define NEAR 150
#define BEGIN 999999

#define AIM 11
#define DETECT 10
#define COLLECT1 9
#define COLLECT2 8
#define DISK A15
#define MODE A12
#define START A11

MOTOR motor(12, 47, 46);	//定义电机对象
MOTOR paper(7, A14, A13);
Servo aim;
Servo collect1;
Servo collect2;

//地图
int (*ROUTINE)[2];
int routine1[18][2] = {
	{ 800,2800 },{ 1600,3800 },{ 1200,3200 },
	{ 2450,2800 },{ 2850,2000 },{ 800,1150 },{ 350,2000 },{ 1600,3800 },{ 1200,3200 },
	{ 2000,2800 },{ 2400,1200 },{ 1600,800 },{ 800,2000 },{ 1600,3800 },{ 1600,3800 }
};
int routine2[18][2] = {
	{ 800,2800 },{ 1600,3800 },{ 2400,3200 },
	{ 400,2000 },{ 800,1200 },{ 2800,2000 },{ 2400,2800 },{ 1600,3800 },{ 1200,3200 },
	{ 2000,2800 },{ 2400,1200 },{ 1600,800 },{ 800,2000 },{ 1600,3800 },{ 1600,3800 }
};
extern int driveFlag;
int diskFlag;
unsigned long recordClock, reverseClock;
int sequence = 0;//第几个点
int skipCount;
int destination[2] = { 800,2800 };
float location[2] = { 215,3250 };//初始坐标 
float dismodPre = BEGIN;

/*车头往右偏angle减小
舵机往右偏增大，中间角度为80,左右极限为45-115
超声波和舵机相反*/

/*void speedMonitor(void)		//测速中断函数
{
	//location[0] += STEP*cosx*driveFlag;
	//location[1] += STEP*sinx*driveFlag;
}

void countline(void)
{
	lineLocation[0] = location[0];
	lineLocation[1] = location[1];
	lineAngle = angle;
	lineFlag = 1;
}*/

void setup() {

  collect1.write(120);
  collect2.write(30);

  Serial2.begin(9600);  
  Serial3.begin(38400);
  Serial.begin(9600);

  aim.attach(AIM);
  collect1.attach(COLLECT1);
  collect2.attach(COLLECT2);

  pinMode(DISK, INPUT);
  pinMode(MODE, INPUT_PULLUP);
  pinMode(START, INPUT_PULLUP);

  diskFlag = digitalRead(DISK);

  if (digitalRead(MODE))
	  ROUTINE = routine2;
  else
	  ROUTINE = routine1;
  
  int i;
  for (i = 0; i < 5; i++)
  {
	  delay(5);
	  posture();
	  ANGLE += angle;
  }
}

void loop() {
	//paper.setSpeed(110, FORWARD);

	float distance[2];
	float dismod;
	float direct;
	float cosx, sinx,diskAngle;
	int disk;
	int skip;
	int reverseFlag = 0;

	posture();
	

	/*if (millis() - recordClock > 2000)
	{
		reverseFlag = 1;
		reverseClock = millis();
	}

	if (millis() - reverseClock > 1000)
	{
		reverseFlag = 0;
	}*/


	distance[0] = destination[0] - location[0];
	distance[1] = destination[1] - location[1];

	dismod = abs(distance[0]) + abs(distance[1]);

	switch (sequence)
	{
	default:
		skip = 0;
		break;
	}

	if (skipCount > skip || dismod < CLOSE)
	{
		sequence++;
		destination[0] = ROUTINE[sequence][0];
		destination[1] = ROUTINE[sequence][1];
		distance[0] = destination[0] - location[0];
		distance[1] = destination[1] - location[1];
		dismodPre = BEGIN;
		skipCount = 0;
	}
	else if (dismod < dismodPre)
	{
		dismodPre = dismod;
		skipCount = 0;
	}
	else if (dismod > dismodPre + CLOSE)
	{
		skipCount++;
		dismodPre = dismod;
	}

	direct = -atan2(distance[1], distance[0])*ANG + angle*ANG + 84;

	/*if (reverseFlag)
		direct = direct + 30;*/

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

	if (digitalRead(START))
	{
		switch (sequence)
		{
		case 0:
			if (millis() > 500)
			{
				/*collect1.write(20);
				collect2.write(150);*/
			}
			motor.setSpeed(255, reverseFlag ? BACKWARD : FORWARD);
			break;
		case 2:case 8:
			motor.setSpeed(255, reverseFlag ? FORWARD : BACKWARD);
			break;
		case 14:
			motor.setSpeed(255, STOP);
			while (1)
			{
				delay(500);
			}
			break;
		default:
			motor.setSpeed(255, reverseFlag ? BACKWARD : FORWARD);
			break;
		}
	}
	

	disk = digitalRead(DISK);
	if (disk ^ diskFlag)
	{
		diskAngle = angle + 0.8 * ((float)84 - direct) / ANG;
		cosx = cos(diskAngle);
		sinx = sin(diskAngle);
		location[0] += fSTEP*cosx*driveFlag;
		location[1] += fSTEP*sinx*driveFlag;
		diskFlag = disk;
		recordClock = millis();
	}
	Serial2.print(location[0]);
	Serial2.print(" ");
	Serial2.println(location[1]);

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