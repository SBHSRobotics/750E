/** @file opcontrol.c
 * @brief File for operator control code
 *
 * This file should contain the user operatorControl() function and any functions related to it.
 *
 * Copyright (c) 2011-2014, Purdue University ACM SIG BOTS.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of Purdue University ACM SIG BOTS nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL PURDUE UNIVERSITY ACM SIG BOTS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Purdue Robotics OS contains FreeRTOS (http://www.freertos.org) whose source code may be
 * obtained from http://sourceforge.net/projects/freertos/files/ or on request.
 */

#include "main.h"
#include "math.h"
#include <charlesLib.h>

#define DEBUG 1

//1370

/*
 * Runs the user operator control code. This function will be started in its own task with the
 * default priority and stack size whenever the robot is enabled via the Field Management System
 * or the VEX Competition Switch in the operator control mode. If the robot is disabled or
 * communications is lost, the operator control task will be stopped by the kernel. Re-enabling
 * the robot will restart the task, not resume it from where it left off.
 *
 * If no VEX Competition Switch or Field Management system is plugged in, the VEX Cortex will
 * run the operator control task. Be warned that this will also occur if the VEX Cortex is
 * tethered directly to a computer via the USB A to A cable without any VEX Joystick attached.
 *
 * Code running in this task can take almost any action, as the VEX Joystick is available and
 * the scheduler is operational. However, proper use of delay() or taskDelayUntil() is highly
 * recommended to give other tasks (including system tasks such as updating LCDs) time to run.
 *
 * This task should never exit; it should end with some kind of infinite loop, even if empty.
 */

//Global Vars
unsigned char driveMode = RC_CAR;
char shooter = 0;
char ispeed = 0;
char cspeed = 0;

Ultrasonic dist;
Gyro gyro;

//Function Declarations
void standardDrive();
void rcCar();
void rcCarP();
void triggerIO();
void speakDriveMode();
void feedback();

//Function Definitions

void debugCtrl() {
	while(DEBUG) {
		char c = getchar();
		printf("%c\n\r",c);
		switch(c) {
		case 'w':
			motorSet(LF,  -127);
			motorSet(LB,  127);
			motorSet(RF, -127);
			motorSet(RB, -127);
			delay(20);
			break;
		case 's':
			motorSet(LF,  127);
			motorSet(LB,  -127);
			motorSet(RF, 127);
			motorSet(RB, 127);
			delay(20);
			break;
		case 'e':
			encoderReset(eLeft);
			encoderReset(eRight);
			break;
		default:
			motorStopAll();
			break;
		}
	}
}

void operatorControl() {
	//The ~ looks cool on the lcd
	lcdPrint(LCD, 1, "~~750E~~ Mode: %d",driveMode);

	dist = ultrasonicInit(DIST_B, DIST_A);
	gyro = gyroInit(GYRO, 0);
	gyroReset(gyro);

	taskCreate(debugCtrl,TASK_DEFAULT_STACK_SIZE,NULL,TASK_PRIORITY_DEFAULT);

	while (1) {

		if(joystickGetDigital(1,7,JOY_UP)) {
			driveMode+=1;

			if(driveMode > NUM_DRIVES) {
				driveMode=1;
			}

			//Display drive mode
			lcdSetText(LCD,1,"");
			lcdPrint(LCD, 1, "~~750E~~ Mode: %d",driveMode);

			speakDriveMode();


		}  else if(joystickGetDigital(1,7,JOY_DOWN)) {
			driveMode-=1;

			if(driveMode <= 0) {
				driveMode=NUM_DRIVES;
			}

			//Display drive mode
			lcdSetText(LCD,1,"");
			lcdPrint(LCD, 1, "~~750E~~ Mode: %d",driveMode);

			//Plays tone to indicate drive mode
			for(int i=0;i<driveMode;i++) {
				speakerPlayRtttl("Beep: d=4,o=5,b=120: 8g#");
			}
		}


		//Run control checks based on drive config
		switch(driveMode) {
			case STANDARD_DRIVE:
				standardDrive();
				break;
			case RC_CAR:
				rcCar();
				break;
			case RC_CAR_P:
				rcCarP();
				break;
			default:
				standardDrive();
				break;
		}

		//Print battery voltage
		lcdSetText(LCD,2,"");
		lcdPrint(LCD, 2, "Battery: %dmV", powerLevelMain());

		feedback();

		delay(20);
	}
}

