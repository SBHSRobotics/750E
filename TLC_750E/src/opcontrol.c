/** @file opcontrol.c
 * @brief File for operator control code
 *
 * This file should contain the user operatorControl() function and any functions related to it.
 *
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/
 *
 * PROS contains FreeRTOS (http://www.freertos.org) whose source code may be
 * obtained from http://sourceforge.net/projects/freertos/files/ or on request.
 */

#include "variables.h"
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
 int ch1;
 int ch2;
 int ch3;
 int ch4;
 int ch6U;
 int ch6D;
 int ch5U;
 int ch5D;
 int thresh = 20;

void operatorControl() {
	while (1) {
		ch1 = joystickGetAnalog(1, 1);
		ch2 = joystickGetAnalog(1, 2);
		ch3 = joystickGetAnalog(1, 3);
		ch4 = joystickGetAnalog(1, 4);
		ch6U = joystickGetDigital(1, 6, JOY_UP);
		ch6D = joystickGetDigital(1, 6, JOY_DOWN);
		ch5U = joystickGetDigital(1, 5, JOY_UP);
		ch5D = joystickGetDigital(1, 5, JOY_DOWN);

		if((abs(ch1) > 0) || (abs(ch2) > 0) || (abs(ch3) > 0) || (abs(ch4) > 0)){
			if(abs(ch1) < thresh) {
				ch1 = 0;
			}
			if(abs(ch2) < thresh) {
				ch2 = 0;
			}
			if(abs(ch3) < thresh) {
				ch3 = 0;
			}
			if(abs(ch4) < thresh) {
				ch4 = 0;
			}

			if(abs(ch3) > abs(ch2)) {
				ch2 = 0;
			}
			else {
				ch3 = 0;
			}

			if(abs(ch3) > 80){
				ch1*=.707106781186548;
			}


			drive(ch1, ch2, ch3, ch4);
		}
		else if (joystickGetDigital(1, 7, JOY_LEFT)) {
			motorSet(MOTOR_BASE_LEFT1_, 127);
			motorSet(MOTOR_BASE_LEFT2_, -127);
			motorSet(MOTOR_BASE_RIGHT1_, -127);
			motorSet(MOTOR_BASE_RIGHT2_, 127);
		}
		else if(joystickGetDigital(1, 7, JOY_RIGHT)) {
			motorSet(MOTOR_BASE_LEFT1_, -127);
			motorSet(MOTOR_BASE_LEFT2_, 127);
			motorSet(MOTOR_BASE_RIGHT1_, 127);
			motorSet(MOTOR_BASE_RIGHT2_, -127);
		}
		else {
			motorStop(MOTOR_BASE_LEFT1_);
			motorStop(MOTOR_BASE_LEFT2_);
			motorStop(MOTOR_BASE_RIGHT1_);
			motorStop(MOTOR_BASE_RIGHT2_);
		}

/*		if((ch6U == 1) || (ch6D == 1)){
			lift((ch6U-ch6D)*127);
		}
		else {
			lift(0);
		}

		if((ch5U == 1) || (ch5D == 1)){
			pince((ch5U-ch5D)*127);
		}
		else {
			pince(0);
		} */
	}
}
