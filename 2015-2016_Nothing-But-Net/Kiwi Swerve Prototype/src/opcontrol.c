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

#define LR 2
#define LW 3
#define MR 4
#define MW 5
#define RR 6
#define RW 7

#define LP 1
#define MP 2
#define RP 3

static const int PID_MOTOR_SCALE = 107;
static const int PID_THRESH = 50;

TaskHandle PID1;
TaskHandle PID2;
TaskHandle PID3;

unsigned char crabPID(int motor, int currentValue, int targetValue);

void operatorControl() {

	PID1=NULL;
	PID2=NULL;
	PID3=NULL;

	printf("INIT: Left: %d Middle: %d  Right: %d\n\r",analogRead(1),analogRead(2),analogRead(3));

	while(1) {
//		if(joystickGetDigital(1,8,JOY_UP)) {
//			motorSet(8,127);
//			motorSet(9,-127);
//			motorSet(10,-127);
//		} else {
//			motorStopAll();
//		}

//		if(joystickGetDigital(1,8,JOY_DOWN)){
//			motorSet(1,127);
//			motorSet(9,127);
//			motorSet(10,-127);
//		}
		int targetVal = analogRead(4);

		unsigned char leftHit = 0,
					  middleHit = 0,
					  rightHit = 0;
		printf("INIT: Left: %d Middle: %d  Right: %d\n\r",analogRead(1),analogRead(2),analogRead(3));

		while (!leftHit || !middleHit || !rightHit) {
			//printf("PID %d\t%d\t%d\t",motorGet(2),motorGet(4),motorGet(6));
			printf("Left: %d\t\t Middle: %d\t\t Right: %d\n\r",analogRead(1),analogRead(2),analogRead(3));

			leftHit = crabPID(LR, analogRead(LP), targetVal);
			middleHit = crabPID(MR, analogRead(MP), targetVal);
			rightHit = crabPID(RR, analogRead(RP), targetVal);
		}

		motorStopAll();
		printf("HIT\n\r");
		delay(20);
	}
}

unsigned char crabPID(int motor, int currentValue, int targetValue) {
	// TODO Integral ~ motors still oscilate sporadically

	if (abs(targetValue-currentValue)<PID_THRESH) {
		motorStop(motor);
		return 1;
	}

	int speed = (int)((((double)targetValue - (double)currentValue)
			/ (double)4095) * (double)PID_MOTOR_SCALE);

	speed +=
		( (speed>0)
			?(127 - PID_MOTOR_SCALE)
			:(-127 + PID_MOTOR_SCALE)
		 );

	motorSet(motor, speed);

	return 0;
}
