
//========================================================//
//     Robot Library   (NOTE: Will not compile directly)  //
//        Anything robot/wiring dependant goes here       //
//========================================================//

#include "RobotLibEvo.c"
#include "Music.c"


//=== Utility Functions ===//


/* Resets chassis encoders */
void encoderReset() {
	//	nMotorEncoder[QuadL] = 0;
	//	nMotorEncoder[QuadR] = 0;
}


/* Calculates Left Shooter speed from RPMs */
int speedLfromRPM(int i) {
	float x = ((float)i);
	float y = -0.011*(x*x) + 2.566*x - 70.267; //Functions from benchmarking
	return y;
}


/* Calculates Right Shooter speed from RPMs */
int speedRfromRPM(int i) {
	float x = ((float)i);
	float y = -0.01*(x*x) + 2.496*x - 60.369;
	return y;
}


/* Shooter speed increase feedback */
task soundSpeedUp(){
  //        140 = Tempo
  //          5 = Default octave
  //    Quarter = Default note length
  //        10% = Break between notes
  //
  PlayTone(  698,   19); wait1Msec( 214);  // Note(F, Duration(Eighth))
  PlayTone(  932,   19); wait1Msec( 214);  // Note(A#, Duration(Eighth))
}


/* Shooter speed decrease feedback */
task soundSpeedDown(){
  //        140 = Tempo
  //          5 = Default octave
  //    Quarter = Default note length
  //        10% = Break between notes
  //
  PlayTone(  932,   19); wait1Msec( 214);  // Note(A#, Duration(Eighth))
  PlayTone(  698,   19); wait1Msec( 214);  // Note(F, Duration(Eighth))
}


//=== Module Functions ===//


/* Runs intake at specified speed. Stops intake if speed is not specified. */
void intakeSet(int speed = 0) {
	motorSet(INTAKE, speed);
}


/* Runs conveyor at specified speed. Stops conveyor if speed is not specified. */
void conveyorSet(int speed = 0) {
	motorSet(CON, speed);
}


/* Hard-sets the shooter to specified speeds */
void shooterSetDirect(int speedL = 0, int speedR = 0) {
	motorSet(S_L, speedL);
	motorSet(S_R, speedR);
}


/* Sets target speed for shooter (to be used with PID task) */
void shooterSetTarget(int target = 0) {
	shooter = target;
}


/* Runs chassis motors at specified speeds. Stops chassis if no parameters are specified
   First two parameters can be used alone for tank functionality.                       */
void driveSet(int leftSpeed = 0, int rightSpeed = 0, int hSpeed = 0) {
	motorSet(W_LF, -leftSpeed);
	motorSet(W_LB, leftSpeed);
	motorSet(W_RF, rightSpeed);
	motorSet(W_RB, rightSpeed);
	motorSet(W_HF, -hSpeed);
	motorSet(W_HB, hSpeed);
	//TODO: Horizontal Axis after holo drive is built
}


/* Uses PID loop with front Ultrasonic sensor to lock in at a distance from the pole */
void driveSetByDistance(int ultrasonicDistance) {
	float err = ultrasonicDistance-SensorValue[Sonar];
	if(abs(err)<=AUTON_PID_THRESH){
				driveSet();
				beep();
			} else {
				float K = 25;
				err=(err>0)?-K-err:K-err;
				driveSet(err, err, 0);
			}
}


//=== Control Functions ===//


/* Map chassis to primary joystick */
void joyMapMain() {
	driveX = joyMain.rx;
  driveY = joyMain.ly;
  driveH = joyMain.lx;

  if(abs(driveX)>20 && abs(driveY)>80) {
		driveY=(int)((float)driveY * 0.8 );
		driveX=(int)((float)driveX * 1.2 );
	}//Shift X & Y values for cleaner turning

	driveY=(abs(driveY)<20)?0:driveY;
	driveX=(abs(driveX)<20)?0:driveX;
	driveH=(abs(driveH)<85)?0:driveH;//Prevent drive from trying to move at low speeds

  driveSet( (driveY+driveX), (driveY-driveX), driveH);
}


