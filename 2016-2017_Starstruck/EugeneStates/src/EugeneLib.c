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
		holoSet(inputGetAnalog(1,4), inputGetAnalog(1,3), inputGetAnalog(1, 1));

    // Get pincer input
    // If 5U is held on the partner joystick, the pincers can be individually moved to sync them
    // TODO: mechanically sync pincers

    // Moves pincers normally
    // if(joystickGetDigital(1,5,JOY_UP)){
  		// pince(127);
    // } else if (joystickGetDigital(1,5,JOY_DOWN)){
    //   pince(-127);
    // } else {
    //   pince(0);
    // }

    // if(joystickGetDigital(1,6,JOY_UP)){
  		// lift(127);
    // } else if (joystickGetDigital(1,6,JOY_DOWN)){
    //   lift(-127);
    // } else {
    //   lift(0);
    // }
    lift(inputGetAnalog(2,3));
    pince(inputGetAnalog(2,1));

	}

// Robot functions
  void lift(int speed){
    // int potVal = analogRead(LIFT_POT);
    // if(potVal > LIFT_BOTTOM_THRESH && potVal < LIFT_TOP_THRESH){
      motorSet(AB,-speed);
      motorSet(CD,speed);
      motorSet(E,speed);
      motorSet(F,-speed);
    // } else {
    //   motorStop(AB);
    //   motorStop(CD);
    //   motorStop(E);
    //   motorStop(F);
    // }
  }

  void pince(int speed){
    // int potVal = analogRead(PINCE_POT);
    // if(potVal > PINCE_BOTTOM_THRESH && potVal < PINCE_TOP_THRESH){
      motorSet(PINCE,speed);
    // } else {
    //   motorStop(PINCE);
    // }
  }

  void driveAuton(int frontLeftSpeed, int backLeftSpeed, int frontRightSpeed, int backRightSpeed){
    motorSet(LF,frontLeftSpeed);
    motorSet(LB,backLeftSpeed);
    motorSet(RF,frontRightSpeed);
    motorSet(RB,backRightSpeed);
  }
