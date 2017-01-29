/*
 * Eugene.c
 *
 *  Created on: Dec 3, 2016
 *      Author: Mikel Matticoli
 */

#include <main.h>
#include <time.h>

// Define drive configurations (Must be tuned based on potentiometer values at a given position)
DriveConfiguration holonomicDrive = {HOLONOMIC_DRIVE, 2750, 2750};
DriveConfiguration tankDrive = {TANK_DRIVE, 3400, 2200};
DriveConfiguration shuffleDrive = {SHUFFLE_DRIVE, 1800, 3900};

DriveConfiguration currentConfig;

bool isCrabKilled = false;

// Private Global variable declarations
// ServoSystem crabLeftServo;
// ServoSystem crabRightServo;

//LCD
int count = 0;
int manualCount = 0;
// Public function definitions
void crabInit(){
	unsigned char leftArr[4] = {LR, 0, 0, 0};
	unsigned char rightArr[4] = {RR, 0, 0, 0};
	bool boolArr[4] = {false, false, false, false};
	// crabLeftServo = servoInit(LP, leftArr, boolArr, PID_MOTOR_SCALE, PID_THRESH);
	// crabRightServo = servoInit(RP, rightArr, boolArr, PID_MOTOR_SCALE, PID_THRESH);

	#if (DEBUG_MODE == 0 || DEBUG_MODE == 2)
		setDriveConfig(DEFAULT_DRIVE_MODE);
	#endif

	#if (DEBUG_MODE == 1 || DEBUG_MODE == 2)
		void debug() {
			int confOverride = -1;
			while(1) {
				printf("%c[2J", (char)27); // Clear Console
				printf("=SWERVE DEBUG=\n\rLeft Potentiometer:\t%d\n\rRight Potentiometer:\t%d\n\r Config: %d,%d\n\r%d",
						analogRead(LP), analogRead(RP), currentConfig.leftWheel, currentConfig.rightWheel,confOverride);

				if( fcount(stdin) > 0) {
					confOverride = ((int)getchar())-48;
					if(confOverride >=0 ) {
						// setDriveConfigById(confOverride);
					}
				} else {
					confOverride = -1;
				}

				delay(100);
			}
		}
		taskCreate(debug,TASK_DEFAULT_STACK_SIZE,NULL,TASK_PRIORITY_DEFAULT);
	#endif
}

void crabKill() {
	// taskDelete(crabRightServo.task);
	// taskDelete(crabLeftServo.task);
}

void setDriveConfig(DriveConfiguration config) {
	currentConfig = config;
	// crabLeftServo.targetValue = config.leftWheel;
	// crabRightServo.targetValue = config.rightWheel;
}

// void setDriveConfigById(int id) {
// 	switch(id) {
// 		case HOLONOMIC_DRIVE:
// 			setDriveConfig(holonomicDrive);
// 			break;
// 		case TANK_DRIVE:
// 			setDriveConfig(tankDrive);
// 			break;
// 		case SHUFFLE_DRIVE:
// 			setDriveConfig(shuffleDrive);
// 			break;
// 		default:
// 			setDriveConfig(currentConfig);
// 			break;
// 	}

	void pulseMotor(unsigned char x) {
	   motorSet(x,127);
	   delay(250);
	   motorSet(x,-127);
	   delay(250);
	   motorSet(x,0);
	 }

	void LCD() {
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
	    case DRIVE_MENU:
	      lcdSetText(uart1,1,"   Drive Mode   ");
	      lcdSetText(uart1,2,"<    Select    >");
	      if (lcdReadButtons(uart1)==LCD_BTN_LEFT) {
	        count--;
	      }
	      else if (lcdReadButtons(uart1)==LCD_BTN_RIGHT) {
	        count++;
	      }
	      else if(lcdReadButtons(uart1)==LCD_BTN_CENTER) {
	        count=DRIVE_VAL;
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
	    case 	POT_VAL: //potentiometer readings
	      lcdPrint(uart1,1,"Left: %d", analogRead(LP));
	      lcdPrint(uart1,2,"Right: %d", analogRead(RP));
	      if(lcdReadButtons(uart1)==LCD_BTN_CENTER)
	        count = POT_MENU; //go back to select menu
	      break;
	    case DRIVE_VAL: //get current drive mode
	      lcdPrint(uart1,1,"Drive Mode");
	      printf("%d",currentConfig.id);
	      switch(currentConfig.id) {
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
	      if(lcdReadButtons(uart1)==LCD_BTN_CENTER) {
	        count=DRIVE_MENU;
	      }
	      break;
			case BATTERY_VAL: //get battery voltage
			  lcdPrint(uart1, 1, "Main: %dmV",powerLevelMain()); //Display main battery on LCD
			  lcdPrint(uart1, 2, "Expander: %dmV",(int)(analogRead(EXPANDER)*35.84)); //Display power expander battery on LCD
	      if(lcdReadButtons(uart1)==LCD_BTN_CENTER) {
	        count=BATTERY_MENU;
	      }
	      break;
	    case SELF_VAL: //self test
	      //currentConfig = holonomicDrive;
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
