#include "main.h"
#include "RobotLib.h"
#include <math.h>

// Begin PID Loop Stuff
#define Kp 1.3 // proportional constant
#define Ki 0.0//0.00000005//0.00000000008 // integral constant
#define Kd 0.0//120.0 // derivative constant
// for the goTo pid
#define Kp2 0.3
#define Ki2 0.000002
#define Kd2 0.0
// for arm pid
#define Kp3 0.75
#define Ki3 0.0
#define Kd3 0.0

#define diameter 4.0 // wheel diameter
#define pi 3.141592653589793238462643383279502884197169 // should be good enough ;)

Encoder encoder;
Gyro gyro;

float error = 0; // error accumulation
float lastError = 0; // previous error
long lastTime = 0; // previous time
// for the goTo pid
float error2 = 0;
float lastError2 = 0;
long lastTime2 = 0;
// for arm pid
float error3 = 0;
float lastError3 = 0;
float lastTime3 = 0;

float derivative = 0;
float derivative2 = 0;
float derivative3 = 0;

int pid(float measurement, float target) {
	long time = micros();
	long elapsedTime = time - lastTime;
	float thisError = measurement - target;
	error += thisError * elapsedTime;
	float dError = (thisError - lastError) / elapsedTime;

	lastTime = time;
	lastError = thisError;

	float proportional = thisError * Kp;
	derivative = dError * Kd;
	if ((abs(proportional) > 20) || (derivative > 0.2)) {
		error = 0;
	}
	float integral = error * Ki;
	return -(proportional + integral + derivative);
}

int pid2(float measurement, float target) {
	long time = micros();
	long elapsedTime = time - lastTime2;
	float thisError = measurement - target;
	error2 += thisError * elapsedTime;
	float dError = (thisError - lastError2) / elapsedTime;

	lastTime2 = time;
	lastError2 = thisError;

	float proportional = thisError * Kp2;
	derivative2 = dError * Kd2;
	if ((abs(proportional) > 20) || (derivative2 > 0.2)) {
		error2 = 0;
	}
	float integral = error2 * Ki2;
	return -(proportional + integral + derivative2);
}

int pid3(float measurement, float target) {
	long time = micros();
	long elapsedTime = time - lastTime3;
	float thisError = measurement - target;
	error3 += thisError * elapsedTime;
	float dError = (thisError - lastError3) / elapsedTime;

	lastTime3 = time;
	lastError3 = thisError;

	float proportional = thisError * Kp3;
	derivative3 = dError * Kd3;
	if ((abs(proportional) > 20) || (derivative3 > 0.2)) {
		error3 = 0;
	}
	float integral = error3 * Ki3;
	return -(proportional + integral + derivative3);
}

void armTo(float value)
{
	int motorPower = pid3(analogRead(2), value);
	bool done = false;
	long triggerTime = 0;
	while (!done)
	{
		if (joystickGetDigital(1, 8, JOY_LEFT))
		{
			done = true;
		}
		printf("Pot: %d		Motor: %d\n\r", analogRead(2), motorGet(LF));
		if (triggerTime == 0)
		{
			if (motorPower < 10)
			{
				triggerTime = millis();
			}
		}
		else
		{
			if (motorPower < 10)
			{
				if (millis() - triggerTime > 1000) // must hold for 2 seconds
				{
					done = true;
				}
			}
			else
			{
				triggerTime = 0;
			}
		}

		motorSet(LB, -motorPower);
		motorSet(LF, -motorPower);
		motorSet(RB, motorPower);
		motorSet(RF, motorPower);

		motorPower = pid3(analogRead(2), value);
	}
	motorStop(LB);
	motorStop(LF);
	motorStop(RB);
	motorStop(RF);
}

