/*
 * ServoLib.c
 *
 *  Created on: Dec 3, 2016
 *      Author: Mikel Matticoli
 */

#include <main.h>
#include <time.h>


// Private function declarations
/**/
void servoLoop(unsigned char motor, int currentValue, int targetValue, bool inverted);

// Public function definitions
ServoSystem servoInit(unsigned char potentiometerPort, unsigned char motorPorts[], bool motorInversion[], int motorScale, int targetTolerance) {
		ServoSystem servo = { potentiometerPort, motorPorts, motorInversion, motorScale, targetTolerance, analogRead(potentiometerPort), NULL };

		void loop(){
			int i;
			while(1) {
				for( i = 0; i < (sizeof(motorPorts) / sizeof(motorPorts[0])); i++ ) {
					servoLoop(servo.motorPorts[i], analogRead(potentiometerPort), servo.targetValue, servo.motorInversion[i]);
				}
			}
		}

		servo.task = taskCreate(loop,TASK_DEFAULT_STACK_SIZE,NULL,TASK_PRIORITY_DEFAULT);
		return servo;
}

void servoKill(ServoSystem servo) {
	taskDelete(servo.task);
}

void servoSet(ServoSystem servo, int targetValue) {
	servo.targetValue = targetValue;
}

// Private function definitions

void servoLoop(unsigned char motor, int currentValue, int targetValue, bool inverted) {
	printf("Motor: %d\tVal: %d\tTarget: %d\n\r", motor, currentValue, targetValue);
	// TODO: Integral ~ motors still oscillate sporadically

	// Do not run motors if they have reached the target position
	if (abs(targetValue-currentValue)<PID_THRESH) {
		motorStop(motor);
		return;
	}

	// Proportional <calculate error>
	double err = (double)((double)targetValue - (double)currentValue);

	//Convert Target Value to Motor Speed
	int speed = (inverted ? -1 : 1) * (int)(((double)err / ((double)4095) * (double)PID_MOTOR_SCALE) );

	// Shift speed scale past deadzone
	speed +=
		( (speed > 0)
			? (127 - PID_MOTOR_SCALE)
			: (-127 + PID_MOTOR_SCALE)
		 );
	motorSet(motor, -speed);
}
