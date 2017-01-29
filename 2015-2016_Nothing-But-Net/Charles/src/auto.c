/** @file auto.c
 * @brief File for autonomous code
 *
 * This file should contain the user autonomous() function and any functions related to it.
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
#include <charlesLib.h>


/*
 * Runs the user autonomous code. This function will be started in its own task with the default
 * priority and stack size whenever the robot is enabled via the Field Management System or the
 * VEX Competition Switch in the autonomous mode. If the robot is disabled or communications is
 * lost, the autonomous task will be stopped by the kernel. Re-enabling the robot will restart
 * the task, not re-start it from where it left off.
 *
 * Code running in the autonomous task cannot access information from the VEX Joystick. However,
 * the autonomous function can be invoked from another task if a VEX Competition Switch is not
 * available, and it can access joystick information if called in this way.
 *
 * The autonomous task may exit, unlike operatorControl() which should never exit. If it does
 * so, the robot will await a switch to another mode or disable/enable cycle.
 */
void autonLeft();
void autonRight();
void autonSkills();
void shooterCounter();
void initAll();
void moveForward();
void shoot();

void autonomous() {
	int i = analogRead(SELECTOR);
	if(i<=1365) {
		autonLeft();
	} else if(i<=2730) {
		autonRight();
	} else {
		autonSkills();
	}

	while(isAutonomous());
}

void initAll() {
	motorSet(SHOOTER_TL, 127);
	motorSet(SHOOTER_TR, -127);
	motorSet(SHOOTER_BL, 127);
	motorSet(SHOOTER_BR, -127);

	delay(2000);

	motorSet(CONVEYOR,-127);
	motorSet(INTAKE,127);
}

void shoot(int s) {
	motorSet(SHOOTER_TL, s);
	motorSet(SHOOTER_TR, -s);
	motorSet(SHOOTER_BL, s);
	motorSet(SHOOTER_BR, -s);
}

void moveForward(int encoderDist) {
	encoderReset(eLeft);
	while(encoderGet(eLeft)<encoderDist) {
		motorSet(LF, -127);
		motorSet(LB, 127);
		motorSet(RF, -127);
		motorSet(RB, -127);
	}
	motorStop(LF);
	motorStop(LB);
	motorStop(RF);
	motorStop(RB);
}

void autonLeft() {
	initAll();
	taskCreate(shooterCounter, TASK_DEFAULT_STACK_SIZE, 0 /*NULL*/,
			TASK_PRIORITY_DEFAULT);

}

void autonRight() {

}

void autonSkills() {
	if(powerLevelMain()>=8000) {
		moveForward(480);
	} else {
		moveForward(850);
	}
	initAll();
	shoot(107);
	taskCreate(shooterCounter, TASK_DEFAULT_STACK_SIZE, 0 /*NULL*/,
			TASK_PRIORITY_DEFAULT);
}

void shooterCounter() {
	while(isAutonomous()) {
		delay(2500);
		speakerPlayRtttl("Boop: d=4,o=5,b=120: 32a#, 32a#");
	}
}
