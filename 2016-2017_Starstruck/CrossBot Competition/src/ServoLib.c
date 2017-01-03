/*
 * ServoLib.c
 *
 *  Created on: Dec 3, 2016
 *      Author: Mikel Matticoli
 */

#include <main.h>

/*
 * This is the core for Mikel's Servo Module. This file should be placed
 * in the src directory. Please note that this file will not compile
 * if ServoLib.h has not been included properly.
 */

// Private function declarations

/*
 * Updates the speed of given motor based on current and target potentiometer values
 */
void servoLoop(ServoSystem servo);

// Public function definitions
ServoSystem servoInit(unsigned char potentiometerPort, unsigned char motorPort, bool motorInverted, int motorScale, int targetTolerance) {
		printf("Initializing servo with potentiometer %d and motor %d\n\r", potentiometerPort, motorPort);
		delay(100);
		// Create ServoSystem struct
		ServoSystem servo = {
			.potentiometerPort = potentiometerPort,
			.motorPort = motorPort,
			.motorInverted = motorInverted,
			.motorScale = motorScale,
			.targetTolerance = targetTolerance,
			.targetValue = malloc(sizeof(int *)), // Initializes .targetValue with an empty integer pointer
			.task = malloc(sizeof(TaskHandle *)) // Initializes .targetValue with an empty TaskHandle pointer
		};
		printf("Setting target");
		delay(100);
		// Set target to current value to prevent system from moving to 0 and potentially breaking itself
		*servo.targetValue = analogRead(potentiometerPort);
		delay(100);
		printf("Target aquired");
		delay(100);
		printf("INIT Pot: %p\t%d\n\r",servo.targetValue, *servo.targetValue);
		delay(100);
		printf("Done.\n\r");
		delay(100);
		printf("Defining loop function\n\r");
		delay(200);

		// This nested loop function runs servoLoop with the proper parameters and delays. This is
		// necessary since taskCreate doesn't accept functions with parameters.
		void loop() {
			ServoSystem s = servo;
			while(1) {
				servoLoop(s);
				delay(100);
			}
		}

		delay(200);
		printf("done\n\r");
		delay(200);
		printf("task create\n\r");
		delay(200);

		// Create a new async task to run servo loop
		*servo.task = taskCreate(loop,TASK_DEFAULT_STACK_SIZE,NULL,TASK_PRIORITY_DEFAULT);
		delay(200);
		printf("Done.\n\r");
		delay(200);

		// Note: This will return a copy of servo, so the location of the return value != &servo
		return servo;
}

void servoKill(ServoSystem servo) {
	// Stops the task associated with a servo system
	// TODO: Delete servo to prevent memory leaks
	taskDelete(*servo.task);
}

void servoSet(ServoSystem servo, int target) {
	// Sets the target value of the given servo system
	*servo.targetValue = target;
}

// Private function definitions

void servoLoop(ServoSystem servo) {
	int currentValue = analogRead(1);

	// Do not run motors if they have reached the target position
	if (abs(*servo.targetValue-currentValue)<PID_THRESH) {
		motorStop(servo.motorPort);
		return;
	}

	// Calculate instantaneous error
	double err = (double)((double)*servo.targetValue - (double)currentValue);

	//Convert Target Value to Motor Speed
	int speed = (servo.motorInverted ? -1 : 1) * (int)(((double)err / ((double)4095) * (double)PID_MOTOR_SCALE) );

	// Shift speed scale past deadzone
	speed +=
		( (speed > 0)
			? (127 - PID_MOTOR_SCALE)
			: (-127 + PID_MOTOR_SCALE)
		 );
	motorSet(servo.motorPort, -speed);

	#if DEBUG_MODE == 1 || DEBUG_MODE == 2
		printf("M: %d\tVal: %d\tTar: %d\tS: %e\n\r", servo.motorPort, currentValue, *servo.targetValue, speed);
		//TODO: Move debug info to on-board menu if LCDLib is included
	#endif
}
