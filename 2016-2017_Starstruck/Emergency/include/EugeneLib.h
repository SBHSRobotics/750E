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
    static const unsigned char LF = 2;	// Front left drive wheel
    static const unsigned char LB = 3;	// Back left drive wheel
    static const unsigned char RF = 8;	// Front Right drive wheel
    static const unsigned char RB = 9;	// Back right drive wheel

  // Lift
    static const unsigned char AB = 4; // Top of lift, Motors A and B together
    static const unsigned char CD = 5; // Bottom of lift, Motors C and D together
    static const unsigned char E = 7; // Left-side lift, middle motor
    static const unsigned char F = 10; // Right-side lift, middle motor

  //Pincers
    static const unsigned char PINCE = 6;

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

// Map functions
  void joystickMap();
  void joystickMapPartner();

// Robot functions
  void lift(int speed);
  void pince(int speed);
  void driveAuton(int frontLeftSpeed, int backLeftSpeed, int frontRightSpeed, int backRightSpeed);
  void driveOp();
  void stopDrive();
  void pulseMotor(unsigned char port);
  void lcdStart();
  void lcdLoop();

#endif //EUGENELIB_H_
