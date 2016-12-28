/*
 * ServoLib.h
 *
 *  Created on: Dec 3, 2016
 *      Author: Mikel Matticoli
 */

#ifndef SERVOLIB_H_
#define SERVOLIB_H_

	/*fd
	 * This is the public API for Mikel's Servo Module. This file should be placed in includes and
	 * included in main.h AFTER API.h
	 *
	 */

#ifndef DEBUG_MODE
	#define DEBUG_MODE 0 // Enable or disable debug output
#endif

// Public types
	typedef struct ServoSystem {
		unsigned char potentiometerPort;
		unsigned char motorPort;
		bool motorInverted;
		int motorScale;
		int targetTolerance;
		int targetValue;
		TaskHandle *task; // Ignore error here, API.h is included before ServoLib.h
	}ServoSystem;

// Public function declarations
	/*
	 * Initializes servo system and starts background tasks
	 *
	 */
	 //TODO Pass array using pointer notation. See http://stackoverflow.com/questions/6567742/passing-an-array-as-an-argument-to-a-function-in-c
	ServoSystem servoInit(unsigned char potentiometerPort, unsigned char motorPort, bool motorInverted, int motorScale, int targetTolerance);
	//ServoSystem servoInitSingle(unsigned char potentiometerPort, unsigned char motorPorts[], int motorScale, int targetTolerance);

	/*
	 * Stops all servo tasks
	 */
	void servoKill(ServoSystem servo);

	/*
	 * Changes the active drive configuration
	 */
	 void servoSet(ServoSystem servo, int targetValue);

#endif /* EUGENE_H_ */
