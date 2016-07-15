/* @file opcontrol.c
  @brief File for operator control code

  This file should contain the user operatorControl() function and any functions related to it.

  Copyright (c) 2011-2014, Purdue University ACM SIG BOTS.
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
       Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.
       Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.
       Neither the name of Purdue University ACM SIG BOTS nor the
        names of its contributors may be used to endorse or promote products
        derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL PURDUE UNIVERSITY ACM SIG BOTS BE LIABLE FOR ANY
  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

  Purdue Robotics OS contains FreeRTOS (http://www.freertos.org) whose source code may be
  obtained from http://sourceforge.net/projects/freertos/files/ or on request.
 */

#include "main.h"

//#define A

/*
  Runs the user operator control code. This function will be started in its own task with the
  default priority and stack size whenever the robot is enabled via the Field Management System
  or the VEX Competition Switch in the operator control mode. If the robot is disabled or
  communications is lost, the operator control task will be stopped by the kernel. Re-enabling
  the robot will restart the task, not resume it from where it left off.

  If no VEX Competition Switch or Field Management system is plugged in, the VEX Cortex will
  run the operator control task. Be warned that this will also occur if the VEX Cortex is
  tethered directly to a computer via the USB A to A cable without any VEX Joystick attached.

  Code running in this task can take almost any action, as the VEX Joystick is available and
  the scheduler is operational. However, proper use of delay() or taskDelayUntil() is highly
  recommended to give other tasks (including system tasks such as updating LCDs) time to run.

  This task should never exit; it should end with some kind of infinite loop, even if empty.
 */

FILE* file;


//Kalman Object Definition
typedef struct{
	unsigned char ch;
	double p;//process noise covariance
	double k;//measurement noise covariance
	double x;//value
	double q;//estimation error covariance
	double r;//kalman gain
}Kalman;

//Kalman Initialize Function
Kalman filterInit(unsigned char channel){
	Kalman k;
	k.ch=channel;
	k.p=2;
	k.k=0;
	k.x=0;
	k.q=2;
	k.r=2;
	return k;
}

//Read Function
int accelRead(Kalman k){
	int measurement = analogReadCalibrated(k.ch);
	k.p += k.q;
	k.k = (k.p / (k.p + k.r));
	k.x += k.k * (measurement - k.x);
	k.p *= (1 - k.k);

	return k.x;
}

void writeKFValues() {
	Kalman filterx = filterInit(1);//Create Kalman Object X
	//Kalman filtery = filterInit(2);//Create Kalman Object Y
	analogCalibrate(1);//Calibrate Analog Port
	file = fopen("kf.csv", "w");
	while(1){
		//accelRead(KalmanObject)
		fprintf(file, "%d,%d\n", analogReadCalibrated(1), accelRead(filterx));
		printf("RawX: %d   \t FilteredX: %d\n\r",analogReadCalibrated(1),accelRead(filterx));
		//printf("RawY: %d\t FilteredY: %d\n\r",analogReadCalibrated(2),accelRead(filtery));
		delay(2);
	}
}

void forward(int speed, int time) {
	motorSet(2, -speed);
	motorSet(9, speed);
	delay(time);
}

#define backLeft 8
#define middleLeft 1
#define frontLeft 2
#define backRight 9
#define middleRight 10
#define frontRight 3

void operatorControl() {
	/*TaskHandle kfWriter = taskRunLoop(writeKFValues, 20);
	delay(3000);*/

	/*forward(100, 500);
	forward(0, 500);
	forward(-100, 500);
	forward(0, 1000);

	fclose(file);
	taskSuspend(kfWriter);*/

	bool running = false;
	TaskHandle kfWriter = NULL;
	while(1) {
		if (abs(joystickGetAnalog(1, 2) > 10)) {
			//forward(joystickGetAnalog(1, 2), 20);
			motorSet(frontLeft, -127);
			motorSet(middleLeft, -127 * 0.7143);
			motorSet(backLeft, 127);
			motorSet(frontRight, 127);
			motorSet(middleRight, 127 * 0.7143);
			motorSet(backRight, -127);
		} else {
			//forward(0, 20);
			motorStop(backLeft);
			motorStop(middleLeft);
			motorStop(frontLeft);
			motorStop(backRight);
			motorStop(middleRight);
			motorStop(frontRight);
		}

		if (joystickGetDigital(1, 8, JOY_DOWN)) {
			if (!running) {
				kfWriter = taskRunLoop(writeKFValues, 20);
				delay(3000);
				running = true;
			}
		}

		if (joystickGetDigital(1, 7, JOY_DOWN)) {
			if (running) {
				fclose(file);
				taskSuspend(kfWriter);
				running = false;
			}
		}
		delay(20);
	}
}
