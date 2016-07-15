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
#include "RobotLib.h"
#include <math.h>


#define LIFT_LEFT 1
#define LIFT_RIGHT 2
#define LIFT_THRESH 200

//int liftDir = 0;

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

//TaskHandle control;

//void teleOp(){
//	while (1&&!isAutonomous) {
//		 printf("Left: %d\tRight: %d\n\r",analogReadCalibrated(1),abs(analogReadCalibrated(2)));
//		 //printf("Main: %d\tBackup: %d\n\r",powerLevelMain(),powerLevelBackup());
//		 driveMap(1);
//		 liftMap(2);
//	wait(20);
//	}
//	taskDelete(control);
//}

void operatorControl() {

	//control = createAsyncTask(teleOp);

	// Encoder enc = encoderInit(1, 2, false);
	while(1){
		driveMap(1);
		liftMap(2);
		// printf("Encoder: %d\n\r", encoderGet(enc));
		printf("Pot: %d\n\r", analogRead(1));
		delay(20);
//		if((joystickGetDigital(1,8,JOY_LEFT)&&joystickGetDigital(1,8,JOY_RIGHT))
//				||(joystickGetDigital(2,8,JOY_LEFT)&&joystickGetDigital(2,8,JOY_RIGHT))){
//			motorStopAll();
//			taskSuspend(control);
//			motorStopAll();
//			delay(1000);
//			taskResume(control);
//
//		}
////		if(!digitalRead(1))
////			lift(UP);
////		else if(!digitalRead(2))
////			lift(DOWN);
////		else
////			lift(STOP);
	}
}