/* Map buttons on primary joystick */
void buttonMapMain() {
	intakeSet(vexRT[Btn6U]?100:
						vexRT[Btn6D]?(-100):
						0);
	if(vexRT[Btn5U]) {
		driveSetByDistance(170);
	} else if(vexRT[Btn5D]) {
		driveSetByDistance(62);
	} else if(vexRT[Btn7U]) {
    driveSetByDistance(222);
  } else if(vexRT[Btn8R]) {
  	StartTask(PinkPanther);
	} else if(vexRT[Btn8L]) {
  	StartTask(HotlineBling);
	} else if(vexRT[Btn8D]) {
  	StartTask(Dhoom);
	}else if(vexRT[Btn8U]) {
  	StartTask(JohnCena);
	}

}


/* Map shooter to secondary joystick */
void joyMapPartner() {
  //Manually edit shooter speed with auditory feedback
	if(joyPartner.rx > 120) {
    shooter+=2;
    StopTask(soundSpeedUp);
		StartTask(soundSpeedUp);
		wait10Msec(10);
	}	else if(joyPartner.rx < -120) {
    shooter-=2;
		StopTask(soundSpeedDown);
	  startTask(soundSpeedDown);
		wait10Msec(10);
	} //pac-man af
}
/* Map buttons on secondary joystick */
void buttonMapPartner() {
		shooter = vexRT[Btn8RXmtr2]?75://Set shooter speed with Joy2 buttons
			  			vexRT[Btn8UXmtr2]?60:
				  		vexRT[Btn8LXmtr2]?55:
				  		vexRT[Btn8DXmtr2]?0:
				  		vexRT[Btn7RXmtr2]?127:
				  		vexRT[Btn7UXmtr2]?100:
			  			vexRT[Btn7LXmtr2]?94:
			  			vexRT[Btn7DXmtr2]?80:shooter;

		conveyorSet(vexRT[Btn6UXmtr2]?100:
							  vexRT[Btn6DXmtr2]?(-100):
						    0); //Successful over-use of ternary operator

		shooterSetDirect(shooter, shooter);
}

///* Map buttons on secondary joystick */
//void buttonMapPartner() {
//		shooter = vexRT[Btn8RXmtr2]?70://Set shooter speed with Joy2 buttons
//			  			vexRT[Btn8UXmtr2]?65:
//				  		vexRT[Btn8LXmtr2]?55:
//				  		vexRT[Btn8DXmtr2]?0:
//				  		vexRT[Btn7RXmtr2]?127:
//				  		vexRT[Btn7UXmtr2]?95:
//			  			vexRT[Btn7LXmtr2]?80:
//			  			vexRT[Btn7DXmtr2]?75:
//			  			vexRT[Btn5UXmtr2]?60:
//			  			vexRT[Btn5DXmtr2]?100:shooter;

//		conveyorSet(vexRT[Btn6UXmtr2]?100:
//							  vexRT[Btn6DXmtr2]?(-100):
//						    0); //Successful over-use of ternary operator

//		shooterSetDirect(shooter, shooter);
//}


//=== LCD Debugging ===//


//Button Macros
#define LCD_L 1
#define LCD_M 2
#define LCD_R 4

//Menu Macros
#define MENU_MAIN 0
#define MENU_BATT 1
#define MENU_MST 2
#define MENU_MEMES 3
#define MENU_SHOOTER 4
#define MENU_POT 5

//Active Menu
int menu = MENU_MAIN;
//Active Menu Option
int option = 1;


/* Display Main and Power Expander Battery Voltages on LCD */
void lcdDisplayVoltage() {
  lcdClear();
  string s = "Main: ";
  sprintf(s,"%dmV",nImmediateBatteryLevel); //Format LCD line 1
  displayLCDString(0, 0, s); //Display main battery on LCD

  s= "Expander: ";
  sprintf(s,"%dmV",SensorValue[Expander]*21.93); //Format LCD line 1
  displayLCDString(1, 0, s); //Display power expander battery on LCD
  wait10Msec(50);
}


/* Display shooter speeds on LCD */
void lcdDisplayShooter() {
  lcdClear();
  displayLCDNumber(0, 0, motor[S_L], 2);
  displayLCDNumber(0, 8, motor[S_R], 2);
  displayLCDString(1,0,"<Back");
}

void lcdDisplayPot() {
  lcdClear();
  displayLCDNumber(0, 0, SensorValue[Sel]/10, 2);
  displayLCDNumber(0, 8, SensorValue[Sel], 2);
  displayLCDString(1,0,"<Back");
}


