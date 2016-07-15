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
int prevTarget = 0;
int start = 0;

int power;
bool hotkey = false;

/*void pidToUlt(Ultrasonic ult, int target, Gyro gyro, int heading) {
	if (ultrasonicGet(ult) != 0)
	{
		int power = pid_wheels(ultrasonicGet(ult), target);
		float gyroErr = gyroGet(gyro) - heading;
		const float gyroConst = 3;
		motorSet(BL, power + (int)(gyroConst * gyroErr));
		motorSet(FL, power + (int)(gyroConst * gyroErr));
		motorSet(BR, -power - (int)(gyroConst * gyroErr));
		motorSet(FR, -power - (int)(gyroConst * gyroErr));
	}

}

void pidToEnc(Encoder enc, int target, Gyro gyro, int heading) {
	if(target != prevTarget) start = encoderGet(enc);
	int power = pid_wheels(encoderGet(enc), target);
	float gyroErr = gyroGet(gyro) - heading;
	const float gyroConst = 0;
	if(encoderGet(enc) < 100) power = 50+encoderGet(enc);
	motorSet(BL, -power - (int)(gyroConst * gyroErr));
	motorSet(FL, power + (int)(gyroConst * gyroErr));
	motorSet(BR, -power - (int)(gyroConst * gyroErr));
	motorSet(FR, power + (int)(gyroConst * gyroErr));
}*/

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

void startLCDMenu();

void operatorControl() {
	// printf("Helllo World!");
	// startLCDMenu();
	// analogCalibrate(6);
	lcdPrint(LCD, 1, "Main Batt %1.3fV", powerLevelMain() / 1000.0f);
	lcdPrint(LCD, 2, "Bkup Batt %1.3fV", powerLevelBackup() / 1000.0f);
	// gyroReset(gyro);
	while(1){
		printf("%1.3fV\n\r", powerLevelMain() / 1000.0f);
		driveMap(1);
		liftMap(2);

		/*if (joystickGetDigital(1, 8, JOY_RIGHT)) {
//			scoreSkyriseAtLevel(0);
//			skyriseAuton();
			armTo(2000);
		} else if (joystickGetDigital(1, 8, JOY_DOWN)) {
			scoreSkyriseAtLevel(1);
		}*/
//		printf("Gyro: %d    Pot: %d\n\r", gyroGet(gyro), analogRead(POT_LEFT));

		hotkey = false;
		if (joystickGetDigital(2, 7, JOY_LEFT))
		{
			power = pid_arm(analogRead(POT_LEFT), 1860);
			hotkey = true;
		}
		else if (joystickGetDigital(2, 7, JOY_RIGHT))
		{
			power = pid_arm(analogRead(POT_LEFT), 2470);
			hotkey = true;
		}
		else if (joystickGetDigital(2, 8, JOY_LEFT))
		{
			power = pid_arm(analogRead(POT_LEFT), 3240);
			hotkey = true;
		}
		else if (joystickGetDigital(2, 8, JOY_RIGHT))
		{
			power = pid_arm(analogRead(POT_LEFT), 3680);
			hotkey = true;
		}

		if (hotkey)
		{
			motorSet(LB, -power);
			motorSet(LF, -power);
			motorSet(LT, power);
			motorSet(RB, power);
			motorSet(RF, power);
			motorSet(RT, -power);
		}

	}
}

void runLCDCode(void *ignore) {
	FILE *LCD_PORT = uart2;

	pinMode(SKYRISE_IN, OUTPUT);
	pinMode(CUBE_IN, OUTPUT);

	// lcdInit(LCD_PORT);

	unsigned char mode = 0;

	while (true) {
		switch (mode) {
		case 0:
			lcdPrint(LCD_PORT, 1, "Main Batt %1.3fV", powerLevelMain() / 1000.0f);
			lcdPrint(LCD_PORT, 2, "Bkup Batt %1.3fV", powerLevelBackup() / 1000.0f);
			break;
		case 1:
			lcdSetText(LCD_PORT, 1, "Arm Debug Mode");
			lcdSetText(LCD_PORT, 2, "Up          Down");
			break;
		case 2:
			lcdSetText(LCD_PORT, 1, "Pneumatics Mode");
			lcdSetText(LCD_PORT, 2, "Cube     Skyrise");
			break;
		}

		int button = lcdReadButtons(LCD_PORT);
		while (button == 0) {
			printf("%d %d\n\r", button, mode);
			button = lcdReadButtons(LCD_PORT);
			delay(20);
		}

		printf("%d %d\n\r", button, mode);

		switch (button) {
		case 2:
			mode = (mode == 2 ? 0 : mode + 1);
			motorStopAll();
			lcdClear(LCD_PORT);
			break;
		case 1:
			switch (mode) {
			case 1:
				if (motorGet(LB) == 0) {
					motorSet(LB,-70);
					motorSet(RB,70);
					motorSet(LF,-70);
					motorSet(RF,70);
					motorSet(LT,70);
					motorSet(RT,-70);
				} else {
					stopLift();
				}
				break;
			case 2:
				// print("hi");
				digitalWrite(SKYRISE_IN, !digitalRead(SKYRISE_IN));
				break;
			}
			break;

			case 4:
				switch (mode) {
				case 1:
					if (motorGet(LB) == 0) {
						motorSet(LB,50);
						motorSet(RB,-50);
						motorSet(LF,50);
						motorSet(RF,-50);
						motorSet(LT,-50);
						motorSet(RT,50);
					} else {
						stopLift();
					}
					break;
				case 2:
					digitalWrite(CUBE_IN, !digitalRead(CUBE_IN));
					break;
				default:
					break;
				}
				break;

				default:
					break;
		}

		while (lcdReadButtons(LCD_PORT) != 0) {
			delay(20);
		}

		delay(20);
	}
}

void startLCDMenu() {
	taskCreate(runLCDCode, TASK_DEFAULT_STACK_SIZE / 2, NULL, TASK_PRIORITY_DEFAULT - 1);
}
