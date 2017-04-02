/*
 * CrossDriver.h
 *
 *  Created on: Feb 4, 2017
 *      Author: Kirsten Meidlinger
 */

#ifndef EUGENELIB_H_
#define EUGENELIB_H_

  /*
   * This is the library for 750E's Starstruck Robot Eugene
   * This file should be placed in include and included in main.h
	 */

 // Port Definitions - Implemented as constants for type-safety. In low-memory situations, these may be substituted for macros.
   // Chassis
     static const unsigned char LF = 1;	// Front left drive wheel, was 1
     static const unsigned char LB = 7;	// Back left drive wheel, was previously 2
     static const unsigned char RF = 10;	// Front Right drive wheel, was previously 9
     static const unsigned char RB = 2;	// Back right drive wheel, was previously 10

   // Lift
     static const unsigned char AB = 3; // Top of lift, Motors A and B together, was 5
     static const unsigned char C = 8; // Motors C bottomleft lift, was 6 y-ed off)
     static const unsigned char E = 4; // Left-side lift, middle motor, was 4
     static const unsigned char F = 9; // Right-side lift, middle motor, was 7
     static const unsigned char D = 5; //Motor D bottom right lift, was 6 yed-off

   //Pincers
     static const unsigned char P_L = 6;



//Sensors
  static const unsigned char ENC_TOP = 1;
  static const unsigned char ENC_BOT = 2;
  static const unsigned char SONAR_ORANGE = 3;
  static const unsigned char SONAR_YELLOW = 4;

  static const unsigned char POT = 1;

  Ultrasonic sonar;
  Encoder liftEnc;

//Sensor Thresholds
  static const int LIFT_TOP_THRESH = 0;
  static const int LIFT_BOTTOM_THRESH = 0;
  static const int PINCE_TOP_THRESH = 0;
  static const int PINCE_BOTTOM_THRESH = 0;

// LCD Constants
  #define BATTERY_MENU 0
  #define AUTON_MENU 1
  #define SELF_MENU 2
  #define MANUAL_MENU 3
  #define MEMES_MENU 4
  #define BATTERY_VAL 5
  #define AUTON_VAL 6
  #define SELF_VAL 7
  #define MANUAL_VAL 8
  #define MEMES_VAL 9

// Global Vars
MenuItem *lcdRoot;

// Map functions
  void joystickMap();
  void joystickMapPartner();

// Robot functions
  void lift(int speed);
  void pince(int speed);
  void driveAuton(int frontLeftSpeed, int backLeftSpeed, int frontRightSpeed, int backRightSpeed);
  void driveOp();
  void stopDrive();
  void driveSetByDistance(int ultrasonicDistance);
  void liftSetByDistance(int encVal);

#endif //EUGENELIB_H_
