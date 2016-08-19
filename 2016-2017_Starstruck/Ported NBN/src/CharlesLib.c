
//========================================================//
//     Robot Library   (NOTE: Will not compile directly)  //
//        Anything robot/wiring dependant goes here       //
//========================================================//

#include "main.h"

const unsigned char EXPANDER = 3;
//LCD Menu

//Active Menu
int menu = MENU_MAIN;
//Active Menu Option
int option = 1;

void joystickMap() {
  joyRefreshAnalog();

  if(!joystickGetDigital(1,5,JOY_DOWN) && !joystickGetDigital(1,5,JOY_UP)) {
  	//joyMapMain();
  }
  //buttonMapMain();

  //joyMapPartner();
  //buttonMapPartner();
}

//=== Utility Functions ===//

/* Uses PID loop with front Ultrasonic sensor to lock in at a distance from the pole */
void driveSetByDistance(int ultrasonicDistance) {
	int AUTON_PID_THRESH = 5;
	float err = ultrasonicDistance-ultrasonicGet(Sonar);

	if(abs(err)<=AUTON_PID_THRESH){
				//driveSet(); these functions don't exist anymore since they aren't relevant to starstruck
				//beep();
			} else {
				float K = 25;
				err=(err>0)?-K-err:K-err;
				//driveSet(err, err, 0);
			}
}

//=== LCD Debugging ===//

/* Display Main and Power Expander Battery Voltages on LCD */
void lcdDisplayVoltage() {
  lcdClear(uart1);
  char* s = "Main: ";
  sprintf(s,"%dmV",powerLevelMain()); //Format LCD line 1
  lcdPrint(0, 0, s); //Display main battery on LCD

  s= "Expander: ";
  sprintf(s,"%dmV",(int)(analogRead(EXPANDER)*21.93)); //Format LCD line 1
  lcdPrint(1, 0, s); //Display power expander battery on LCD
  wait(500);
}