void goTo(float inches)
{
	float degrees = (360 * inches) / (diameter * pi);
	int motorPower = pid2(-encoderGet(encoder), degrees);
	bool done = false;
	long triggerTime = 0;
	while (!done)
	{
		if (joystickGetDigital(1, 8, JOY_LEFT))
		{
			done = true;
		}
		printf("Encoder: %d		Motor: %d\n\r", encoderGet(encoder), motorGet(FR));
		if (triggerTime == 0)
		{
			if (motorPower < 10)
			{
				triggerTime = millis();
			}
		}
		else
		{
			if (motorPower < 10)
			{
				if (millis() - triggerTime > 1000) // must hold for 2 seconds
				{
					done = true;
				}
			}
			else
			{
				triggerTime = 0;
			}
		}


		motorSet(BL, motorPower);
		motorSet(ML, -motorPower);
		motorSet(FL, motorPower);
		motorSet(BR, -motorPower);
		motorSet(MR, motorPower);
		motorSet(FR, -motorPower);

		motorPower = pid2(-encoderGet(encoder), degrees);
	}
	motorStop(BL);
	motorStop(ML);
	motorStop(FL);
	motorStop(BR);
	motorStop(MR);
	motorStop(FR);
}

void turnTo(int degrees)
{
	int motorPower = pid(gyroGet(gyro), degrees) * (1.0 / 2.0);
	bool done = false;
	long triggerTime = 0;
	while (!done)
	{
		if (joystickGetDigital(1, 8, JOY_LEFT))
		{
			done = true;
		}
		printf("Gyro: %d	Motor: %d\n\r", gyroGet(gyro), motorGet(FR));
		if (triggerTime == 0)
		{
			if (motorPower < 10)
			{
				triggerTime = millis();
			}
		}
		else
		{
			if (motorPower < 10)
			{
				if (millis() - triggerTime > 1000) // must hold for 2 seconds
				{
					done = true;
				}
			}
			else
			{
				triggerTime = 0;
			}
		}


		motorSet(BL, -motorPower);
		motorSet(ML, motorPower);
		motorSet(FL, -motorPower);
		motorSet(BR, -motorPower);
		motorSet(MR, motorPower);
		motorSet(FR, -motorPower);

		motorPower = pid(gyroGet(gyro), degrees);
	}
	motorStop(BL);
	motorStop(ML);
	motorStop(FL);
	motorStop(BR);
	motorStop(MR);
	motorStop(FR);
}
// END PID LOOP STUFF

//TODO: LCD, IME, Movement,

/*IME*/
int eLeft,eRight;

/*void imeReinitialize(){
		imeShutdown();
		imeReset(eLeft);
		imeReset(eRight);
		imeInitializeAll();
	}*/

int imeRead(int ime){
	if(ime==IME_LEFT){
		imeGet(IME_LEFT,&eLeft);
		return eLeft;
	}else if(ime==IME_RIGHT){
		imeGet(IME_RIGHT,&eRight);
		eRight=-eRight;
		return eRight;
	}
	return 0;
}

/*TASK MANAGEMENT*/
TaskHandle createAsyncTask(void code()){
	TaskHandle t = taskCreate(code, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT);
	return t;
}

void pauseTaskUntil(TaskHandle t, bool b){
	void f(){
		taskSuspend(t);
		while(!b);
		taskResume(t);
	};
	createAsyncTask(f);
}

void taskKill(TaskHandle task){
	taskSuspend(task);
	taskDelete(task);
}

