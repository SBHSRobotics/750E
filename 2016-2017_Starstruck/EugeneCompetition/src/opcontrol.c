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

void driveMap(unsigned char frontLeft, unsigned char backLeft, unsigned char frontRight, unsigned char backRight);
void joystickMapMain();
void joystickMapPartner();


void operatorControl() {

//	crabKill();

	while (1) {
		/*switch(currentConfig.id) {
			case TANK_DRIVE:
				driveMap(LF,LB,RF,RB);
				break;
			case SHUFFLE_DRIVE:
				driveMap(LF,LB,RF,RB);
			case HOLONOMIC_DRIVE:
			default:
				driveMap(LF,LB,RF,RB);
				break;
		}*/
		joystickMapMain();
		//joystickMapPartner();

		delay(20);
	}
}

void driveMap(unsigned char frontLeft, unsigned char backLeft, unsigned char frontRight, unsigned char backRight) {
	int thresh = 20;
	int ch1 = joystickGetAnalog(1, 1);
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
		motorSet(frontLeft, ch3 + ch2 + ch4 + ch1);
		motorSet(backRight, -(ch3 + ch2 - ch1 + ch4));
		motorSet(frontRight, -(ch3 + ch2 - ch4 - ch1));
	}
	else if (joystickGetDigital(1, 7, JOY_LEFT)) {
		motorSet(backLeft, 127);
		motorSet(frontLeft, -127);
		motorSet(backRight, -(-127));
		motorSet(frontRight, -(127));
	}
	else if (joystickGetDigital(1, 7, JOY_RIGHT)) {
		motorSet(backLeft, -127);
		motorSet(frontLeft, 127);
		motorSet(backRight, -127);
		motorSet(frontRight, -(-127));
	} else {
		motorStop(backLeft);
		motorStop(frontLeft);
		motorStop(backRight);
		motorStop(frontRight);
	}
	/*#define s 70
	//Lift code
	if (joystickGetDigital(1, 7, JOY_UP)) {
		motorSet(TL, -s);
		motorSet(BL, s);
		motorSet(TR, s);
		motorSet(BR, -s);
	} else if (joystickGetDigital(1, 7, JOY_DOWN)) {
		motorSet(TL, s);
		motorSet(BL, -s);
		motorSet(TR, -s);
		motorSet(BR, s);
	} else {
		motorSet(TL, 0);
		motorSet(BL, 0);
		motorSet(TR, 0);
		motorSet(BR, 0);
	}*/
}

void joystickMapMain(){
	if(joystickGetDigital(1,7,JOY_UP)){
		motorSet(LF,127);
		motorSet(LB,127);
		motorSet(RF,127);
		motorSet(RB,127);
	}

	if(abs(joystickGetAnalog(2,4))>15){
		motorSet(PL,joystickGetAnalog(2,4));
	} else {
		motorStop(PL);
	}

	if(abs(joystickGetAnalog(2,1))>15){
		motorSet(PR,joystickGetAnalog(2,1));
	} else {
		motorStop(PR);
	}

	if(joystickGetDigital(2,6,JOY_UP)){
		motorSet(AC,-127);
		motorSet(BD,127);
	} else if(joystickGetDigital(2,6,JOY_DOWN)) {
		motorSet(AC,127);
		motorSet(BD,-127);
	} else {
		motorSet(AC,-20);
		motorSet(BD,20);
	}

	if((joystickGetDigital(1,8,JOY_DOWN))&&(!isCrabKilled)) {
		setDriveConfig(holonomicDrive);
	}
	if((joystickGetDigital(1,8,JOY_UP))&&(!isCrabKilled)) {
		setDriveConfig(tankDrive);
	}
	if((joystickGetDigital(1,8,JOY_LEFT))&&(!isCrabKilled)){
		setDriveConfig(shuffleDrive);
	}
	driveMap(LF,LB,RF,RB);
}

void joystickMapPartner(){

}
