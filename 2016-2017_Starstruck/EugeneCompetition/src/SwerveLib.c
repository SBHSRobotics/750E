/*
 * Eugene.c
 *
 *  Created on: Dec 3, 2016
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

// Private Global variable declarations
ServoSystem crabLeftServo;
ServoSystem crabRightServo;
// Public function definitions
void crabInit(){
	unsigned char leftArr[4] = {LR, 0, 0, 0};
	unsigned char rightArr[4] = {RR, 0, 0, 0};
	bool boolArr[4] = {false, false, false, false};
	crabLeftServo = servoInit(LP, leftArr, boolArr, PID_MOTOR_SCALE, PID_THRESH);
	crabRightServo = servoInit(RP, rightArr, boolArr, PID_MOTOR_SCALE, PID_THRESH);

	#if (DEBUG_MODE == 0 || DEBUG_MODE == 2)
		setDriveConfig(DEFAULT_DRIVE_MODE);
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
	taskDelete(crabLeftServo.task);
	taskDelete(crabRightServo.task);
}

void setDriveConfig(DriveConfiguration config) {
	currentConfig = config;
	crabLeftServo.targetValue = config.leftWheel;
	crabRightServo.targetValue = config.rightWheel;
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