/*MOVEMENT*/
//DRIVE
void move(int dir, int t){
	if(dir==FORWARD){
		setMotor(FL,127);
		setMotor(ML,-127);
		setMotor(BL,-127);
		setMotor(FR,-127);
		setMotor(MR,127);
		setMotor(BR,127);
	}else if(dir==BACKWARD){
		setMotor(FL,-127);
		setMotor(ML,127);
		setMotor(BL,127);
		setMotor(FR,127);
		setMotor(MR,-127);
		setMotor(BR,-127);
	}else if(dir==LEFT){
		setMotor(FL,127);
		setMotor(ML,0);
		setMotor(BL,127);
		setMotor(FR,127);
		setMotor(MR,0);
		setMotor(BR,127);
	}else if(dir==RIGHT){
		setMotor(FL,-127);
		setMotor(ML,0);
		setMotor(BL,-127);
		setMotor(FR,-127);
		setMotor(MR,0);
		setMotor(BR,-127);
	}else if(dir==LEFT_TURN){
		setMotor(FL,127);
		setMotor(ML,-127);
		setMotor(BL,-127);
		setMotor(FR,-127);
		setMotor(MR,127);
		setMotor(BR,127);
	}else if(dir==RIGHT_TURN){
		setMotor(FL,127);
		setMotor(ML,-127);
		setMotor(BL,-127);
		setMotor(FR,-127);
		setMotor(MR,127);
		setMotor(BR,127);
	}
	delay(t);
	stop();
}

void smove(int dir, int t, int s){
	if(dir==FORWARD){
		setMotor(FL,s);
		setMotor(ML,-s);
		setMotor(BL,-s);
		setMotor(FR,-s);
		setMotor(MR,s);
		setMotor(BR,s);
	}else if(dir==BACKWARD){
		setMotor(FL,-s);
		setMotor(ML,s);
		setMotor(BL,s);
		setMotor(FR,s);
		setMotor(MR,-s);
		setMotor(BR,-s);
	}else if(dir==LEFT){
		setMotor(FL,s);
		setMotor(ML,0);
		setMotor(BL,s);
		setMotor(FR,s);
		setMotor(MR,0);
		setMotor(BR,s);
	}else if(dir==RIGHT){
		setMotor(FL,-s);
		setMotor(ML,0);
		setMotor(BL,-s);
		setMotor(FR,-s);
		setMotor(MR,0);
		setMotor(BR,-s);
	}else if(dir==LEFT_TURN){
		setMotor(FL,s);
		setMotor(ML,-s);
		setMotor(BL,-s);
		setMotor(FR,-s);
		setMotor(MR,s);
		setMotor(BR,s);
	}else if(dir==RIGHT_TURN){
		setMotor(FL,s);
		setMotor(ML,-s);
		setMotor(BL,-s);
		setMotor(FR,-s);
		setMotor(MR,s);
		setMotor(BR,s);
	}
	delay(t);
	stop();
}

//LIFT
int liftSpeed = 127;
int liftDir;

void setLiftSpeed(int speed){
	liftSpeed = speed;
}

int getLiftSpeed(){
	return liftSpeed;
}

void lift(int dir){
	if(dir>0){
		motorSet(LB,127);
		motorSet(LF,127);
		motorSet(RB,-127);
		motorSet(RF,-127);
	}else if(dir<0){
		motorSet(LB,-127);
		motorSet(LF,-127);
		motorSet(RB,127);
		motorSet(RF,127);
	}else{
		motorSet(LF,0);
		motorSet(LB,0);
		motorSet(RF,0);
		motorSet(RB,0);
	}
}

//Discarded correctional code:
void liftLevel(){
	int leftPot = abs(analogReadCalibrated(POT_LEFT));
	int rightPot = abs(analogReadCalibrated(POT_RIGHT));
	if(abs(leftPot-rightPot)>LIFT_MAXTILT){
		if(leftPot>rightPot){
			if(liftDir>0){
				//raise right
				motorSet(RB,-60);
				motorSet(RF,-60);
			}else{
				//lower left
				motorSet(LB,-60);
				motorSet(LF,-60);
			}
		}else{
			if(liftDir>0){
				//raise left
				motorSet(LB,60);
				motorSet(LF,60);
			}else{
				//lower right
				motorSet(RB,60);
				motorSet(RF,60);
			}
		}
	}else{
		lift(STOP);
	}
}

void stop(){
	motorStop(FL);
	motorStop(ML);
	motorStop(BL);
	motorStop(FR);
	motorStop(MR);
	motorStop(BR);
}

void stopLift(){
	motorSet(LF,0);
	motorSet(LB,0);
	motorSet(RF,0);
	motorSet(RB,0);
}


