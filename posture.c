#include <Wire.h>
#include <JY901.h>
float ANGLE;//赛场本身偏角
float angle,cosx, sinx;
//int flag;
//int b;
void posture();
void posture()
{
	while (Serial3.available())
	{
		JY901.CopeSerialData(Serial3.read()); //Call JY901 data cope function
		//flag = 1;
	}

	angle = (float)JY901.stcAngle.Angle[2] / 32768 * 3.14 - ANGLE;
	cosx = cos(angle);
	sinx = sin(angle);

	/*if (flag)
	{
		int a;
		a = millis();
		Serial3.println(a - b);
		b = a;
	}*/
}

