/*
 * SwerveLib.c
 *
 *  Created on: Jul 24, 2016
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

// LCD
int count = 0; // keeps track of position in main LCD menu
int manualCount = 0; // keeps track of position in manual test LCD submenu
int memesCount = 0;



// Private Global Constants
static const double Kp=1.0,
			 	 	Ki=1.0;

// Private types
typedef struct CrabGroup {
	TaskHandle thread;
	int errSum;
	double timeSum;
}CrabGroup;	// Defines a wheel or group of wheels controlled by a motor/potentiometer pair

// Private Global variable declarations
CrabGroup leftWheel;
CrabGroup rightWheel;

// Private function declarations
/**/
void crabPID(unsigned char motor, int currentValue, int targetValue, CrabGroup group);
void pincerPID(unsigned char motor, int currentValue, int targetValue);

// Public function definitions
void crabInit() {
	void leftSidePID() {
		while(1) {
			crabPID(LR, analogRead(LP), currentConfig.leftWheel, leftWheel);
			delay(200);
		}
	}

	void rightSidePID() {
		while(1) {
			crabPID(RR, analogRead(RP), currentConfig.rightWheel, rightWheel);
			delay(200);
		}
	}
	#if (DEBUG_MODE == 0 || DEBUG_MODE == 2)
		setDriveConfig(DEFAULT_DRIVE_MODE);
		leftWheel.thread = taskCreate(leftSidePID,TASK_DEFAULT_STACK_SIZE,NULL,TASK_PRIORITY_DEFAULT);
		rightWheel.thread = taskCreate(rightSidePID,TASK_DEFAULT_STACK_SIZE,NULL,TASK_PRIORITY_DEFAULT);
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
						setDriveConfigById(confOverride);
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
	taskDelete(leftWheel.thread);
	taskDelete(rightWheel.thread);
}

void setDriveConfig(DriveConfiguration config) {
	currentConfig = config;
	leftWheel.errSum = 0;
	rightWheel.errSum = 0;
}

void setDriveConfigById(int id) {
	switch(id) {
		case HOLONOMIC_DRIVE:
			setDriveConfig(holonomicDrive);
			break;
		case TANK_DRIVE:
			setDriveConfig(tankDrive);
			break;
		case SHUFFLE_DRIVE:
			setDriveConfig(shuffleDrive);
			break;
		default:
			setDriveConfig(currentConfig);
			break;
	}
}

// Private function definitions

void crabPID(unsigned char motor, int currentValue, int targetValue, CrabGroup group) {
	// TODO: Integral ~ motors still oscillate sporadically

	// Do not run motors if they have reached the target position
	if (abs(targetValue-currentValue)<PID_THRESH) {
		motorStop(motor);
		return;
	}

	// Proportional <calculate error>
	double err = (double)Kp*((double)targetValue - (double)currentValue);

	//Integral
//	group.errSum += err;
//	group.prevIter = time(NULL);

	//Convert Target Value to Motor Speed
	int speed = (int)(((double)err / ((double)4095) * (double)PID_MOTOR_SCALE) );

	// Shift speed scale past deadzone
	speed +=
		( (speed > 0)
			? (127 - PID_MOTOR_SCALE)
			: (-127 + PID_MOTOR_SCALE)
		 );
	// motorSet(motor, -speed);
}

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
    case AUTON_MENU: // runs auton
      lcdSetText(uart1,1,"    Run Auton   ");
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
    case AUTON_VAL: // run auton !!
      //currentConfig = holonomicDrive;
      lcdClear(uart1);
			lcdPrint(uart1, 1, "Press Center to ");
      lcdPrint(uart1, 2, "!!!End Auton!!! ");
			autonomous();
			delay(50);
      count=AUTON_MENU;
      break;
    case MEMES_VAL:
			lcdPrint(uart1,1,"  Hall of Memes ");
			int memesMaximum = 4;
			switch(memesCount) {
				case 0:
					lcdPrint(uart1,2,"<     Fork     >");
					if(lcdReadButtons(uart1)==LCD_BTN_LEFT) {
						memesCount=memesMaximum;
						printf("reached");
					} else if(lcdReadButtons(uart1)==LCD_BTN_RIGHT) {
						memesCount++;
						printf("reached");
						printf("%d",manualCount);
					} else if(lcdReadButtons(uart1)==LCD_BTN_CENTER) {
						pulseMotor(manualCount+1);
					}
					break;
				case 1:
					lcdPrint(uart1,2,"<     Damb     >");
					if(lcdReadButtons(uart1)==LCD_BTN_LEFT) {
						memesCount--;
					} else if(lcdReadButtons(uart1)==LCD_BTN_RIGHT) {
						memesCount++;
					}
					break;
				case 2:
					lcdPrint(uart1,2,"<What the Sus? >");
					if(lcdReadButtons(uart1)==LCD_BTN_LEFT) {
						memesCount--;
					} else if(lcdReadButtons(uart1)==LCD_BTN_RIGHT) {
						memesCount++;
					}
					break;

				case 3:
					lcdPrint(uart1,2,"<  >");
					if(lcdReadButtons(uart1)==LCD_BTN_LEFT) {
						memesCount--;
					} else if(lcdReadButtons(uart1)==LCD_BTN_RIGHT) {
						memesCount++;
					}
					break;
				case 4:
					lcdPrint(uart1,2,"Green Meme:Greme");
					if(lcdReadButtons(uart1)==LCD_BTN_LEFT) {
						memesCount--;
					} else if(lcdReadButtons(uart1)==LCD_BTN_RIGHT) {
						memesCount=0;
					}
					break;
				default:
					break;
				}
      break;
    default: count=0;
      break;
  }
}
