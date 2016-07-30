/*
 * SwerveLib.h
 *
 *  Created on: Jul 24, 2016
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
	#define DEBUG_MODE 1 // Enable or disable debug output
#endif

// Default drive mode (MUST BE A GLOBAL DriveConfiguration LISTED BELOW)
#define DEFAULT_DRIVE_MODE holonomicDrive

// Port Definitions - Implemented as constants for type-safety. In low-memory situations, these may be substituted for macros.
  	// Chassis
	static const unsigned char LR = 10;	// Left side rotation control
	static const unsigned char LF = 9;	// Front left drive wheel
	static const unsigned char LB = 8;	// Back left drive wheel
	static const unsigned char RR = 1;	// Right side rotation control
	static const unsigned char RF = 3;	// Front Right drive wheel
	static const unsigned char RB = 2;	// Back right drive wheel
	//TODO: CHANGE THESE PORTS (Not intuitive at all)

	// Sensors
	static const unsigned char LP = 8;	// Left side rotation measurement
	static const unsigned char RP = 1;	// Right side rotation measurement

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

// Public Global Variable declarations (defined in SwerveLib.c)
	extern DriveConfiguration holonomicDrive;	// Standard holonomic-x
	extern DriveConfiguration tankDrive;	// Standard tank
	extern DriveConfiguration shuffleDrive;	// Horizontal tank

	extern DriveConfiguration currentConfig;

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

#endif /* SWERVELIB_H_ */
