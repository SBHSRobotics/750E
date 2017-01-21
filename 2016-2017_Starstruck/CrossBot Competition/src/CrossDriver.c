/*
 * SwerveLib.c
 *
 *  Created on: Jul 24, 2016
 *      Author: Mikel Matticoli
 */

#include <main.h>

void lcdLoop();

ServoSystem drive;
ServoSystem leftPincer;
ServoSystem rightPincer;
int newTarget;

void driveInit() {
	drive = servoInit(ROT_POT, R, false, PID_MOTOR_SCALE, PID_THRESH);
	leftPincer = servoInit(PL_POT, PL, true, 80, 300);
	rightPincer = servoInit(PR_POT, PR, false, 80, 300);
	delay(1000);
	//printf("RET Pot: %p\t%d\n\r",drive.targetValue, *drive.targetValue);
	delay(1000);
	//printf("Target: %p\t%d\n\r", drive.targetValue, *drive.targetValue);
	driveSetPos(0);
	delay(1000);
	//printf("Target: %p\t%d\n\r", drive.targetValue, *drive.targetValue);
	delay(1000);
}

void driveSetPos(int targetVal) {
	servoSet(drive, targetVal);
	// printf("drivepos at %p set to %d is now %d\n\r", drive.targetValue, targetVal, *drive.targetValue);
}

void driveForward(int speed) {
	motorSet(WF, speed);
	motorSet(WL, speed);
	motorSet(WR, -speed);
	motorSet(WB, -speed);
}

void pointTurn(int speed){ //potentiometer values: LEFT: 3550 FORWARD: 2050 RIGHT: 550
	int pot = analogRead(drive.potentiometerPort);

	if(abs(pot-2050)<PID_THRESH){ //straight forward
		motorSet(WL,motorGet(WL)+speed);
		motorSet(WR,motorGet(WR)+speed);
	} else if (abs(pot-3550)<PID_THRESH){ //straight left
		motorSet(WF,motorGet(WF)-speed);
		motorSet(WB,motorGet(WB)-speed);
	} else if (abs(pot-550)<PID_THRESH){ //straight right
		motorSet(WF,motorGet(WF)+speed);
		motorSet(WB,motorGet(WB)+speed);
	} else if((pot<3550-PID_THRESH)&&(pot>2050+PID_THRESH)){ //between forward and left
		motorSet(WF,motorGet(WF)-speed);
		motorSet(WL,motorGet(WL)+speed);
		motorSet(WB,motorGet(WB)-speed);
		motorSet(WR,motorGet(WR)+speed);
	} else if((pot<2050-PID_THRESH)&&(pot>550+PID_THRESH)){ //between forward and right
		motorSet(WF,motorGet(WF)+speed);
		motorSet(WL,motorGet(WL)+speed);
		motorSet(WB,motorGet(WB)+speed);
		motorSet(WR,motorGet(WR)+speed);
	} else {
		motorStop(WF);
		motorStop(WB);
		motorStop(WL);
		motorStop(WR);
	}
}

// LIFT
void lift(int speed) {
	motorSet(LIFT_AB, -speed); // Clockwise is down, counterclockwise is up
	motorSet(LIFT_CD, -speed);
	motorSet(LIFT_E, -speed);
}

// PINCER
void pince(int target) {
	newTarget = servoGetTarget(leftPincer)+target;

	if(newTarget>2400){ // just past parallel
		newTarget = 2400;
	} else if (newTarget<200){
		newTarget = 200;
	}
	printf("newTarget: %d\tJS: %d\n",newTarget,joystickGetAnalog(1,1)); //TODO: uncomment all the print statements
	printf("Left Pot: %d\tRight Pot: %d\n",analogRead(PL_POT),analogRead(PR_POT));

	servoSet(leftPincer,newTarget);
	servoSet(rightPincer,newTarget);

	printf("Left Motor: %d\tRight Motor: %d\n\n",motorGet(PL),motorGet(PR));
}

// LCD
int count = 0; // keeps track of position in main LCD menu
int manualCount = 0; // keeps track of position in manual test LCD submenu

void pulseMotor(unsigned char x) {
   motorSet(x,127);
   delay(250);
   motorSet(x,-127);
   delay(250);
   motorSet(x,0);
 }

 void lcdStart() {
	 taskCreate(lcdLoop,TASK_DEFAULT_STACK_SIZE,NULL,TASK_PRIORITY_DEFAULT);
 }

