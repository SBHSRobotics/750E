/*
 * SwerveLib.c
 *
 *  Created on: Jul 24, 2016
 *      Author: Mikel Matticoli
 */

#include <main.h>
#include <time.h>

// Define drive configurations (Must be tuned based on potentiometer values at a given position)
DriveConfiguration holonomicDrive = {HOLONOMIC_DRIVE, 2750, 2750};
DriveConfiguration tankDrive = {TANK_DRIVE, 3400, 2200};
DriveConfiguration shuffleDrive = {SHUFFLE_DRIVE, 1800, 3900};

DriveConfiguration currentConfig;

bool isCrabKilled = false;

// LCD
int count = 0; //keeps track of position in main LCD menu
int manualCount = 0; //kepps track of position in manual test LCD submenu


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
void pincerPID(unsigned char motor, int currentValue, int targetValue);

// Public function definitions
void crabInit() {
	void leftSidePID() {
		while(1) {
			crabPID(LR, analogRead(LP), currentConfig.leftWheel, leftWheel);
			delay(200);
		}
	}

	void rightSidePID() {
		while(1) {
			crabPID(RR, analogRead(RP), currentConfig.rightWheel, rightWheel);
			delay(200);
		}
	}
	#if (DEBUG_MODE == 0 || DEBUG_MODE == 2)
		setDriveConfig(DEFAULT_DRIVE_MODE);
		leftWheel.thread = taskCreate(leftSidePID,TASK_DEFAULT_STACK_SIZE,NULL,TASK_PRIORITY_DEFAULT);
		rightWheel.thread = taskCreate(rightSidePID,TASK_DEFAULT_STACK_SIZE,NULL,TASK_PRIORITY_DEFAULT);
	#endif

	#if (DEBUG_MODE == 1 || DEBUG_MODE == 2)
		void debug() {
			int confOverride = -1;
			while(1) {
				printf("%c[2J", (char)27); // Clear Console
				printf("=SWERVE DEBUG=\n\rLeft Potentiometer:\t%d\n\rRight Potentiometer:\t%d\n\r Config: %d,%d\n\r%d",
						analogRead(LP), analogRead(RP), currentConfig.leftWheel, currentConfig.rightWheel,confOverride);

				if( fcount(stdin) > 0) {
					confOverride = ((int)getchar())-48;
					if(confOverride >=0 ) {
						setDriveConfigById(confOverride);
					}
				} else {
					confOverride = -1;
				}

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

void setDriveConfigById(int id) {
	switch(id) {
		case HOLONOMIC_DRIVE:
			setDriveConfig(holonomicDrive);
			break;
		case TANK_DRIVE:
			setDriveConfig(tankDrive);
			break;
		case SHUFFLE_DRIVE:
			setDriveConfig(shuffleDrive);
			break;
		default:
			setDriveConfig(currentConfig);
			break;
	}
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
	motorSet(motor, -speed);
	}
}
