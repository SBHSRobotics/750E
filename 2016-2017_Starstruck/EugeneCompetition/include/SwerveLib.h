/*
 * Eugene.h
 *
 *  Created on: Dec 3, 2016
 *      Author: Mikel Matticoli
 */

#ifndef SWERVELIB_H_
#define SWERVELIB_H_

	/*
	 * This is the public API for 750E's Swerve Drive Library. This file should be placed in includes and
	 * included in main.h
	 *
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
	static const unsigned char LR = 0;	// Left side rotation control
	static const unsigned char LF = 2;	// Front left drive wheel
	static const unsigned char LB = 3;	// Back left drive wheel
	static const unsigned char RR = 0;	// Right side rotation control
	static const unsigned char RF = 9;	// Front Right drive wheel
	static const unsigned char RB = 8;	// Back right drive wheel

	// Lift
	static const unsigned char AC = 4; // Motor A and C y-ed into one port, same dir
	static const unsigned char BD = 5; // Motor B and D y-ed into one port, same dir

	//Pincers
	static const unsigned char PL = 1; // Left pincer
	static const unsigned char PR = 10; // Right pincer

	// Sensors
	static const unsigned char LP = 0;	// Left side rotation measurement
	static const unsigned char RP = 0;	// Right side rotation measurement
	static const unsigned char EXPANDER = 2; // Power Expander
// Constants
	// PID (These may need tuning depending on mechanical implementation and weight)
	static const int PID_MOTOR_SCALE = 80;	// Lower = faster, Higher = More precise
	static const int PID_THRESH = 60;		// Minimum power (motor speed as error approaches zero)
	// NOTE: When modifying other parts of the robot, these constants may need to be adjusted
	// to accommodate weight changes

	// Drive Configs - These must be constant expressions to be used in struct init http://goo.gl/QLQMuc
	#define HOLONOMIC_DRIVE 0
	#define TANK_DRIVE 1
	#define SHUFFLE_DRIVE 2

// Public types
	typedef struct DriverConfiguration {
		unsigned char id;
		int leftWheel;
		int rightWheel;
	}DriveConfiguration;

// Public Global Variable declarations (defined in Eugene.c)
	extern DriveConfiguration holonomicDrive;	// Standard holonomic-x
	extern DriveConfiguration tankDrive;	// Standard tank
	extern DriveConfiguration shuffleDrive;	// Horizontal tank

	extern DriveConfiguration currentConfig;
	extern bool isCrabKilled;

// Public function declarations
	/*
	 * Initializes swerve drive and starts background tasks
	 */
	void crabInit();

	/*
	 * Stops all swerve drive tasks
	 */
	void crabKill();

	/*
	 * Changes the active drive configuration
	 */
	 void setDriveConfig(DriveConfiguration config);
	 void setDriveConfigById(int id);

	 /*
 	 * Moves a motor in either direction to check if it works
 	 */
	void pulseMotor(unsigned char x);

	/*
	 * LCD Menu
	 */
 	void LCD();

#endif /* SWERVELIB_H_ */
