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
void servoLoop(unsigned char motor, unsigned char potentiometerPort, int targetValue, bool inverted);

// Public function definitions
ServoSystem servoInit(unsigned char potentiometerPort, unsigned char motorPort, bool motorInverted, int motorScale, int targetTolerance) {
		printf("servo obj init");
		delay(200);
		ServoSystem servo = {
			.potentiometerPort = potentiometerPort,
			.motorPort = motorPort,
			.motorInverted = motorInverted,
			.motorScale = motorScale,
			.targetTolerance = targetTolerance,
			.targetValue = analogRead(potentiometerPort)
		};
		delay(200);
		printf("done");
		delay(200);
		printf("loop def");
		delay(200);
		void loop(){
			while(1) {
				servoLoop(&servo.motorPort, servo.potentiometerPort, &servo.targetValue, &servo.motorInverted);
			}
		}
		delay(200);
		printf("done");
		delay(200);
		printf("task create");
		delay(200);

		servo.task = taskCreate(loop,TASK_DEFAULT_STACK_SIZE,NULL,TASK_PRIORITY_DEFAULT);
		delay(200);
		printf("done");
		delay(200);
		return servo;
}

void servoKill(ServoSystem servo) {
	taskDelete(servo.task);
}

void servoSet(ServoSystem servo, int targetValue) {
	servo.targetValue = targetValue;
}

// Private function definitions

void servoLoop(unsigned char *motor, unsigned char potentiometerPort, int *targetValue, bool *inverted) {
	delay(200);
	int currentValue = analogRead(1);
	targetValue= 0;
	printf("Motor: %d\tVal: %d;%d\tTarget: %d\n\r", *motor, potentiometerPort, currentValue, targetValue);
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
