#include <Servo.h>
#include "motorSpeed.h"
#include "posture.c"
//#include "avoid.c"
#include "grab.c"
#include "push.c"

#define ANG 57.32
#define CLOSE 500
#define WARNING 30
#define UNIT 400

#define AIM 3
#define PUSH1 4
#define PUSH2 5
#define DETECT 6
#define ROTATE 7
#define FETCH 10
#define SEIZE 9
#define TrigPin 26
#define EchoPin 28

MOTOR motor(8, 22, 24);	//定义电机对象
//SR04 ultro(TrigPin, EchoPin);
Servo aim;
Servo detect;
Servo push1;
Servo push2;
Servo rotate;
Servo fetch;
Servo seize;

//地图
const int ROUTINE[18][2] = {
	{ 700,3000 },{ 2300,2800 },{ 2500,2000 },
	{ 2400,1200 },{ 800,1200 },{ 400,2000 },
	{ 1200,2400 },{ 1600,3200 },{ 1600,3600 }
};
int line[2];
int sequence = 0;//第几个点
float location[2] = { 60,3200 };//初始坐标 
int destination[2] = { 700,3000 };
float dismodPre = 9999999;

/*车头往右偏angle减小
舵机往右偏增大，中间角度为80
超声波和舵机相反，中间角度为90*/

void speedMonitor(void)		//测速中断函数
{
	location[0] += STEP*cos(angle);
	location[1] += STEP*sin(angle);
}

void countline(void)
{
	if (abs(line[0] - location[0]) > abs(line[1] - location[0]))
		line[0] = (int)(location[0] / 400 + 0.5)*UNIT;
	else
		line[1] = (int)(location[1] / 400 + 0.5)*UNIT;
}

void setup() {

  Serial3.begin(9600);  
  Serial1.begin(38400);
  Serial.begin(9600);

  aim.attach(AIM);
  detect.attach(DETECT);
  push1.attach(PUSH1);
  push2.attach(PUSH2);
 
  attachInterrupt(0, speedMonitor, RISING);//霍尔编码器A相，上升沿触发，2
  //attachInterrupt(2, countline, RISING);//21

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

	//avoid();

	float distance[2];
	float dismod;
	float direct;
	int nextflag = 0;
	
	distance[0] = destination[0] - location[0];
	distance[1] = destination[1] - location[1];

	dismod = distance[0] * distance[0] + distance[1] * distance[1];

	if (dismod<CLOSE)
	{
		switch (sequence)
		{
		case 2:
			motor.setSpeed(255, STOP);
			while (1)
			{
				delay(10000);
			}
			break;
		case 15:
			//机械臂放置夹取
			motor.setSpeed(100, STOP);
			grab();
			break;
		case 7:case 11:
			//推木块
			motor.setSpeed(100, STOP);
			push();
			break;
		case 17:
			Serial.print("FINISH!!");
			motor.setSpeed(255, STOP);
			while (1)
			{
				delay(10000);
			}
			break;
		}

		nextflag = 1;
	}

	if (dismod - dismodPre > 500)
	{
		nextflag = 1;
	}

	if (nextflag)
	{
		
		sequence++;
		destination[0] = ROUTINE[sequence][0];
		destination[1] = ROUTINE[sequence][1];
		distance[0] = destination[0] - location[0];
		distance[1] = destination[1] - location[1];
		dismodPre = 9999999;
	}
	else
		dismodPre = dismod;
	
	direct = -atan(distance[1] / distance[0])*ANG + angle*ANG + 80;
	Serial3.print("direct:");
	Serial3.print(direct);
	if (direct > 110)
		direct = 110;
	else if (direct < 50)
		direct = 50;

	aim.write(direct);
	detect.write(170 - direct);

	if (Serial3.available())
	{
		motor.setSpeed(200, FORWARD);
	}

	Serial3.print("angle:");
	Serial3.println(angle*ANG);
	
	Serial3.print("x");
	Serial3.print(location[0]);
	Serial3.print("y");
	Serial3.println(location[1]);

	if (sequence > 3)
	{
		Serial3.println("boom");
		motor.setSpeed(255, STOP);
		while (1)
		{
			delay(10000);
		}
	}
}
