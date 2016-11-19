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
void LCD();
void pulseMotor(unsigned char x);

int lcdEnabled;

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

		if(lcdReadButtons(uart1)==LCD_BTN_LEFT+LCD_BTN_RIGHT){
			if(lcdEnabled==0){
				lcdEnabled=1;
			} else {lcdEnabled=0;}
		}

		if(lcdEnabled==1){
			LCD();
		}

		delay(40);
	}
}

void pulseMotor(unsigned char x){
   motorSet(x,127);
   delay(250);
   motorSet(x,-127);
   delay(250);
   motorSet(x,0);
 }

void LCD(){
  lcdClear(uart1);
  lcdSetBacklight(uart1, true);
  int count = 0;
  int manualCount = 0;
  while(1){
    delay(200);
     //uart1, line #, string, any variable for %d
    switch (count) {
      case POT_MENU:
        lcdSetText(uart1,1, " Potentiometers ");
        lcdSetText(uart1,2, "<    Select    >");
        if (lcdReadButtons(uart1)==LCD_BTN_LEFT){
          count=MEMES_MENU;
        } else if (lcdReadButtons(uart1)==LCD_BTN_RIGHT){
          count++;
        }
        else if(lcdReadButtons(uart1)==LCD_BTN_CENTER){
          count = POT_VAL;
        }
        break;
      case DRIVE_MENU:
        lcdSetText(uart1,1,"   Drive Mode   ");
        lcdSetText(uart1,2,"<    Select    >");
        if (lcdReadButtons(uart1)==LCD_BTN_LEFT){
          count--;
        }
        else if (lcdReadButtons(uart1)==LCD_BTN_RIGHT){
          count++;
        }
        else if(lcdReadButtons(uart1)==LCD_BTN_CENTER){
          count=DRIVE_VAL;
        }
        break;
      case SELF_MENU:
        lcdSetText(uart1,1,"   Self Test    ");
        lcdSetText(uart1,2,"<    Select    >");
        if (lcdReadButtons(uart1)==LCD_BTN_LEFT){
          count--;
        }
        else if (lcdReadButtons(uart1)==LCD_BTN_RIGHT){
          count++;
        }
        else if(lcdReadButtons(uart1)==LCD_BTN_CENTER){
          count=SELF_VAL;
        }
        break;
      case MANUAL_MENU:
        lcdSetText(uart1,1,"  Manual Test   ");
        lcdSetText(uart1,2,"<    Select    >");
        if (lcdReadButtons(uart1)==LCD_BTN_LEFT){
          count--;
        }
        else if (lcdReadButtons(uart1)==LCD_BTN_RIGHT){
          count++;
        }
        else if(lcdReadButtons(uart1)==LCD_BTN_CENTER){
          count=MANUAL_VAL;
        }
        break;
      case MEMES_MENU:
        lcdSetText(uart1,1,"   Dank Memes   ");
        lcdSetText(uart1,2,"<    Select    >");
        if (lcdReadButtons(uart1)==LCD_BTN_LEFT){
          count--;
        }
        else if (lcdReadButtons(uart1)==LCD_BTN_RIGHT){
          count=POT_MENU;
        }
        else if(lcdReadButtons(uart1)==LCD_BTN_CENTER){
          count=MEMES_VAL;
        }
        break;
      case POT_VAL: //potentiometer readings
        lcdPrint(uart1,1,"Left: %d", analogRead(LP));
        lcdPrint(uart1,2,"Right: %d", analogRead(RP));
        if(lcdReadButtons(uart1)==LCD_BTN_CENTER)
          count = POT_MENU; //go back to select menu
        break;
      case DRIVE_VAL: //get current drive mode
        lcdPrint(uart1,1,"Drive Mode");
        printf("%d",currentConfig.id);
        switch(currentConfig.id){
          case 0: //HOLO
            lcdPrint(uart1,2,"Holonomic %d",currentConfig.id);
            break;
          case 1: //TANK
            lcdPrint(uart1,2,"Tank %d",currentConfig.id);
            break;
          case 2: //SHUFFLE
            lcdPrint(uart1,2,"Shuffle %d",currentConfig.id);
            break;
        }
        if(lcdReadButtons(uart1)==LCD_BTN_CENTER){
          count=DRIVE_MENU;
        }
        break;
      case SELF_VAL: //self test
        //currentConfig = holonomicDrive;
        lcdClear(uart1);
        lcdPrint(uart1, 2, "Don't Touch!!!!!");
        for(unsigned char x=1;x<=10;x++){
          lcdPrint(uart1, 1, "Motor %d",x);
          pulseMotor(x);
        }
        count=SELF_MENU;
        break;
      case MANUAL_VAL: //manual test
        lcdClear(uart1);
        //TODO: this doesnt work fix it it doesnt switch in submenu
        lcdPrint(uart1,1,"Choose Motor");
        printf("%d",lcdReadButtons(uart1));
        switch(manualCount){
          case 0:
            lcdPrint(uart1,2,"<    Motor 1   >");
            if(lcdReadButtons(uart1)==LCD_BTN_LEFT){
              manualCount=10;
              printf("reached");
            } else if(lcdReadButtons(uart1)==LCD_BTN_RIGHT){
              manualCount++;
              printf("reached");
              printf("%d",manualCount);
            } else if(lcdReadButtons(uart1)==LCD_BTN_CENTER){
              pulseMotor(manualCount+1);
            }
            break;
          case 1:
            lcdPrint(uart1,2,"<    Motor 2   >");
            if(lcdReadButtons(uart1)==LCD_BTN_LEFT){
              manualCount--;
            } else if(lcdReadButtons(uart1)==LCD_BTN_RIGHT){
              manualCount++;
            } else if(lcdReadButtons(uart1)==LCD_BTN_CENTER){
              pulseMotor(manualCount+1);
            }
            break;
          case 2:
            lcdPrint(uart1,2,"<    Motor 3   >");
            if(lcdReadButtons(uart1)==LCD_BTN_LEFT){
              manualCount--;
            } else if(lcdReadButtons(uart1)==LCD_BTN_RIGHT){
              manualCount++;
            } else if(lcdReadButtons(uart1)==LCD_BTN_CENTER){
              pulseMotor(manualCount+1);
            }
            break;
          case 3:
            lcdPrint(uart1,2,"<    Motor 4   >");
            if(lcdReadButtons(uart1)==LCD_BTN_LEFT){
              manualCount--;
            } else if(lcdReadButtons(uart1)==LCD_BTN_RIGHT){
              manualCount++;
            } else if(lcdReadButtons(uart1)==LCD_BTN_CENTER){
              pulseMotor(manualCount+1);
            }
            break;
          case 4:
            lcdPrint(uart1,2,"<    Motor 5   >");
            if(lcdReadButtons(uart1)==LCD_BTN_LEFT){
              manualCount--;
            } else if(lcdReadButtons(uart1)==LCD_BTN_RIGHT){
              manualCount++;
            } else if(lcdReadButtons(uart1)==LCD_BTN_CENTER){
              pulseMotor(manualCount+1);
            }
            break;
          case 5:
            lcdPrint(uart1,2,"<    Motor 6   >");
            if(lcdReadButtons(uart1)==LCD_BTN_LEFT){
              manualCount--;
            } else if(lcdReadButtons(uart1)==LCD_BTN_RIGHT){
              manualCount++;
            } else if(lcdReadButtons(uart1)==LCD_BTN_CENTER){
              pulseMotor(manualCount+1);
            }
            break;
          case 6:
            lcdPrint(uart1,2,"<    Motor 7   >");
            if(lcdReadButtons(uart1)==LCD_BTN_LEFT){
              manualCount--;
            } else if(lcdReadButtons(uart1)==LCD_BTN_RIGHT){
              manualCount++;
            } else if(lcdReadButtons(uart1)==LCD_BTN_CENTER){
              pulseMotor(manualCount+1);
            }
            break;
          case 7:
            lcdPrint(uart1,2,"<    Motor 8   >");
            if(lcdReadButtons(uart1)==LCD_BTN_LEFT){
              manualCount--;
            } else if(lcdReadButtons(uart1)==LCD_BTN_RIGHT){
              manualCount++;
            } else if(lcdReadButtons(uart1)==LCD_BTN_CENTER){
              pulseMotor(manualCount+1);
            }
            break;
          case 8:
            lcdPrint(uart1,2,"<    Motor 9   >");
            if(lcdReadButtons(uart1)==LCD_BTN_LEFT){
              manualCount--;
            } else if(lcdReadButtons(uart1)==LCD_BTN_RIGHT){
              manualCount++;
            } else if(lcdReadButtons(uart1)==LCD_BTN_CENTER){
              pulseMotor(manualCount+1);
            }
            break;
          case 9:
            lcdPrint(uart1,2,"<   Motor 10   >");
            if(lcdReadButtons(uart1)==LCD_BTN_LEFT){
              manualCount--;
            } else if(lcdReadButtons(uart1)==LCD_BTN_RIGHT){
              manualCount++;
            } else if(lcdReadButtons(uart1)==LCD_BTN_CENTER){
              pulseMotor(manualCount+1);
            }
            break;
          case 10:
            lcdPrint(uart1,2,"<     Exit     >");
            if(lcdReadButtons(uart1)==LCD_BTN_LEFT){
              manualCount--;
            } else if(lcdReadButtons(uart1)==LCD_BTN_RIGHT){
              manualCount=0;
            } else if(lcdReadButtons(uart1)==LCD_BTN_CENTER){
              count=MANUAL_MENU;
              manualCount=0;
            }
            break;
        }
        break;
      case MEMES_VAL:
        break;
      default: count=0;
        break;
    }
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
	/*if((joystickGetDigital(1,7,JOY_DOWN))&&(joystickGetDigital(1,7,JOY_RIGHT))){ //end crab default to tank
		motorSet(LR, 127);
		motorSet(RR, 127);
		delay(1000);
		motorSet(LR, 0);
		motorSet(RR, 0);

		setDriveConfig(tankDrive);
		isCrabKilled = true;
	}*/
	if((joystickGetDigital(1,6,JOY_DOWN))&&(!isCrabKilled)) {
		setDriveConfig(holonomicDrive);
	}
	if((joystickGetDigital(1,6,JOY_UP))&&(!isCrabKilled)) {
		setDriveConfig(tankDrive);
	}
	if((joystickGetDigital(1,5,JOY_UP))&&(!isCrabKilled)){
		setDriveConfig(shuffleDrive);
	}
	driveMap(LF,LB,RF,RB);
	//pneumatics code
	/*if(joystickGetDigital(1,8,JOY_UP)){
		digitalWrite(SOL, HIGH);
	}
	if(joystickGetDigital(1,8,JOY_DOWN)){
		digitalWrite(SOL, LOW);
	}*/
}

void joystickMapPartner(){
	//TODO: change joystick from 1 to 2
			motorSet(TL,(joystickGetAnalog(1,2)<30)&&(joystickGetAnalog(1,2)>-30) ? 0 : -joystickGetAnalog(1,2)); //TODO: fuck this
			motorSet(BL,(joystickGetAnalog(1,2)<30)&&(joystickGetAnalog(1,2)>-30) ? 0 : -joystickGetAnalog(1,2));
			motorSet(TR,(joystickGetAnalog(1,2)<30)&&(joystickGetAnalog(1,2)>-30) ? 0 : joystickGetAnalog(1,2));
			motorSet(BR,(joystickGetAnalog(1,2)<30)&&(joystickGetAnalog(1,2)>-30) ? 0 : joystickGetAnalog(1,2));
}
