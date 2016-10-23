/*
 * SwerveLib.c
 *
 *  Created on: Jul 24, 2016
 *      Author: Mikel Matticoli
 */

#include <main.h>
#include <time.h>

// Define drive configurations (Must be tuned based on potentiometer values at a given position)
DriveConfiguration holonomicDrive = {HOLONOMIC_DRIVE, 2000, 2000};
DriveConfiguration tankDrive = {TANK_DRIVE, 1250, 2750};
DriveConfiguration shuffleDrive = {SHUFFLE_DRIVE, 12750, 1250};

DriveConfiguration currentConfig;

// Private Global Constants
static const double Kp=1.0,
			 	 	Ki=1.0;

// Private types
typedef struct CrabGroup {
	TaskHandle thread;
	int errSum;
	double timeSum;
}CrabGroup;	// Defines a wheel or group of wheels controlled by a motor/potentiometer pair

// Private Global variable declarations
CrabGroup leftWheel;
CrabGroup rightWheel;

// Private function declarations
/**/
void crabPID(unsigned char motor, int currentValue, int targetValue, CrabGroup group);


// Public function definitions
void crabInit(){
	void leftSidePID(){
		while(1)
			crabPID(LR, analogRead(LP), currentConfig.leftWheel, leftWheel);
	}

	void rightSidePID(){
		while(1)
			crabPID(RR, analogRead(RP), currentConfig.rightWheel, rightWheel);
	}
	#if (DEBUG_MODE == 0)
		setDriveConfig(DEFAULT_DRIVE_MODE);
		leftWheel.thread = taskCreate(leftSidePID,TASK_DEFAULT_STACK_SIZE,NULL,TASK_PRIORITY_DEFAULT);
		rightWheel.thread = taskCreate(rightSidePID,TASK_DEFAULT_STACK_SIZE,NULL,TASK_PRIORITY_DEFAULT);
	#endif

	#if (DEBUG_MODE == 1)
		void debug() {
			while(1) {
				printf("%c[2J", (char)27); // Clear Console
				printf("=SWERVE DEBUG=\n\rLeft Potentiometer:\t%d\n\rRight Potentiometer:\t%d",analogRead(LR), analogRead(RR));
				delay(100);
			}
		}
		taskCreate(debug,TASK_DEFAULT_STACK_SIZE,NULL,TASK_PRIORITY_DEFAULT);
	#endif
}

void crabKill() {
	taskDelete(leftWheel.thread);
	taskDelete(rightWheel.thread);
}

void setDriveConfig(DriveConfiguration config) {
	currentConfig = config;
	leftWheel.errSum = 0;
	rightWheel.errSum = 0;
}


// Private function definitions

void crabPID(unsigned char motor, int currentValue, int targetValue, CrabGroup group) {
	// TODO: Integral ~ motors still oscillate sporadically

	// Do not run motors if they have reached the target position
	if (abs(targetValue-currentValue)<PID_THRESH) {
		motorStop(motor);
		return;
	}

	// Proportional <calculate error>
	double err = (double)Kp*((double)targetValue - (double)currentValue);

	//Integral
//	group.errSum += err;
//	group.prevIter = time(NULL);

	//Convert Target Value to Motor Speed
	int speed = (int)(((double)err / ((double)4095) * (double)PID_MOTOR_SCALE) );

	// Shift speed scale past deadzone
	speed +=
		( (speed > 0)
			? (127 - PID_MOTOR_SCALE)
			: (-127 + PID_MOTOR_SCALE)
		 );
	motorSet(motor, speed);
}
