void grab();

#define STAGE 45
#define LONG 0
#define MIDDLE 90
#define SHORT 180
#define TIGHT 0
#define LOOSE 180

extern Servo rotate;
extern Servo fetch;
extern Servo seize;

void grab()
{
	
	float direct;

	direct = STAGE - angle + 90;
	rotate.write(direct);

	fetch.write(MIDDLE);
	seize.write(LOOSE);
	fetch.write(LONG);
	seize.write(TIGHT);
	fetch.write(MIDDLE);
	seize.write(LOOSE);
	fetch.write(SHORT);
}

