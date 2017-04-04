void push();

extern Servo push1;
extern Servo push2;

#define OUT1 75
#define OUT2 100
#define IN1 140
#define IN2 30

void push()
{
	push1.write(OUT1);
	push2.write(OUT2);
	
	delay(2000);

	push1.write(IN1);
	push2.write(IN2);
	delay(2000);
}