/* LCD Debugging Menu - Runs asynchronously
WARNING: WHEN RUNNING POST ASYNCHRONOUSLY, DO NOT ATTEMPT JOYSTICK CONTROL */
task lcdMenu() {
  while(true) {
    wait1Msec(50);
    lcdClear();

    if(nLCDButtons == LCD_L) {
      option++;
      wait1Msec(100);
    } else if (nLCDButtons == LCD_R) {
      option--;
      wait1Msec(100);

    }

    //Show options based on active menu
    switch(menu) {
      case MENU_MAIN:

        displayLCDString(0,3,"Main Menu");

        switch(option) {

          case 0:
            option=6;
            break;

          case 1:
            displayLCDString(1,0,"< BATTERY LVL  >");
            if(nLCDButtons==LCD_M) {
              menu=MENU_BATT;
              option=1;
              wait10Msec(50);
            }
            break;

          case 2:
            displayLCDString(1,0,"<  SELF-TEST   >");
              if(nLCDButtons==LCD_M) {
                powerOnSelfTest();
              }
              break;

          case 3:
            displayLCDString(1,0,"< MANUAL TEST  >");
              if(nLCDButtons==LCD_M) {
                menu=MENU_MST;
                option=1;
                wait10Msec(50);

              }
              break;
          case 4:
            displayLCDString(1,0,"< PLAY MUSIC  >");
              if(nLCDButtons==LCD_M) {
                menu=MENU_MEMES;
                option=1;
                wait10Msec(50);

              }
              break;
          case 5:
            displayLCDString(1,0,"< SHOOTER SPD  >");
            if(nLCDButtons==LCD_M) {
              menu=MENU_SHOOTER;
              option=1;
              wait10Msec(50);
            }
            break;
          case 6:
            displayLCDString(1,0,"<   POT  SPD   >");
            if(nLCDButtons==LCD_M) {
              menu=MENU_POT;
              option=1;
              wait10Msec(50);
            }
            break;

          case 7:
            option = 1;
            break;

          default:
            displayLCDString(1,0,"<    ERROR     >");
            break;

        }
        break;

      case MENU_BATT:
        lcdDisplayVoltage();
        if(nLCDButtons == LCD_L) {
          menu=MENU_MAIN;
          option=1;
          wait10Msec(50);
        }
        break;

      case MENU_SHOOTER:
        lcdDisplayShooter();
        if(nLCDButtons == LCD_L) {
          menu=MENU_MAIN;
          option=1;
          wait10Msec(50);
        }
        break;

      case MENU_MST:
        displayLCDString(0,0,"  Select Motor  ");
        switch(option) {

          case 0:
            option=11;
            break;

          case 1:
            displayLCDString(1,0,"<    Port 1    >");
            if(nLCDButtons==LCD_M) {
              pulseMotor(port1);
            }
            break;

          case 2:
            displayLCDString(1,0,"<    Port 2    >");
            if(nLCDButtons==LCD_M) {
              pulseMotor(port2);
            }
            break;

          case 3:
            displayLCDString(1,0,"<    Port 3    >");
            if(nLCDButtons==LCD_M) {
              pulseMotor(port3);
            }
            break;

          case 4:
            displayLCDString(1,0,"<    Port 4    >");
            if(nLCDButtons==LCD_M) {
              pulseMotor(port4);
            }
            break;

          case 5:
            displayLCDString(1,0,"<    Port 5    >");
            if(nLCDButtons==LCD_M) {
              pulseMotor(port5);
            }
            break;

          case 6:
            displayLCDString(1,0,"<    Port 6    >");
            if(nLCDButtons==LCD_M) {
              pulseMotor(port6);
            }
            break;

          case 7:
            displayLCDString(1,0,"<    Port 7    >");
            if(nLCDButtons==LCD_M) {
              pulseMotor(port7);
            }
            break;

          case 8:
            displayLCDString(1,0,"<    Port 8    >");
            if(nLCDButtons==LCD_M) {
              pulseMotor(port8);
            }
            break;

          case 9:
            displayLCDString(1,0,"<    Port 9    >");
            if(nLCDButtons==LCD_M) {
              pulseMotor(port9);
            }
            break;

          case 10:
            displayLCDString(1,0,"<    Port 10   >");
            if(nLCDButtons==LCD_M) {
              pulseMotor(port10);
            }
            break;

          case 11:
            displayLCDString(1,0,"<     Back    >");
            if(nLCDButtons==LCD_M) {
              menu=MENU_MAIN;
              option=1;
              wait10Msec(100);
            }
            break;

          case 12:
            option = 1;
            break;

          default:
            displayLCDString(1,0,"<    ERROR     >");
            break;
          }
          break;

      case MENU_MEMES:
        switch(option) {

          case 0:
            option=8;
            break;

          case 1:
            displayLCDString(1,0,"<     Beep     >");
            if(nLCDButtons==LCD_M) {
              beep();
            }
            break;

          case 2:
            displayLCDString(1,0,"<     Boop     >");
              if(nLCDButtons==LCD_M) {
                boop();
              }
              break;

          case 3:
            displayLCDString(1,0,"<    Dhoom     >");
              if(nLCDButtons==LCD_M) {
                StartTask(Dhoom);
              }
              break;

          case 4:
            displayLCDString(1,0,"<  Take On Me  >");
              if(nLCDButtons==LCD_M) {
                StartTask(TakeOnMe);
              }
              break;

          case 5:
            displayLCDString(1,0,"<  BigBoyRayC  >");
              if(nLCDButtons==LCD_M) {
                StartTask(PinkPanther);
              }
              break;

          case 6:
            displayLCDString(1,0,"< HotlineBling >");
              if(nLCDButtons==LCD_M) {
                StartTask(HotlineBling);
              }
              break;
          case 7:
            displayLCDString(1,0,"<  John  Cena  >");
              if(nLCDButtons==LCD_M) {
                StartTask(JohnCena);
              }
              break;
          case 8:
            displayLCDString(1,0,"<      Back       >");
              if(nLCDButtons==LCD_M) {
                option = 1;
                menu = MENU_MAIN;
                wait10Msec(100);
              }
              break;
          case 9:
            option = 1;
            break;

          default:
            displayLCDString(1,0,"<    ERROR     >");
            break;
          }
          break;
      case MENU_POT:
        lcdDisplayPot();
        if(nLCDButtons == LCD_L) {
          menu=MENU_MAIN;
          option=1;
          wait10Msec(50);
        }
        break;
      default:
        break;
    }
  }
}


