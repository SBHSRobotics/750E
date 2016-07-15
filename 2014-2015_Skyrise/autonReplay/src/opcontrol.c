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

Mutex doNotRecord;


void strafe(int dir, int t, int s) {
	if (dir == FORWARD) {
		motorSet(FL, s);
		motorSet(ML, -s);
		motorSet(BL, s);
		motorSet(FR, -s);
		motorSet(MR, s);
		motorSet(BR, -s);
	} else if (dir == BACKWARD) {
		motorSet(FL, -s);
		motorSet(ML, s);
		motorSet(BL, -s);
		motorSet(FR, s);
		motorSet(MR, -s);
		motorSet(BR, s);
	} else if (dir == RIGHT) {
		motorSet(FL, s);
		motorSet(ML, 0);
		motorSet(BL, -s);
		motorSet(FR, s);
		motorSet(MR, 0);
		motorSet(BR, -s);
	} else if (dir == LEFT) {
		motorSet(FL, -s);
		motorSet(ML, 0);
		motorSet(BL, s);
		motorSet(FR, -s);
		motorSet(MR, 0);
		motorSet(BR, s);
	} else if (dir == LEFT_TURN) {
		motorSet(FL, s);
		motorSet(ML, -s);
		motorSet(BL, s);
		motorSet(FR, -s);
		motorSet(MR, s);
		motorSet(BR, -s);
	} else if (dir == RIGHT_TURN) {
		motorSet(FL, s);
		motorSet(ML, -s);
		motorSet(BL, s);
		motorSet(FR, s);
		motorSet(MR, -s);
		motorSet(BR, s);
	}
	delay(t);
	stop();
}

void oldPath() {
	/*encoder = encoderInit(1, 2, false);
	gyro = gyroInit(4, 0);
	if (gyro == NULL) {
		print("failed to init gyro");
	}*/

	encoderReset(encoder);
	gyroReset(gyro);

	/*armTo(4095);
	armTo(700);*/
	armTo(3654);
	delay(100);
	armTo(4095);

	turnTo(45);
	encoderReset(encoder);
	goTo(22);
	encoderReset(encoder);
	armTo(3900);
	strafe(RIGHT, 400, 100);
	encoderReset(encoder);
	goTo(22);
	encoderReset(encoder);
	turnTo(135);
	/*armTo(700);
	encoderReset(encoder);
	goTo(7);
	armTo(3800);
	encoderReset(encoder);
	goTo(-5);
	armTo(4095);*/
}
	//goTo(-2);

//	lift(UP);
//	delay(1250);
//	while(analogRead(1)<180)
//		lift(UP);
//	lift(STOP);
//	strafe(BACKWARD,400, 60);
//	delay(1000);
//	lift(UP);
//	delay(700);
//	lift(DOWN);
//	strafe(FORWARD,320, 127);
//	motorStopAll();
//
//	turnTo(-90);
//	//strafe(BACKWARD, 150, 60);
//
//	lift(DOWN);
//	delay(1200);
//	lift(STOP);
//	strafe(FORWARD,500, 127);
//	motorStopAll();