void lcdLoop() {
  lcdSetBacklight(uart1, true);
  switch (count) {
    case POT_MENU:
      lcdSetText(uart1,1, " Potentiometers ");
      lcdSetText(uart1,2, "<    Select    >");
      if (lcdReadButtons(uart1)==LCD_BTN_LEFT) {
        count=MEMES_MENU;
      } else if (lcdReadButtons(uart1)==LCD_BTN_RIGHT) {
        count++;
      }
      else if(lcdReadButtons(uart1)==LCD_BTN_CENTER) {
        count = POT_VAL;
      }
      break;
		case BATTERY_MENU:
      lcdSetText(uart1,1,"Battery  Voltage");
      lcdSetText(uart1,2,"<    Select    >");
      if (lcdReadButtons(uart1)==LCD_BTN_LEFT) {
        count--;
      }
      else if (lcdReadButtons(uart1)==LCD_BTN_RIGHT) {
        count++;
      }
      else if(lcdReadButtons(uart1)==LCD_BTN_CENTER) {
        count=BATTERY_VAL;
      }
      break;
		case AUTON_MENU:
      lcdSetText(uart1,1," Run Autonomous ");
      lcdSetText(uart1,2,"<    Select    >");
			if (lcdReadButtons(uart1)==LCD_BTN_LEFT) {
        count--;
      }
      else if (lcdReadButtons(uart1)==LCD_BTN_RIGHT) {
        count++;
      }
      else if(lcdReadButtons(uart1)==LCD_BTN_CENTER) {
        count=AUTON_VAL;
      }
			break;
    case SELF_MENU:
      lcdSetText(uart1,1,"   Self Test    ");
      lcdSetText(uart1,2,"<    Select    >");
      if (lcdReadButtons(uart1)==LCD_BTN_LEFT) {
        count--;
      }
      else if (lcdReadButtons(uart1)==LCD_BTN_RIGHT) {
        count++;
      }
      else if(lcdReadButtons(uart1)==LCD_BTN_CENTER) {
        count=SELF_VAL;
      }
      break;
    case MANUAL_MENU:
      lcdSetText(uart1,1,"  Manual Test   ");
      lcdSetText(uart1,2,"<    Select    >");
      if (lcdReadButtons(uart1)==LCD_BTN_LEFT) {
        count--;
      }
      else if (lcdReadButtons(uart1)==LCD_BTN_RIGHT) {
        count++;
      }
      else if(lcdReadButtons(uart1)==LCD_BTN_CENTER) {
        count=MANUAL_VAL;
      }
      break;
    case MEMES_MENU:
      lcdSetText(uart1,1,"   Dank Memes   ");
      lcdSetText(uart1,2,"<    Select    >");
      if (lcdReadButtons(uart1)==LCD_BTN_LEFT) {
        count--;
      }
      else if (lcdReadButtons(uart1)==LCD_BTN_RIGHT) {
        count=POT_MENU;
      }
      else if(lcdReadButtons(uart1)==LCD_BTN_CENTER) {
        count=MEMES_VAL;
      }
      break;
    case 	POT_VAL: // potentiometer readings
			lcdClear(uart1);
      lcdPrint(uart1,1,"Drive: %d", analogRead(ROT_POT));
      if(lcdReadButtons(uart1)==LCD_BTN_CENTER)
        count = POT_MENU; //go back to select menu
      break;
		case BATTERY_VAL: // get battery voltage
		  lcdPrint(uart1, 1, "Main: %dmV",powerLevelMain()); //Display main battery on LCD
		  lcdPrint(uart1, 2, "Expander: %dmV",(int)(analogRead(EXPANDER)*35.84)); //Display power expander battery on LCD
      if(lcdReadButtons(uart1)==LCD_BTN_CENTER) {
        count=BATTERY_MENU;
      }
      break;
		case AUTON_VAL:
			lcdPrint(uart1, 1, " Running Auton  ");
			lcdPrint(uart1, 2, "Don't Touch!!!!!");
			autonomous();
			count=AUTON_MENU;
			break;
    case SELF_VAL: // self test
      lcdClear(uart1);
      lcdPrint(uart1, 2, "Don't Touch!!!!!");
      for(unsigned char x=1;x<=10;x++) {
        lcdPrint(uart1, 1, "Motor %d",x);
        pulseMotor(x);
      }
			delay(50);
      count=SELF_MENU;
      break;
    case MANUAL_VAL: //manual single motor test
      lcdClear(uart1);
      lcdPrint(uart1,1,"Choose Motor");
      printf("%d",lcdReadButtons(uart1));
      switch(manualCount) {
        case 0:
          lcdPrint(uart1,2,"<    Motor 1   >");
          if(lcdReadButtons(uart1)==LCD_BTN_LEFT) {
            manualCount=10;
            printf("reached");
          } else if(lcdReadButtons(uart1)==LCD_BTN_RIGHT) {
            manualCount++;
            printf("reached");
            printf("%d",manualCount);
          } else if(lcdReadButtons(uart1)==LCD_BTN_CENTER) {
            pulseMotor(manualCount+1);
          }
          break;
        case 1:
          lcdPrint(uart1,2,"<    Motor 2   >");
          if(lcdReadButtons(uart1)==LCD_BTN_LEFT) {
            manualCount--;
          } else if(lcdReadButtons(uart1)==LCD_BTN_RIGHT) {
            manualCount++;
          } else if(lcdReadButtons(uart1)==LCD_BTN_CENTER) {
            pulseMotor(manualCount+1);
          }
          break;
        case 2:
          lcdPrint(uart1,2,"<    Motor 3   >");
          if(lcdReadButtons(uart1)==LCD_BTN_LEFT) {
            manualCount--;
          } else if(lcdReadButtons(uart1)==LCD_BTN_RIGHT) {
            manualCount++;
          } else if(lcdReadButtons(uart1)==LCD_BTN_CENTER) {
            pulseMotor(manualCount+1);
          }
          break;
        case 3:
          lcdPrint(uart1,2,"<    Motor 4   >");
          if(lcdReadButtons(uart1)==LCD_BTN_LEFT) {
            manualCount--;
          } else if(lcdReadButtons(uart1)==LCD_BTN_RIGHT) {
            manualCount++;
          } else if(lcdReadButtons(uart1)==LCD_BTN_CENTER) {
            pulseMotor(manualCount+1);
          }
          break;
        case 4:
          lcdPrint(uart1,2,"<    Motor 5   >");
          if(lcdReadButtons(uart1)==LCD_BTN_LEFT) {
            manualCount--;
          } else if(lcdReadButtons(uart1)==LCD_BTN_RIGHT) {
            manualCount++;
          } else if(lcdReadButtons(uart1)==LCD_BTN_CENTER) {
            pulseMotor(manualCount+1);
          }
          break;
        case 5:
          lcdPrint(uart1,2,"<    Motor 6   >");
          if(lcdReadButtons(uart1)==LCD_BTN_LEFT) {
            manualCount--;
          } else if(lcdReadButtons(uart1)==LCD_BTN_RIGHT) {
            manualCount++;
          } else if(lcdReadButtons(uart1)==LCD_BTN_CENTER) {
            pulseMotor(manualCount+1);
          }
          break;
        case 6:
          lcdPrint(uart1,2,"<    Motor 7   >");
          if(lcdReadButtons(uart1)==LCD_BTN_LEFT) {
            manualCount--;
          } else if(lcdReadButtons(uart1)==LCD_BTN_RIGHT) {
            manualCount++;
          } else if(lcdReadButtons(uart1)==LCD_BTN_CENTER) {
            pulseMotor(manualCount+1);
          }
          break;
        case 7:
          lcdPrint(uart1,2,"<    Motor 8   >");
          if(lcdReadButtons(uart1)==LCD_BTN_LEFT) {
            manualCount--;
          } else if(lcdReadButtons(uart1)==LCD_BTN_RIGHT) {
            manualCount++;
          } else if(lcdReadButtons(uart1)==LCD_BTN_CENTER) {
            pulseMotor(manualCount+1);
          }
          break;
        case 8:
          lcdPrint(uart1,2,"<    Motor 9   >");
          if(lcdReadButtons(uart1)==LCD_BTN_LEFT) {
            manualCount--;
          } else if(lcdReadButtons(uart1)==LCD_BTN_RIGHT) {
            manualCount++;
          } else if(lcdReadButtons(uart1)==LCD_BTN_CENTER) {
            pulseMotor(manualCount+1);
          }
          break;
        case 9:
          lcdPrint(uart1,2,"<   Motor 10   >");
          if(lcdReadButtons(uart1)==LCD_BTN_LEFT) {
            manualCount--;
          } else if(lcdReadButtons(uart1)==LCD_BTN_RIGHT) {
            manualCount++;
          } else if(lcdReadButtons(uart1)==LCD_BTN_CENTER) {
            pulseMotor(manualCount+1);
          }
          break;
        case 10:
          lcdPrint(uart1,2,"<     Exit     >");
          if(lcdReadButtons(uart1)==LCD_BTN_LEFT) {
            manualCount--;
          } else if(lcdReadButtons(uart1)==LCD_BTN_RIGHT) {
            manualCount=0;
          } else if(lcdReadButtons(uart1)==LCD_BTN_CENTER) {
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
