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

/*
 * Runs pre-initialization code. This function will be started in kernel mode one time while the
 * VEX Cortex is starting up. As the scheduler is still paused, most API functions will fail.
 *
 * The purpose of this function is solely to set the default pin modes (pinMode()) and port
 * states (digitalWrite()) of limit switches, push buttons, and solenoids. It can also safely
 * configure a UART port (usartOpen()) but cannot set up an LCD (lcdInit()).
 */
void initializeIO() {
    digitalWrite(SOL, LOW);
    pinMode(SOL, OUTPUT);
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

void initialize() {
  lcdInit(uart1);
  lcdClear(uart1);
  taskCreate(LCD,TASK_DEFAULT_STACK_SIZE,NULL,TASK_PRIORITY_DEFAULT);
	crabInit();
}
