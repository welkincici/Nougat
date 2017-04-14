#include <Servo.h>
#include "motorSpeed.h"
#include "posture.c"
//#include "bump.c"
//#include "avoid.c"
//#include "grab.c"
//#include "push.c"
#define ANG 57.32
#define ARRIVE 50
#define CLOSE 50
#define NEAR 150
#define WARNING 30
#define UNIT 400
#define BEGIN 999999

#define AIM 7
#define PUSH1 8
#define PUSH2 9
#define DETECT 10
#define ROTATE 12
#define FETCH 11
#define SEIZE 13
#define TrigPin 26
#define EchoPin 28
#define PULSE 49

MOTOR motor(4, 22, 24);	//定义电机对象
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
	{ 800,3000 },{ 1800,2700 },{ 2300,2800 },
	{ 2500,2600 },{ 2400,1800 },
	{ 800,1200 },{ 1200,2400 },
	{ 1600,3200 },{ 1600,3600 }
};
//int line[2];
//bool driveFlag;
int sequence = 0;//第几个点
int destination[2] = { 1200,800 };
float location[2] = { 0,3200 };//初始坐标 
float dismodPre = BEGIN;

/*车头往右偏angle减小
舵机往右偏增大，中间角度为80,左右极限为45-120
超声波和舵机相反，中间角度为90
机械臂rotate中间70*/

void speedMonitor(void)		//测速中断函数
{
	location[0] += STEP*cosx;
	location[1] += STEP*sinx;
}
/*void countline(void)
{
	if (abs((int)location[1]%400-200) < abs((int)location[0] % 400 - 200))
		location[0] = (int)(location[0] / 400 + 0.5)*UNIT;
	else
		location[1] = (int)(location[1] / 400 + 0.5)*UNIT;
}*/

void setup() {

  Serial3.begin(9600);  
  Serial1.begin(38400);
  Serial.begin(9600);

  aim.attach(AIM);
  //detect.attach(DETECT);
  //push1.attach(PUSH1);
  //push2.attach(PUSH2);
  //rotate.attach(ROTATE);
  //fetch.attach(FETCH);
  //seize.attach(SEIZE);
 
  attachInterrupt(0, speedMonitor, RISING);//霍尔编码器A相，上升沿触发，2
  //attachInterrupt(1, countline, RISING);//3

  //pinMode(PULSE, INPUT);

  delay(500);
  int i;
  for (i = 0; i < 5; i++)
  {
	  delay(5);
	  posture();
	  ANGLE += angle;
  }
}

void loop() {
	//avoid();
	/*if(driveFlag==1)
		bump();*/
	posture();

	float distance[2];
	float dismod;
	float direct;
	int nextflag = 0;
	
	distance[0] = destination[0] - location[0];
	distance[1] = destination[1] - location[1];

	dismod = abs(distance[0]) + abs(distance[1]);

	if (dismod<ARRIVE)
	{
		switch (sequence)
		{
		case 0:case 1:case 2:case 3:case 4:case 5:case 6:case 7:case 8:
			//Serial3.println("GOOD");
			break;
		/*case 15:
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
			break;*/
		}

		nextflag = 1;
	}

	if (dismod < dismodPre)
	{
		dismodPre = dismod;
	}

	if (dismod - dismodPre > CLOSE&& dismod <400)
	{
		nextflag = 1;
		//Serial3.println("SKIP");
	}

	if (nextflag)
	{
		sequence++;
		destination[0] = ROUTINE[sequence][0];
		destination[1] = ROUTINE[sequence][1];
		distance[0] = destination[0] - location[0];
		distance[1] = destination[1] - location[1];
		dismodPre = BEGIN;
		//motor.setSpeed(255, STOP);
		//driveFlag = false;
		//delay(1000);
	}
	
	direct = -atan2(distance[1], distance[0])*ANG + angle*ANG + 80;

	/*Serial.print("sequence:");
	Serial.print(sequence);
	Serial.print("x");
	Serial.print(location[0]);
	Serial.print("y");
	Serial.println(location[1]);
	Serial.print("angle");
	Serial.println(angle);
	Serial.print("direct");
	Serial.println(direct);*/
	/*Serial3.print("sequence:");
	Serial3.print(sequence);
	Serial3.print("x");
	Serial3.print(location[0]);
	Serial3.print("y");
	Serial3.println(location[1]);
	Serial3.print("direct");
	Serial3.println(direct);*/

	if (direct > 260)
		direct = direct - 360;
	else if (direct < -100)
		direct = direct + 360;

	if (direct > 120)
		direct = 120;
	else if (direct < 45)
		direct = 45;

	if(dismod>NEAR)
		aim.write(direct);
	//detect.write(170 - direct);

	/*if (Serial3.read()=='g')
	{
		motor.setSpeed(255, FORWARD);
	}*/
	motor.setSpeed(255, FORWARD);

	//motor.setSpeed(255, FORWARD);
	//driveFlag = true;

	if (sequence > 0)
	{
		motor.setSpeed(255, STOP);
		//driveFlag = false;
		while (1)
		{
			delay(10000);
		}
	}
}
