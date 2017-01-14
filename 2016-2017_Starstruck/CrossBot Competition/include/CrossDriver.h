/*
 * CrossDriver.h
 *
 *  Created on: Dec 27, 2016
 *      Author: Mikel Matticoli
 */

#ifndef CROSSDRIVER_H_
#define CROSSDRIVER_H_

	/*
	 * This is the public API for 750E's CrossBot Drive Library. This file should be placed in includes and
	 * included in main.h
	 *
	 * This library is dependent on ServoLib. Make sure to include ServoLib before CrossDriver in main.h
	 */


// LCD Constants
#define POT_MENU 0
#define BATTERY_MENU 1
#define AUTON_MENU 2
#define SELF_MENU 3
#define MANUAL_MENU 4
#define MEMES_MENU 5
#define POT_VAL 6
#define BATTERY_VAL 7
#define AUTON_VAL 8
#define SELF_VAL 9
#define MANUAL_VAL 10
#define MEMES_VAL 11

// Port Definitions - Implemented as constants for type-safety. In low-memory situations, these may be substituted for macros.
  	// Chassis
	static const unsigned char R = 7;	// Swerve rotation control motor
	static const unsigned char WF = 2;	// Front drive wheel
	static const unsigned char WL = 3;	// Left drive wheel
	static const unsigned char WR = 8;	// Right drive wheel
	static const unsigned char WB = 9;	// Back drive wheel

	// Lift
	static const unsigned char LIFT_AB = 4; // Motor A and B y-ed into one port, same dir
	static const unsigned char LIFT_CD = 5; // Motor C and D y-ed into one port, same dir
	static const unsigned char LIFT_E = 6; // Motor E not on PE

	// Pincers
	static const unsigned char PL = 1; // Left pincer
	static const unsigned char PR = 10; // Right pincer

	// Sensors
		// Analog
	static const unsigned char ROT_POT = 1;	// Swerve rotation control potentiometer
	static const unsigned char PL_POT = 2; // Left pincer potentiometer
	static const unsigned char PR_POT = 3; // Right pincer potentiometer

	static const unsigned char EXPANDER = 2;
	static const unsigned char GYRO = 3;
		// Digital
	// This space intentionally left blank

// Constants
	// PID (These may need tuning depending on mechanical implementation and weight)
	static const int PID_MOTOR_SCALE = 65;	// Lower = faster, Higher = More precise
	static const int PID_THRESH = 100;		// Minimum power (motor speed as error approaches zero)
	// NOTE: When modifying other parts of the robot, these constants may need to be adjusted
	// to accommodate weight changes
	static const int DRIVE_START = 2040;


// Global Vars
	extern ServoSystem drive;

// Drive
void driveInit();

void driveSetPos(int targetValue);
void driveForward(int speed);
void pointTurn(int speed);
void lift(int speed);
void pince(int speed);

// LCD TODO:  MOVE INTO SEPARATE MODULE (MenuLib?)
void lcdStart();
void lcdLoop();

#endif /* CROSSDRIVER_H_ */