//=== Field Positioning System ===//

//DISABLED FOR SPARTA - MORE TESTING NEEDED

// //Tile Mode Macros
// #define FPS_MODE_START 1
// #define FPS_MODE_BAN 2
// #define FPS_MODE_TARGET 3

// //Starting Tile ID Macros
// #define FPS_TILE_RED1 1
// #define FPS_TILE_RED2 2
// #define FPS_TILE_BLUE1 3
// #define FPS_TILE_BLUE2 4

// //Basic Tile Map          1   2   3   4   5   6
// int fpsModeMap[][] = { { 02, 02, 00, 00, 02, 03 },   // 1
//                        { 02, 00, 00, 00, 00, 02 },   // 2
//                        { 00, 00, 00, 00, 00, 00 },   // 3
//                        { 00, 00, 00, 00, 00, 00 },   // 4
//                        { 01, 00, 00, 00, 00, 02 },   // 5
//                        { 00, 01, 00, 00, 02, 02 } }; // 6

// //Basic Tile Map          1   2   3   4   5   6
// int fpsDMap[][] = { { 0, 0, 00, 00, 02, 03 },   // 1
//                     { 0, 00, 00, 00, 00, 02 },   // 2
//                     { 00, 00, 00, 00, 00, 00 },   // 3
//                     { 00, 00, 00, 00, 00, 00 },   // 4
//                     { 01, 00, 00, 00, 00, 02 },   // 5
//                     { 00, 01, 00, 00, 02, 02 } }; // 6




// /* Get mode for a tile at xy coordinates
//       Note: Array access is map[y][x]   */
// int fpsTileMode(int tile_x, int tile_y) {
//   return fpsModeMap[tile_y][tile_x];
// }


// /* Prints field map in RobotC Debug Stream */
// void fpsPrintMap() {
//   int x, y;
//   for(y=0; y<6; y++){
//     writeDebugStream("\n");
//     for(x=0; x<6; x++) {
//       int i = fpsTileMode(x,y);
//       writeDebugStream( ((i==FPS_MODE_START) ? "S\t" :
//                          (i==FPS_MODE_BAN) ? "X\t" :
//                          (i==FPS_MODE_TARGET) ? "T\t" :
//                          "_\t") );
//     }
//   }
// }
