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
#include <math.h>

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

//Port Definitions
	//Wheels
#define FL 9
#define FR 8
#define BL 3
#define BR 2

//Lift
#define LB 4//D
#define LF 5//C
#define RB 6//B
#define RF 7//A

#define LT 1
#define RT 10

#define SKYRISE_IN 5
#define CUBE_IN 6

#define ANALOG_THRESH 20


int RA_X,RA_Y,LA_X,LA_Y;


void operatorControl() {
	digitalWrite(CUBE_IN,LOW);
	digitalWrite(SKYRISE_IN,LOW);
	pinMode(CUBE_IN,OUTPUT);
	pinMode(SKYRISE_IN,OUTPUT);
	while (1) {
		RA_X = joystickGetAnalog(1, 4);
		LA_Y = joystickGetAnalog(1, 3);

		if(!joystickGetDigital(1,5,JOY_UP)&&(abs(RA_X)<=20)&&(abs(LA_Y)<=20)){
			RA_X = joystickGetAnalog(2, 4);
			LA_Y = joystickGetAnalog(2, 3);
		}

		if(!joystickGetDigital(1,5,JOY_DOWN)&&(!joystickGetDigital(1,6,JOY_UP)&&(!joystickGetDigital(1,6,JOY_DOWN)))){
			if(joystickGetAnalog(2,2)>20){
				motorSet(LB,-127);
				motorSet(RB,127);
				motorSet(LF,-127);
				motorSet(RF,127);
				motorSet(LT,127);
				motorSet(RT,-127);
			}else if(joystickGetAnalog(2,2)<-20){
				motorSet(LB,127);
				motorSet(RB,-127);
				motorSet(LF,127);
				motorSet(RF,-127);
				motorSet(LT,-127);
				motorSet(RT,127);
			}else{
				motorSet(LB,0);
				motorSet(RB,0);
				motorSet(LF,0);
				motorSet(RF,0);
				motorSet(LT,0);
				motorSet(RT,0);
			}
		}else{
			if(joystickGetDigital(1,6,JOY_UP)&&joystickGetDigital(1,6,JOY_DOWN)){
				motorSet(LB,0);
				motorSet(RB,0);
				motorSet(LF,0);
				motorSet(RF,0);
				motorSet(LT,0);
				motorSet(RT,0);
			}else if(joystickGetAnalog(1,2)>20){
				motorSet(LB,-127);
				motorSet(RB,127);
				motorSet(LF,-127);
				motorSet(RF,127);
				motorSet(LT,127);
				motorSet(RT,-127);
			}else if(joystickGetAnalog(1,2)<-20){
				motorSet(LB,127);
				motorSet(RB,-127);
				motorSet(LF,127);
				motorSet(RF,-127);
				motorSet(LT,-127);
				motorSet(RT,127);
			}else{
				motorSet(LB,0);
				motorSet(RB,0);
				motorSet(LF,0);
				motorSet(RF,0);
				motorSet(LT,0);
				motorSet(RT,0);
			}
		}

		if (abs(LA_Y) < ANALOG_THRESH) {
			LA_Y = 0;
		}
		if ((abs(LA_X) < ANALOG_THRESH*2.5)&&(abs(LA_Y) > 95)) {
			LA_X = 0;
		}
		if (abs(RA_Y) < ANALOG_THRESH*3) {
			RA_Y = 0;
		}
		if (abs(RA_X) < ANALOG_THRESH*0.75) {
			RA_X = 0;
		}

		if (abs(LA_Y) > abs(RA_Y)) {
			RA_Y = 0;
		} else {
			LA_Y = 0;
		}

		if(joystickGetDigital(1,7,JOY_UP)){
			digitalWrite(SKYRISE_IN,HIGH);
		}else if(joystickGetDigital(1,7,JOY_DOWN)){
			digitalWrite(SKYRISE_IN,LOW);
		}

		if(joystickGetDigital(1,7,JOY_LEFT)){
			digitalWrite(CUBE_IN,HIGH);
		}else if(joystickGetDigital(1,7,JOY_RIGHT)){
			digitalWrite(CUBE_IN,LOW);
		}

		motorSet(BL, (LA_Y + RA_Y + RA_X - LA_X));
		motorSet(FL, (LA_Y + RA_Y + LA_X + RA_X));
		motorSet(BR, -(LA_Y + RA_Y - RA_X + LA_X));
		motorSet(FR, -(LA_Y + RA_Y - LA_X - RA_X));

		delay(20);
	}
}
