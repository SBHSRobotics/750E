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

#ifndef DEBUG_MODE
	#define DEBUG_MODE 0 // Enable or disable debug output
#endif

// Default drive mode (MUST BE A GLOBAL DriveConfiguration LISTED BELOW)
#define DEFAULT_DRIVE_MODE holonomicDrive

//LCD Constants
#define POT_MENU 0
#define DRIVE_MENU 1
#define BATTERY_MENU 2
#define SELF_MENU 3
#define MANUAL_MENU 4
#define MEMES_MENU 5
#define POT_VAL 6
#define DRIVE_VAL 7
#define BATTERY_VAL 8
#define SELF_VAL 9
#define MANUAL_VAL 10
#define MEMES_VAL 11

// Port Definitions - Implemented as constants for type-safety. In low-memory situations, these may be substituted for macros.
  	// Chassis
	static const unsigned char R = 1;	// Swerve rotation control motor
	static const unsigned char WF = 2;	// Front drive wheel
	static const unsigned char WL = 3;	// Left drive wheel
	static const unsigned char WR = 8;	// Right drive wheel
	static const unsigned char WB = 9;	// Back drive wheel

	// Lift
	// static const unsigned char AC = 4; // Motor A and C y-ed into one port, same dir
	// static const unsigned char BD = 5; // Motor B and D y-ed into one port, same dir

	//Pincers
	// static const unsigned char PL = 6; // Left pincer
	// static const unsigned char PR = 7; // Right pincer

	// Sensors
		// Analog
	static const unsigned char RP = 1;	// Swerve rotation control potentiometer
	static const unsigned char EXPANDER = 2;
	static const unsigned char GYRO = 3;
		// Digital
	// This space intentionally left blank

// Constants
	// PID (These may need tuning depending on mechanical implementation and weight)
	static const int PID_MOTOR_SCALE = 80;	// Lower = faster, Higher = More precise
	static const int PID_THRESH = 60;		// Minimum power (motor speed as error approaches zero)
	// NOTE: When modifying other parts of the robot, these constants may need to be adjusted
	// to accommodate weight changes
	static const int DRIVE_START = 2040;


// Global Vars
	extern ServoSystem drive;

// Drive
void driveInit();

void driveSet(int targetValue);

// LCD TODO:  MOVE INTO SEPARATE MODULE (MenuLib?)
void LCD();

#endif /* CROSSDRIVER_H_ */
