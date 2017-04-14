#include <Wire.h>
#include <JY901.h>
float ANGLE;//赛场本身偏角
float angle,cosx, sinx;
//int b;
void posture();
void posture()
{
	while (Serial1.available())
	{
		JY901.CopeSerialData(Serial1.read()); //Call JY901 data cope function
	}

	angle = (float)JY901.stcAngle.Angle[2] / 32768 * 3.14 - ANGLE;
	cosx = cos(angle);
	sinx = sin(angle);
	/*int a;
	a = millis();
	Serial.println(a - b);
	b = a;*/
}