/* LCD Debugging Menu - Runs asynchronously
WARNING: WHEN RUNNING POST ASYNCHRONOUSLY, DO NOT ATTEMPT JOYSTICK CONTROL */
void lcdMenu() {
  while(true) {
    wait(50);
    lcdClear(uart1);

    if(lcdReadButtons(uart1) == LCD_BTN_LEFT) {
      option++;
      wait(100);
    } else if (lcdReadButtons(uart1) == LCD_BTN_RIGHT) {
      option--;
      wait(100);

    }

    //Show options based on active menu
    switch(menu) {
      case MENU_MAIN:

        lcdSetText(uart1,2,"Main Menu");

        switch(option) {

          case 0:
            option=6;
            break;

          case 1:
            lcdSetText(uart1,2,"< BATTERY LVL  >");
            if(lcdReadButtons(uart1)==LCD_BTN_CENTER) {
              menu=MENU_BATT;
              option=1;
              wait(500);
            }
            break;

          case 2:
            lcdSetText(uart1,1,"<  SELF-TEST   >");
              if(lcdReadButtons(uart1)==LCD_BTN_CENTER) {
                powerOnSelfTest();
              }
              break;

          case 3:
            lcdSetText(uart1,1,"< MANUAL TEST  >");
              if(lcdReadButtons(uart1)==LCD_BTN_CENTER) {
                menu=MENU_MST;
                option=1;
                wait(500);

              }
              break;
          case 4:
            lcdSetText(uart1,1,"< PLAY MUSIC  >");
              if(lcdReadButtons(uart1)==LCD_BTN_CENTER) {
                menu=MENU_MEMES;
                option=1;
                wait(500);

              }
              break;
          case 5:
            lcdSetText(uart1,1,"< SHOOTER SPD  >");
            if(lcdReadButtons(uart1)==LCD_BTN_CENTER) {
              menu=MENU_SHOOTER;
              option=1;
              wait(500);
            }
            break;
          case 6:
            lcdSetText(uart1,1,"<   POT  SPD   >");
            if(lcdReadButtons(uart1)==LCD_BTN_CENTER) {
              menu=MENU_POT;
              option=1;
              wait(500);
            }
            break;

          case 7:
            option = 1;
            break;

          default:
            lcdSetText(uart1,1,"<    ERROR     >");
            break;

        }
        break;

      case MENU_BATT:
        lcdDisplayVoltage();
        if(lcdReadButtons(uart1) == LCD_BTN_LEFT) {
          menu=MENU_MAIN;
          option=1;
          wait(500);
        }
        break;

      case MENU_SHOOTER:
        //lcdDisplayShooter();
        if(lcdReadButtons(uart1) == LCD_BTN_LEFT) {
          menu=MENU_MAIN;
          option=1;
          wait(500);
        }
        break;

      case MENU_MST:
        lcdPrint(0,0,"  Select Motor  ");
        switch(option) {

          case 0:
            option=11;
            break;

          case 1:
            lcdPrint(1,0,"<    Port 1    >");
            if(lcdReadButtons(uart1)==LCD_BTN_CENTER) {
              pulseMotor(1);
            }
            break;

          case 2:
            lcdPrint(1,0,"<    Port 2    >");
            if(lcdReadButtons(uart1)==LCD_BTN_CENTER) {
              pulseMotor(2);
            }
            break;

          case 3:
            lcdPrint(1,0,"<    Port 3    >");
            if(lcdReadButtons(uart1)==LCD_BTN_CENTER) {
              pulseMotor(3);
            }
            break;

          case 4:
            lcdPrint(1,0,"<    Port 4    >");
            if(lcdReadButtons(uart1)==LCD_BTN_CENTER) {
              pulseMotor(4);
            }
            break;

          case 5:
            lcdPrint(1,0,"<    Port 5    >");
            if(lcdReadButtons(uart1)==LCD_BTN_CENTER) {
              pulseMotor(5);
            }
            break;

          case 6:
            lcdPrint(1,0,"<    Port 6    >");
            if(lcdReadButtons(uart1)==LCD_BTN_CENTER) {
              pulseMotor(6);
            }
            break;

          case 7:
            lcdPrint(1,0,"<    Port 7    >");
            if(lcdReadButtons(uart1)==LCD_BTN_CENTER) {
              pulseMotor(7);
            }
            break;

          case 8:
            lcdPrint(1,0,"<    Port 8    >");
            if(lcdReadButtons(uart1)==LCD_BTN_CENTER) {
              pulseMotor(8);
            }
            break;

          case 9:
            lcdPrint(1,0,"<    Port 9    >");
            if(lcdReadButtons(uart1)==LCD_BTN_CENTER) {
              pulseMotor(9);
            }
            break;

          case 10:
            lcdPrint(1,0,"<    Port 10   >");
            if(lcdReadButtons(uart1)==LCD_BTN_CENTER) {
              pulseMotor(10);
            }
            break;

          case 11:
            lcdPrint(1,0,"<     Back    >");
            if(lcdReadButtons(uart1)==LCD_BTN_CENTER) {
              menu=MENU_MAIN;
              option=1;
              wait(1000);
            }
            break;

          case 12:
            option = 1;
            break;

          default:
            lcdPrint(1,0,"<    ERROR     >");
            break;
          }
          break;

      case MENU_MEMES:
        switch(option) {

          case 0:
            option=8;
            break;

          case 1:
            lcdPrint(1,0,"<     Beep     >");
            if(lcdReadButtons(uart1)==LCD_BTN_CENTER) {
              //beep();
            }
            break;

          case 2:
            lcdPrint(1,0,"<     Boop     >");
              if(lcdReadButtons(uart1)==LCD_BTN_CENTER) {
                //boop();
              }
              break;

          case 3:
            lcdPrint(1,0,"<    Dhoom     >");
              if(lcdReadButtons(uart1)==LCD_BTN_CENTER) {
                //StartTask(Dhoom);
              }
              break;

          case 4:
            lcdPrint(1,0,"<  Take On Me  >");
              if(lcdReadButtons(uart1)==LCD_BTN_CENTER) {
                //StartTask(TakeOnMe);
              }
              break;

          case 5:
            lcdPrint(1,0,"<  BigBoyRayC  >");
              if(lcdReadButtons(uart1)==LCD_BTN_CENTER) {
                //StartTask(PinkPanther);
              }
              break;

          case 6:
            lcdPrint(1,0,"< HotlineBling >");
              if(lcdReadButtons(uart1)==LCD_BTN_CENTER) {
                //StartTask(HotlineBling);
              }
              break;
          case 7:
            lcdPrint(1,0,"<  John  Cena  >");
              if(lcdReadButtons(uart1)==LCD_BTN_CENTER) {
                //StartTask(JohnCena);
              }
              break;
          case 8:
            lcdPrint(1,0,"<      Back       >");
              if(lcdReadButtons(uart1)==LCD_BTN_CENTER) {
                option = 1;
                menu = MENU_MAIN;
                wait(1000);
              }
              break;
          case 9:
            option = 1;
            break;

          default:
            lcdPrint(1,0,"<    ERROR     >");
            break;
          }
          break;
      case MENU_POT:
        if(lcdReadButtons(uart1) == LCD_BTN_LEFT) {
          menu=MENU_MAIN;
          option=1;
          wait(500);
        }
        break;
      default:
        break;
    }
  }
}