void standardDrive() {
	int leftSpeed = joystickGetAnalog(1,3);
	int rightSpeed = -joystickGetAnalog(1,2);

	motorSet(LF, -leftSpeed);
	motorSet(LB, leftSpeed);
	motorSet(RF, rightSpeed);
	motorSet(RB, rightSpeed);

	triggerIO();
}

void rcCar() {
	int LY = joystickGetAnalog(1,3);
	int LX = joystickGetAnalog(1,1);

	if(abs(LX)>20 && abs(LY)>80) {
		LY*=.8;
		LX*=1.2;
	}

	LY=(abs(LY)<20)?0:LY;
	LX=(abs(LX)<20)?0:LX;


	motorSet(LF,  -(LY+LX));
	motorSet(LB,  (LY+LX));
	motorSet(RF, -(LY-LX));
	motorSet(RB, -(LY-LX));

	triggerIO();
}

void rcCarP() {
	int LY = joystickGetAnalog(1,3);
	int LX = (int)(joystickGetAnalog(1,1)*.6);

	if(abs(LX)>20 && abs(LY)>80) {
		LY*=.8;
		LX*=1.2;
	}


	LY=(abs(LY)<20)?0:LY;
	LX=(abs(LX)<20)?0:LX;

	motorSet(LF,  -(LY+LX));
	motorSet(LB,  (LY+LX));
	motorSet(RF, -(LY-LX));
	motorSet(RB, -(LY-LX));

	triggerIO();
}

void triggerIO() {
	ispeed = (joystickGetDigital(1,5,JOY_UP))?127:
			 (joystickGetDigital(1,5,JOY_DOWN))?(-127):0;

	motorSet(INTAKE, ispeed);

	cspeed = (joystickGetDigital(1,6,JOY_UP))?(-127):
			 (joystickGetDigital(1,6,JOY_DOWN))?127:0;

	motorSet(CONVEYOR, cspeed);

	shooter = joystickGetDigital(1,8,JOY_UP)?127:
			  joystickGetDigital(1,8,JOY_DOWN)?0:
			  joystickGetDigital(1,8,JOY_RIGHT)?-40:shooter;

	motorSet(SHOOTER_TL, shooter);
	motorSet(SHOOTER_TR, -shooter);
	motorSet(SHOOTER_BL, shooter);
	motorSet(SHOOTER_BR, -shooter);
}

void speakDriveMode() {
	//Plays tone to indicate drive mode
	for(int i=0;i<driveMode;i++) {
		speakerPlayRtttl("Beep: d=4,o=5,b=120: 8a#");
	}

}

void feedback() {
	printf("Gyro: %d\t Ulrasonic: %d\t EncLeft: %d\t EncRight: %d\n\r",gyroGet(gyro), ultrasonicGet(dist), encoderGet(eLeft), encoderGet(eRight));

	int d = ultrasonicGet(dist);

	if(joystickGetDigital(1,7,JOY_LEFT) && d>0 && d<50) {
		speakerPlayRtttl("Meep: d=4,o=5,b=120: 32a#");
	}

	//This is a compiler directive. The compiler will ignore this code if DEBUG is 0.
//	#if DEBUG
//		printf("LF: %d\tRF: %d\n\rLB: %d\tRB: %d\n\r",
//				motorGet(LF), motorGet(RF),
//				motorGet(LB), motorGet(RB));
//	#endif

}


