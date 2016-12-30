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
		ServoSystem servo = {
			.potentiometerPort = potentiometerPort,
			.motorPort = motorPort,
			.motorInverted = motorInverted,
			.motorScale = motorScale,
			.targetTolerance = targetTolerance,
			.targetValue = malloc(sizeof(int *)),
			.task = malloc(sizeof(TaskHandle *))
		};
		printf("Setting target");
		delay(100);
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

		*servo.task = taskCreate(loop,TASK_DEFAULT_STACK_SIZE,NULL,TASK_PRIORITY_DEFAULT);
		delay(200);
		printf("done\n\r");
		delay(200);
		return servo;
}

void servoKill(ServoSystem servo) {
	taskDelete(*servo.task);
}

void servoSet(ServoSystem servo, int target) {
	// printf("SET1 Pot: %p\t%d\n\r", servo.targetValue, *servo.targetValue);
	*servo.targetValue = target;
	// printf("SET2 Pot: %p\t%d\n\r", servo.targetValue, *servo.targetValue);
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
		printf("M: %d\tVal: %d\Tar: %d\tS: %e\n\r", servo.motorPort, currentValue, *servo.targetValue, speed);
		//TODO: Move debug info to on-board menu
	#endif
}