void setMotor(unsigned char channel, int speed){
	int oldSpeed = motorGet(channel); //get current speed
	motorSet(channel,(oldSpeed+((speed-oldSpeed)/MAX_SPEED_DIF)-((speed-oldSpeed)%MAX_SPEED_DIF)));
}

/*File IO*/
void fsend(char fileName[], char content[]){
	FILE* f = fopen(fileName,"w");
	fprint(content,f);
	fclose(f);
	printf("written");
}

char fgetChar(char fileName[], int pos){
	FILE* f = fopen(fileName,"r");
	char c = '?';
	for(int i=0; i<=pos;i++)
		c = fgetc(f);
	fclose(f);
	printf("written");
	return c;
}

/*TeleOp*/
int RA_X,RA_Y,LA_X,LA_Y;

void driveMap(int stick){
	RA_X = joystickGetAnalog(stick, 1);
	RA_Y = joystickGetAnalog(stick, 2);
	LA_Y = joystickGetAnalog(stick, 3);
	LA_X = joystickGetAnalog(stick, 4);

	if(joystickGetDigital(1,5,JOY_UP)){
		RA_X/=2;
		RA_Y/=2;
		LA_X/=2;
		LA_Y/=2;
	}else{
		if (abs(LA_Y) < ANALOG_THRESH) {
			LA_Y = 0;
		}
		if (abs(LA_X) < ANALOG_THRESH*1.5) {
			LA_X = 0;
		}
		if (abs(RA_Y) < ANALOG_THRESH) {
			RA_Y = 0;
		}
		if (abs(RA_X) < ANALOG_THRESH) {
			RA_X = 0;
		}

		if (abs(LA_Y) > abs(RA_Y)) {
			RA_Y = 0;
		} else {
			LA_Y = 0;
		}
	}
	motorSet(BL, (LA_Y + RA_Y + RA_X - LA_X));
	motorSet(ML, -(LA_Y + RA_Y + RA_X));
	motorSet(FL, (LA_Y + RA_Y + LA_X + RA_X));
	motorSet(BR, -(LA_Y + RA_Y - RA_X + LA_X));
	motorSet(MR, (LA_Y + RA_Y - RA_X));
	motorSet(FR, -(LA_Y + RA_Y - LA_X - RA_X));
}

void liftMap(int stick){
	//		if(joystickGetDigital(stick,6,JOY_DOWN)&&joystickGetDigital(stick,6,JOY_UP)){
	//			analogCalibrate(1);
	//			analogCalibrate(2);
	//
	//		}

	if(joystickGetDigital(stick,7,JOY_UP)){
		motorSet(LB,50);
		motorSet(LF,50);
	}else if(joystickGetDigital(stick,7,JOY_DOWN)){
		motorSet(LB,-50);
		motorSet(LF,-50);
	}else if(joystickGetDigital(stick,8,JOY_UP)){
		motorSet(RB,-50);
		motorSet(RF,-50);
	}else if(joystickGetDigital(stick,8,JOY_DOWN)){
		motorSet(RB,50);
		motorSet(RF,50);
	}else if(joystickGetDigital(stick,5,JOY_DOWN)){
		motorSet(LB,joystickGetAnalog(stick,3));
		motorSet(RB,-joystickGetAnalog(stick,2));
		motorSet(LF,joystickGetAnalog(stick,3));
		motorSet(RF,-joystickGetAnalog(stick,2));
	}else{
		if (abs(joystickGetAnalog(stick, 2)) > 20) {
			motorSet(LB,joystickGetAnalog(stick,2));
			motorSet(RB,-joystickGetAnalog(stick,2));
			motorSet(LF,joystickGetAnalog(stick,2));
			motorSet(RF,-joystickGetAnalog(stick,2));
		} else {
			motorStop(LB);
			motorStop(LF);
			motorStop(RB);
			motorStop(RF);
		}
	}
}
