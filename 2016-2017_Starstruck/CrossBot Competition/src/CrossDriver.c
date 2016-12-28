/*
 * SwerveLib.c
 *
 *  Created on: Jul 24, 2016
 *      Author: Mikel Matticoli
 */

#include <main.h>
#include <time.h>

ServoSystem drive;

void driveInit() {
	drive = servoInit(RP, R, false, PID_MOTOR_SCALE, PID_THRESH);
	driveSet(DRIVE_START);
}

void driveSet(int targetVal) {
	servoSet(drive, targetVal);
}

// LCD
int count = 0; //keeps track of position in main LCD menu
int manualCount = 0; //kepps track of position in manual test LCD submenu

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
      lcdPrint(uart1,1,"Drive: %d", analogRead(RP));
      if(lcdReadButtons(uart1)==LCD_BTN_CENTER)
        count = POT_MENU; //go back to select menu
      break;
    case DRIVE_VAL: //get current drive mode
      lcdPrint(uart1,1,"Drive Mode:");
      lcdPrint(uart1,2,"Function Disabled");

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
