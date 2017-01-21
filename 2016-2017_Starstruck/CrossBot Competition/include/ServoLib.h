/*
 * ServoLib.h
 *
 *  Created on: Dec 3, 2016
 *      Author: Mikel Matticoli
 */

#ifndef SERVOLIB_H_
#define SERVOLIB_H_

	/*
	 * This is the public API for Mikel's Servo Module. This file should be placed
	 * in the includes directory and included in main.h AFTER API.h
	 *
	 */

#ifndef DEBUG_MODE
	#define DEBUG_MODE 2 // Enable or disable debug output
#endif

// Public types
	typedef struct ServoSystem {
		unsigned char potentiometerPort;
		unsigned char motorPort;
		bool motorInverted;
		int motorScale;
		int targetTolerance;
		int *targetValue; // Changing values are stored as pointers so that ServoSystem variables can be passed by value
		TaskHandle *task; // Ignore error here, API.h is included before ServoLib.h
	}ServoSystem;

// Public function declarations
	/*
	 * Initializes a servo system and starts background tasks
	 */
	ServoSystem servoInit(unsigned char potentiometerPort, unsigned char motorPort, bool motorInverted, int motorScale, int targetTolerance);

	// NOTE: Each system is limited to 1 motor per potentiometer due to issues
	// with passing arrays around, although more than one system can utilize
	// the same potentiometer.
	// See http://stackoverflow.com/questions/6567742/passing-an-array-as-an-argument-to-a-function-in-c

	/*
	 * Stops a servo task
	 */
	void servoKill(ServoSystem servo);

	/*
	 * Changes the active drive configuration
	 */
	 void servoSet(ServoSystem servo, int targetValue);

	 /*
 	 * Returns current target value
 	 */
	 int servoGetTarget(ServoSystem servo);

	 /*
 	 * Returns servo's current position (AnalogRead)
 	 */
	 int servoGetPosition(ServoSystem servo); 

#endif /* EUGENE_H_ */
