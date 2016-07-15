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

FILE *file;

#define backLeft 8
#define middleLeft 1
#define frontLeft 2
#define backRight 9
#define middleRight 10
#define frontRight 3

#define thresh 10

#define multiplier 0.7143

#define FORWARD 0
#define RIGHT 90
#define BACKWARD 180
#define LEFT 270

void move(int dir, int t) {
	if (dir == FORWARD) {

	} else if (dir == RIGHT) {

	} else if (dir == BACKWARD) {

	} else if (dir == LEFT) {

	}
}

void drive() {
	/*motorSet(2, joystickGetAnalog(1, 2));
	motorSet(3, joystickGetAnalog(1, 3));*/
	/*	int ch1 = joystickGetAnalog(1, 1);
	int ch2 = joystickGetAnalog(1, 2);
	int ch3 = joystickGetAnalog(1, 3);
	int ch4 = joystickGetAnalog(1, 4);

	if ((abs(ch3) > thresh) || (abs(ch4) > thresh) || (abs(ch2) > thresh) || (abs(ch1) > thresh)) {
		if (abs(ch3) < thresh) {
			ch3 = 0;
		}
		if (abs(ch4) < thresh) {
			ch4 = 0;
		}
		if (abs(ch2) < thresh) {
			ch2 = 0;
		}
		if (abs(ch1) < thresh) {
			ch1 = 0;
		}

		if (abs(ch3) > abs(ch2)) {
			ch2 = 0;
		}
		else {
			ch3 = 0;
		}

		motorSet(backLeft, ch3 + ch2 + ch1 - ch4);
		motorSet(middleLeft, -(ch3 + ch2 + ch1 * multiplier));
		motorSet(frontLeft, ch3 + ch2 + ch4 + ch1);
		motorSet(backRight, -(ch3 + ch2 - ch1 + ch4));
		motorSet(middleRight, ch3 + ch2 - ch1 * multiplier);
		motorSet(frontRight, -(ch3 + ch2 - ch4 - ch1));
	}
	else if (joystickGetDigital(1, 7, JOY_LEFT)) {
		motorSet(backLeft, 127);
		motorSet(middleLeft, 0);
		motorSet(frontLeft, -127);
		motorSet(backRight, -(-127));
		motorSet(middleRight, 0);
		motorSet(frontRight, -(127));
	}
	else if (joystickGetDigital(1, 7, JOY_RIGHT)) {
		motorSet(backLeft, -127);
		motorSet(middleLeft, 0);
		motorSet(frontLeft, 127);
		motorSet(backRight, -127);
		motorSet(middleRight, 0);
		motorSet(frontRight, -(-127));
	}
	else {
		motorStop(backLeft);
		motorStop(middleLeft);
		motorStop(frontLeft);
		motorStop(backRight);
		motorStop(middleRight);
		motorStop(frontRight);
	}*/
	if (joystickGetDigital(1, 7, JOY_UP)) {
		motorSet(frontLeft, -127);
		motorSet(middleLeft, -127);
		motorSet(backLeft, 127);
		motorSet(frontRight, 127);
		motorSet(middleRight, 127);
		motorSet(backRight, -127);
	} else if (joystickGetDigital(1, 7, JOY_DOWN)) {
		motorSet(frontLeft, 127);
		motorSet(middleLeft, 127);
		motorSet(backLeft, -127);
		motorSet(frontRight, -127);
		motorSet(middleRight, -127);
		motorSet(backRight, 127);
	} else if (joystickGetDigital(1, 7, JOY_RIGHT)) {
		motorSet(frontLeft, 127);
		motorSet(middleLeft, 0);
		motorSet(backLeft, 127);
		motorSet(frontRight, 127);
		motorSet(middleRight, 0);
		motorSet(backRight, 127);
	} else if (joystickGetDigital(1, 7, JOY_LEFT)) {
		motorSet(frontLeft, -127);
		motorSet(middleLeft, 0);
		motorSet(backLeft, -127);
		motorSet(frontRight, -127);
		motorSet(middleRight, 0);
		motorSet(backRight, -127);
	} else {
		motorStop(backLeft);
		motorStop(middleLeft);
		motorStop(frontLeft);
		motorStop(backRight);
		motorStop(middleRight);
		motorStop(frontRight);
	}
}

void recordData() {
	for (int i = 1; i <= 10; i++) {
		printf("%d\t", motorGet(i));
		fprintf(file, "%d ", motorGet(i));
	}
	fprintf(file, "%d\n", digitalRead(1));
	printf("%d\n\r", digitalRead(1));
}

int getEncoderValue(int port, Encoder enc) {
	if (enc == NULL) {
		int retVal = -1;
		if (!imeGet(port, &retVal)) {
			print("Error reading IME!\n\r");
		}
		return retVal;
	} else {
		return encoderGet(enc);
	}
}

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
void operatorControl() {
	bool recording = false;
	while (1) {
		drive();
		if (joystickGetDigital(1, 8, JOY_UP)) {
			if (!recording) {
				file = fopen("mDat.dat", "w");
				print("Started recording!\n\r");
				recording = true;
			}
		} else if (joystickGetDigital(1, 8, JOY_DOWN)) {
			if (recording) {
				print("Stopped recording!\n\r");
				fclose(file);
				recording = false;
			}
		} else if (joystickGetDigital(1, 8, JOY_LEFT)) {
			print("Started auton!\n\r");
			autonomous();
			print("Finished auton!\n]r");
		}

		if (recording) {
			recordData();
		}
		delay(20);
	}
}
