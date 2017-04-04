#include <SR04.h>
#include "motorSpeed.h"
#define WARNING 30
#define ANG 57.32

extern Servo detect;
extern float location[2];
extern float destination[2];
extern float angle;
extern SR04 ultro;
extern MOTOR motor;
extern int sequence;
void avoid()
{
  /*避障
  1.获得超声波所发来障碍物信息
  2.综合所在位置进行避障*/
  /*if (ultro.Get() < WARNING)
  {
	  float direct;
	  int i = 0;
	  int rand[21] = {40,45,50,55,60,65,70,75,80,85,90,95,100,105,110,115,120,125,130,135,140};

	  motor.setSpeed(255, STOP);
	  detect.write(10);
	  while (ultro.Get() < WARNING)
	  {
		  //回避
		  if (i > 20)
		  {
			  i = 0;
			  delay(30);
		  }
			  
		  detect.write(rand[i++]);
		  delay(10);
	  }

	  direct = (170 - detect.read()) / ANG - angle;

	  destination[0] = location[0] + cos(direct)*WARNING;
	  destination[1] = location[1] - sin(direct)*WARNING;
	  sequence--;
  }*/
  
}