void drive(void) {
	/*if (joystickGetDigital(1, 8, JOY_DOWN)) {
		motorSet(2, 0);
		motorSet(3, 50);
	} else {
		// int speed = 0;
		if (abs(joystickGetAnalog(1, 3)) > 20) {
			motorSet(2, 0);
			motorSet(3, joystickGetAnalog(1, 3));
		} else if (abs(joystickGetAnalog(1, 2)) > 20) {
			motorSet(2, joystickGetAnalog(1, 2));
			motorSet(3, joystickGetAnalog(1, 2));
		} else {
			motorStop(2);
			motorStop(3);
		}
	}*/

	driveMap(1);
	liftMap(2);

	/*if (joystickGetDigital(1, 8, JOY_UP)) {
		oldPath();
	}

	if (joystickGetDigital(1, 8, JOY_LEFT)) {
		turnTo(-90);
	}*/

	/*if (joystickGetDigital(1, 5, JOY_DOWN)) {
		//mutexTake(doNotRecord)
		autonomous();
		while (joystickGetDigital(1, 5, JOY_DOWN)) { delay(5); }
	}*/

	/*if (joystickGetDigital(1, 6, JOY_DOWN)) {
		loadTreeFromStart("b");
		mutexTake(doNotRecord, -1);
		smove(FORWARD, 500, 80);
		mutexGive(doNotRecord);
		if (DEBUG) {
			print("all done here");
			for (int i = 0; i < numPairs; i++) {
				printPair(&pairMap[i]);
			}
			getchar();
		}
		while (joystickGetDigital(1, 6, JOY_DOWN)) { delay(5); }
	}

	if (joystickGetDigital(1, 6, JOY_UP)) {
		writeTreeToFile("fml.dat");
		// createCodeFile();
		mutexTake(doNotRecord, -1);
		smove(FORWARD, 500, 80);
		mutexGive(doNotRecord);
		while (joystickGetDigital(1, 6, JOY_UP)) { delay(5); }
	}

	if (joystickGetDigital(1, 8, JOY_UP)) {
		runCurrentPath(rootNode->child, -1);
		while (joystickGetDigital(1, 7, JOY_UP)) { delay(5); }
	}

	if (joystickGetDigital(1, 7, JOY_DOWN)) {
		restartTree();
		while (joystickGetDigital(1, 7, JOY_DOWN)) { delay(5); }
	}

	if (joystickGetDigital(1, 7, JOY_LEFT)) {
		printTree();
		while (joystickGetDigital(1, 7, JOY_LEFT)) { delay(5); }
	}*/

	/*if (joystickGetDigital(1, 7, JOY_RIGHT)) {
		printf("DEBUG: Memory Available: %1.3f KB\n\r", (float)measureAvailableMemory() / 1000);
		while (joystickGetDigital(1, 7, JOY_RIGHT)) { delay(5); }
	}*/

	/*createTestTree();
	printTree();
	delay(500);

	writeTreeToFile("test.dat");
	delay(500);

	restartTree();
	printTree();
	delay(500);

	loadTreeFromFile("test.dat");
	printTree();
	delay(500);*/
}

// This is the stupidest code I have ever written for VEX
void getVEXNETsConnected() {
	void printBS(void) {
		print("hi");
	}

	TaskHandle spewBS = taskRunLoop(printBS, 20);
	getchar();
	taskDelete(spewBS);
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
 * tethered kn to a computer via the USB A to A cable without any VEX Joystick attached.
 *
 * Code running in this task can take almost any action, as the VEX Joystick is available and
 * the scheduler is operational. However, proper use of delay() or taskDelayUntil() is highly
 * recommended to give other tasks (including system tasks such as updating LCDs) time to run.
 *
 * This task should never exit; it should end with some kind of infinite loop, even if empty.
 */

void operatorControl() {
	/*if (!isOnline()) {
		getVEXNETsConnected();
	}*/
	printDebug("hi");
	// taskRunLoop(runAutonReader, 20);
	/*taskCreate(runAutonReader, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT + 1);
	delay(200);
	while (!mutexTake(readerInit, 10)) {
		//printDebug("Waiting!");
		delay(20);
	}
	// mutexDelete(readerInit);
	// createTestTree();
	// printTree();
	// print("hi");
	// getchar();
	// createCodeFile();
	if (!DEBUG) {
		// taskRunLoop(runEditor, 20);
		taskCreate(runEditor, TASK_DEFAULT_STACK_SIZE / 2, NULL, TASK_PRIORITY_DEFAULT);
	}*/
	while (true) {
		// print("hi");
		// mutexTake(doNotDrive, -1);
		drive();
		// mutexGive(doNotDrive);
		delay(20);
	}
}
