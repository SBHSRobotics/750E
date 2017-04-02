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
  bool swap = false;

  // Lift positions
  #define LIFT_CARRY 550
  #define LIFT_TOP 930
  #define LIFT_STAR_SIDE 360
  #define LIFT_STAR_CENTER 280

// Map functions
	void joystickMap(){
    // Get drive input

    if(inputGetDigital(1, 8, JOY_UP) && inputGetDigital(1, 8, JOY_RIGHT) &&
        inputGetDigital(2, 8, JOY_UP) && inputGetDigital(2, 8, JOY_RIGHT)) {
          swap = !swap;
          delay(500);
          if (swap) {
            beep();
            boop();
          } else {
            boop();
            beep();
          }
    } else if (joystickGetDigital(1,7,JOY_UP)) {
      driveSetByDistance(50);
      printf("SONAR %d\n",ultrasonicGet(sonar));
    } else {
      holoSet(0, inputGetAnalog((swap ? 2 : 1),3), inputGetAnalog((swap ? 2 : 1), 1));
    }

    // Get pincer input
    if(isJoystickConnected(2) || isAutonomous() || isReplayerAuton()) {
      lift(inputGetAnalog((swap ? 1 : 2), 3));
      pince(inputGetAnalog((swap ? 1 : 2), 1));
      // lift(joystickGetDigital(1, 5, JOY_UP) ? 127 : joystickGetDigital(1,5,JOY_DOWN) ? -127 : 0);
      // pince(joystickGetDigital(1, 6, JOY_UP) ? 127 : joystickGetDigital(1, 6, JOY_DOWN) ? -127 : 0);

      // if(joystickGetDigital(2,5,JOY_UP) && joystickGetDigital(2,5,JOY_DOWN)) {
      //   encoderReset(liftEnc);
      //   beep();
      // } else if(joystickGetDigital(2, 8, JOY_UP)) {
      //   liftSetByDistance(LIFT_TOP);
      // } else if(joystickGetDigital(2, 8, JOY_RIGHT)) {
      //   liftSetByDistance(LIFT_CARRY);
      // } else if(joystickGetDigital(2, 8, JOY_LEFT)) {
      //   liftSetByDistance(LIFT_STAR_SIDE);
      // } else if(joystickGetDigital(2, 8, JOY_DOWN)) {
      //   liftSetByDistance(LIFT_STAR_CENTER);
      // }

    } else if(digitalRead(1)){
      digitalWrite(2,HIGH);
      //Single Driver Controls (NOT INCLUDED IN AUTON RECORDINGS)
      if(joystickGetDigital(1,6,JOY_UP)){
      pince(-127);
    } else if (joystickGetDigital(1,6,JOY_DOWN)){
        pince(127);
      } else {
        pince(0);
      }

      if(joystickGetDigital(1,5,JOY_UP)){
      lift(127);
    } else if (joystickGetDigital(1,5,JOY_DOWN)){
        lift(-127);
      } else {
        lift(0);
      }

    } else {
      digitalWrite(2, LOW);
    }

	}

// Robot functions
void lift(int speed){
  motorSet(AB,speed);
  motorSet(C,speed);
  motorSet(D,speed);
  motorSet(E,speed);
  motorSet(F,speed);
}

void pince(int speed){
  motorSet(P_L,speed);
}

  void driveAuton(int frontLeftSpeed, int backLeftSpeed, int frontRightSpeed, int backRightSpeed){
    motorSet(LF,frontLeftSpeed);
    motorSet(LB,backLeftSpeed);
    motorSet(RF,frontRightSpeed);
    motorSet(RB,backRightSpeed);
  }

    void driveSetByDistance(int ultrasonicDistance) {
    	int AUTON_PID_THRESH = 5;
    	float err = ultrasonicDistance-ultrasonicGet(sonar);

    	if(abs(err)<=AUTON_PID_THRESH){
    				beep();
            holoSet(0, 0, 0);
    			} else {
    				float K = 25;
    				err=(err>0)?-K-err:K-err;
            printf("ERR %f\n", err);
    				holoSet(0, -err, 0);
    			}
    }

    void liftSetByDistance(int ultrasonicDistance) {
    	int AUTON_PID_THRESH = 5;
    	float err = ultrasonicDistance-encoderGet(liftEnc);

    	if(abs(err)<=AUTON_PID_THRESH){
    				beep();
            holoSet(0, 0, 0);
    			} else {
    				float K = -10;
    				err=(err>0)?-K-err:K-err;
            printf("ENC %d TAR %d ERR %f\n",encoderGet(liftEnc), ultrasonicDistance, err);
    				lift(-err);
    			}
    }
