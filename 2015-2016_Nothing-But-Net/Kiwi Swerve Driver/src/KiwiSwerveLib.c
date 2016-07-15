/*
 * KiwiSwerveLib.c
 *
 *  Created on: Aug 2, 2015
 *      Author: Mikel
 */
#include <main.h>
#include <time.h>


DriverConfiguration kiwiConfig = {2000, 2000, 2000};
DriverConfiguration lock = {3600, 3600, 3600};

typedef struct Wheel{
	TaskHandle thread;
	//time_t prevIter;
	int errSum;
	double timeSum;;
}Wheel;

Wheel leftWheel;
Wheel middleWheel;
Wheel rightWheel;

DriverConfiguration currentConfig;

double Kp=1,
	   Ki=1;

void crabPID(unsigned char motor, int currentValue, int targetValue, Wheel wheel) {
	// TODO Integral ~ motors still oscilate sporadically

	// Do not run motors if they have reached the target position
	if (abs(targetValue-currentValue)<PID_THRESH) {
		motorStop(motor);
		return;
	}

	// Proportional <calculate error>
	double err = (double)Kp*((double)targetValue - (double)currentValue);

	//Integral
//	wheel.errSum += err;

//	wheel.prevIter = time(NULL);

	//Convert Target Value to Motor Speed
	int speed = (int)(((double)err / ((double)4095) * (double)PID_MOTOR_SCALE) );

	// Shift speed scale past deadzone
	speed +=
		( (speed>0)
			?(127 - PID_MOTOR_SCALE)
			:(-127 + PID_MOTOR_SCALE)
		 );

	motorSet(motor, speed);
}

void activateDriveConfig(DriverConfiguration config) {
	currentConfig = config;
	leftWheel.errSum = 0;
	middleWheel.errSum = 0;
	rightWheel.errSum = 0;
}

void driveInit(){
	void leftWheelPID(){
		while(1)
			crabPID(LR, analogRead(LP), currentConfig.leftWheel, leftWheel);
	}
	void middleWheelPID(){
		while(1)
			crabPID(MR, analogRead(MP), currentConfig.middleWheel, middleWheel);
	}
	void rightWheelPID(){
		while(1)
			crabPID(RR, analogRead(RP), currentConfig.rightWheel, rightWheel);
	}
	void debug() {
		while(1){
			printf("L: %d\tM: %d\t R: %d\n\r",analogRead(LP),analogRead(MP),analogRead(RP));
		}
	}
	activateDriveConfig(lock);
	leftWheel.thread = taskCreate(leftWheelPID,TASK_DEFAULT_STACK_SIZE,NULL,TASK_PRIORITY_DEFAULT);
	middleWheel.thread = taskCreate(middleWheelPID,TASK_DEFAULT_STACK_SIZE,NULL,TASK_PRIORITY_DEFAULT);
	rightWheel.thread = taskCreate(rightWheelPID,TASK_DEFAULT_STACK_SIZE,NULL,TASK_PRIORITY_DEFAULT);
	taskCreate(debug,TASK_DEFAULT_STACK_SIZE,NULL,TASK_PRIORITY_DEFAULT);
}

void driveKill(){
	taskDelete(leftWheel.thread);
	taskDelete(middleWheel.thread);
	taskDelete(rightWheel.thread);
}

