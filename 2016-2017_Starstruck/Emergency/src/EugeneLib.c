/*
 * EugeneLib.c
 *
 *  Created on: Feb 4, 2017
 *      Author: Kirsten Meidlinger
 */

 #include <main.h>

 /*
  * This is the file for abstracting Eugene's basic functions.
  * This file should be placed in the src directory. Please note
  * that this file will not compile if EugeneLib.h has not been
  * included properly.
  */

// Map functions
	void joystickMap(){
    // Get drive input
		driveOp();

    // Get pincer input
    if(joystickGetDigital(2,5,JOY_UP)){
      // If 5U is held on the partner joystick, the pincers can be individually moved to sync them
      // TODO: mechanically sync pincers
		  if(abs(joystickGetAnalog(2,4))>15){
  			motorSet(PL,joystickGetAnalog(2,4));
  		} else {
  			motorStop(PL);
  		}
  		if(abs(joystickGetAnalog(2,1))>15){
  			motorSet(PR,joystickGetAnalog(2,1));
  		} else {
  			motorStop(PR);
  		}
    } else {
      // Moves pincers normally
      if(abs(joystickGetAnalog(2,1))>15){
    		pince(joystickGetAnalog(2,1));
      } else {
        pince(0);
      }
    }

    // Get lift input
    if(abs(joystickGetAnalog(2,3))>15){
  		lift(joystickGetAnalog(2,3));
    } else {
      lift(0);
    }
    if(joystickGetDigital(2,7,JOY_UP)){
      motorSet(CD,127);//-
    }
    if(joystickGetDigital(2,7,JOY_LEFT)){
      motorSet(AB,127);//?
    }
    if(joystickGetDigital(2,7,JOY_RIGHT)){
      motorSet(E,127);
    }
    if(joystickGetDigital(2,7,JOY_DOWN)){
      motorSet(F,127);//-?
    }

    if(joystickGetDigital(2,8,JOY_UP)){
      motorSet(CD,-127);
    }
    if(joystickGetDigital(2,8,JOY_LEFT)){
      motorSet(AB,-127);
    }
    if(joystickGetDigital(2,8,JOY_RIGHT)){
      motorSet(E,-127);
    }
    if(joystickGetDigital(2,8,JOY_DOWN)){
      motorSet(F,-127);
    }
	}

// Robot functions
  void lift(int speed){
    motorSet(AB,-speed);
    motorSet(CD,speed);
    motorSet(E,-speed);
    motorSet(F,speed);
  }

  void pince(int speed){
    motorSet(PL,speed);
    motorSet(PR,speed);
  }

  void driveAuton(int frontLeftSpeed, int backLeftSpeed, int frontRightSpeed, int backRightSpeed){
    motorSet(LF,frontLeftSpeed);
    motorSet(LB,backLeftSpeed);
    motorSet(RF,frontRightSpeed);
    motorSet(RB,backRightSpeed);
  }

  void driveOp(){
    // Begin holo code
    int thresh = 20;
    int ch1 = joystickGetAnalog(1, 1);
    int ch2 = joystickGetAnalog(1, 2);
    int ch3 = joystickGetAnalog(1, 3);
    int ch4 = joystickGetAnalog(1, 4);

    printf("ch1: %d\t ch2: %d\t ch3: %d\t ch4: %d\n",ch1,ch2,ch3,ch4);

    // if ((abs(ch3) > thresh) || (abs(ch4) > thresh) || (abs(ch2) > thresh) || (abs(ch1) > thresh)) {
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

      motorSet(LB, -ch3 + ch2 - ch1 + ch4);
      motorSet(LF, ch3 + ch2 + ch4 + ch1);
      motorSet(RB, -(ch3 + ch2 - ch1 + ch4));
      motorSet(RF, -(ch3 + ch2 - ch4 - ch1));
      motorSet(CL, (ch3 + ch2 + ch1));
      motorSet(CR, (ch3 + ch2 + ch1));

    // } else {
    //   stopDrive();
    // }
    printf("LF: %d\t LB: %d\t RF: %d\t RB: %d\n",motorGet(LF),motorGet(LB),motorGet(RF),motorGet(RB));
    // End holo code
  }

  void stopDrive(){
    motorStop(LB);
    motorStop(LF);
    motorStop(RB);
    motorStop(RF);
    motorStop(CL);
    motorStop(CR);
  }

// LCD
  int count = 0; // keeps track of position in main LCD menu
  int manualCount = 0; // keeps track of position in manual test LCD submenu
  bool auton = false;

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
 		case BATTERY_MENU:
       lcdSetText(uart1,1,"Battery  Voltage");
       lcdSetText(uart1,2,"<    Select    >");
       if (lcdReadButtons(uart1)==LCD_BTN_LEFT) {
         count=MEMES_MENU;
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
         count=BATTERY_MENU;
       }
       else if(lcdReadButtons(uart1)==LCD_BTN_CENTER) {
         count=MEMES_VAL;
       }
       break;
 		case BATTERY_VAL: // get battery voltage
 		  lcdPrint(uart1, 1, "Main: %dmV",powerLevelMain()); //Display main battery on LCD
 		  //lcdPrint(uart1, 2, "Expander: %dmV",(int)(analogRead(EXPANDER)*35.84)); //Display power expander battery on LCD
       if(lcdReadButtons(uart1)==LCD_BTN_CENTER) {
         count=BATTERY_MENU;
       }
       break;
 		case AUTON_VAL:
      lcdPrint(uart1, 1, "  Are you sure? ");
      lcdPrint(uart1, 2, " No         Yes ");
      if(lcdReadButtons(uart1) == LCD_BTN_LEFT){
        count = AUTON_MENU;
      } else if(lcdReadButtons(uart1) == LCD_BTN_RIGHT) {
        auton = true;
      }
      if(auton == true){
   			lcdPrint(uart1, 1, " Running Auton  ");
   			lcdPrint(uart1, 2, "Don't Touch!!!!!");
   			autonomous();
   			count=AUTON_MENU;
      }
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
       lcdClear(uart1);
       lcdPrint(uart1, 1, "  Coming  Soon  ");
       lcdPrint(uart1, 2, "<     Exit     >");
       if(lcdReadButtons(uart1)==LCD_BTN_CENTER){
         count=MEMES_MENU;
       }
       break;
     default: count=0;
       break;
   }
 }
