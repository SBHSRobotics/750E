/** @file init.c
 * @brief File for initialization code
 *
 * This file should contain the user initialize() function and any functions related to it.
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

Ultrasonic front;
Ultrasonic back;
Encoder encoder;
Gyro gyro;
TaskHandle autonSelectorTask;

int sectionSize = POT_MAX_VALUE / SECTIONS;

void autonSelector(void *ignore) {
	while (1) {
		autonPath = analogRead(AUTON_POT) / sectionSize;

		lcdSetText(LCD, 1, "Auton Selector");
		switch (autonPath) {
		case RED_CUBE_1:
			lcdSetText(LCD, 2, "Red CUBE 1");
			break;
		case RED_CUBE_2:
			lcdSetText(LCD, 2, "Red Cube 2");
			break;
		case RED_CUBE_3:
			lcdSetText(LCD, 2, "Red Cube 3");
			break;
		case RED_SKYRISE:
			lcdSetText(LCD, 2, "Red Skyrise");
			break;
		case BLUE_CUBE_1:
			lcdSetText(LCD, 2, "Blue Cube 1");
			break;
		case BLUE_CUBE_2:
			lcdSetText(LCD, 2, "Blue Cube 2");
			break;
		case BLUE_CUBE_3:
			lcdSetText(LCD, 2, "Blue Cube 3");
			break;
		case BLUE_SKYRISE:
			lcdSetText(LCD, 2, "Blue Skyrise");
			break;
		}

		if (lcdReadButtons(LCD) != 0) {
			lcdSetText(LCD, 1, "Auton Selected");
			taskDelete(autonSelectorTask);
		}

		delay(20);
	}
}

/*
 * Runs pre-initialization code. This function will be started in kernel mode one time while the
 * VEX Cortex is starting up. As the scheduler is still paused, most API functions will fail.
 *
 * The purpose of this function is solely to set the default pin modes (pinMode()) and port
 * states (digitalWrite()) of limit switches, push buttons, and solenoids. It can also safely
 * configure a UART port (usartOpen()) but cannot set up an LCD (lcdInit()).
 */
void initializeIO() {
	digitalWrite(CUBE_IN,LOW);
	digitalWrite(SKYRISE_IN,LOW);
	pinMode(CUBE_IN,OUTPUT);
	pinMode(SKYRISE_IN,OUTPUT);
}

/*
 * Runs user initialization code. This function will be started in its own task with the default
 * priority and stack size once when the robot is starting up. It is possible that the VEXnet
 * communication link may not be fully established at this time, so reading from the VEX
 * Joystick may fail.
 *
 * This function should initialize most sensors (gyro, encoders, ultrasonics), LCDs, global
 * variables, and IMEs.
 *
 * This function must exit relatively promptly, or the operatorControl() and autonomous() tasks
 * will not start. An autonomous mode selection menu like the pre_auton() in other environments
 * can be implemented in this task if desired.
 */
int auton;

void initialize() {
	setTeamName("750E");
	front = ultrasonicInit(ULTRA_FRONT_1, ULTRA_FRONT_2);
	back = ultrasonicInit(ULTRA_BACK_1, ULTRA_BACK_2);
	encoder = encoderInit(ENCODER_1, ENCODER_2, false);
	gyro = gyroInit(1, 0);
	//int exit = 0;
	//imeReinitialize();
	speakerInit();
	lcdInit(LCD);
	lcdClear(LCD);

//			int count=0;
//			while (!exit) {
//				delay(20);
//
//		// Selecting which auton program.
//				if (joystickGetDigital(1,7,JOY_DOWN)|| joystickGetDigital(2,7,JOY_DOWN))
//				{
//					exit=1;
//				}
//				else
//				{
//					switch (count)
//					{
//		//Autonomous 1.
//					case 0:  lcdSetText(uart2,1, "Autonomous 1");
//					lcdSetText(uart2,2, "<    Select   >");
//					if (lcdReadButtons(uart2)==LCD_BTN_LEFT){
//						count=3;
//					}
//					else if (lcdReadButtons(uart2)==LCD_BTN_RIGHT){
//						count++;
//				         }
//		//If selected, display...
//					else if (lcdReadButtons(uart2)==LCD_BTN_CENTER){
//						lcdSetText(uart2,1,"Autonomous 1");
//						lcdSetText(uart2,2,"is running.");
//						exit=1;
//						break;
//					}
//					break;
//		//Autonomous	 2.
//					case 1: lcdSetText(uart2,1,"Autonomous 2");
//					lcdSetText(uart2,2,"<    Select    >");
//					if (lcdReadButtons(uart2)==LCD_BTN_LEFT){
//						count--;
//						}
//						else if (lcdReadButtons(uart2)==LCD_BTN_RIGHT){
//							count++;
//						}
//		//If selected, display...
//				         else if (lcdReadButtons(uart2)==LCD_BTN_CENTER){
//				        	 lcdSetText(uart2,1,"Autonomous 2");
//				        	 lcdSetText(uart2,2,"is running.");
//							 exit=1;
//				        	 break;
//				         }
//						break;
//		//Autonomous 3.
//				case 2: lcdSetText(uart2,1,"Autonomous 3");
//						lcdSetText(uart2,2,"<    Select    >");
//						if (lcdReadButtons(uart2)==LCD_BTN_LEFT){
//							count--;
//						}
//						else if (lcdReadButtons(uart2)==LCD_BTN_RIGHT){
//							count++;
//						}
//		//If selected, display...
//				         else if (lcdReadButtons(uart2)==LCD_BTN_CENTER){
//				        	 lcdSetText(uart2,1,"Autonomous 3");
//				        	 lcdSetText(uart2,2,"is running.");
//							 exit=1;
//				        	 break;
//				         }
//						break;
//		//Autonomous 4.
//				case 3: lcdSetText(uart2,1,"Autonomous 4");
//						lcdSetText(uart2,2,"<    Select   >");
//						if (lcdReadButtons(uart2)==LCD_BTN_LEFT){
//							count--;
//						}
//						else if (lcdReadButtons(uart2)==LCD_BTN_RIGHT){
//							count++;
//						}
//		//If selected, display...
//				         else if (lcdReadButtons(uart2)==LCD_BTN_CENTER){
//				        	 lcdSetText(uart2,1,"Autonomous 4");
//				        	 lcdSetText(uart2,2,"is running.");
//							 exit=1;
//				        	 break;
//				         }
//						break;
//				case 4: lcdSetText(uart2,1,"Self Destruct");
//										lcdSetText(uart2,2,"<    Select   >");
//										if (lcdReadButtons(uart2)==LCD_BTN_LEFT){
//											count--;
//										}
//										else if (lcdReadButtons(uart2)==LCD_BTN_RIGHT){
//											count=0;
//										}
//						//If selected, display...
//								         else if (lcdReadButtons(uart2)==LCD_BTN_CENTER){
//								        	 lcdSetText(uart2,1,"Autonomous 4");
//								        	 lcdSetText(uart2,2,"is running.");
//								        	 count=0;
//											 exit=1;
//								        	 break;
//								         }
//										break;
//
//				default: count=0;
//						 break;
//
//				}
//
//			}
//			auton = count+1;
//		}
		lcdSetText(uart2,1,"750EVOLUTION");
		lcdSetText(uart2,2,"THIS IS A ROBOT");
		lcdSetBacklight(uart2, true);
		autonSelectorTask = taskCreate(autonSelector, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT);
}
