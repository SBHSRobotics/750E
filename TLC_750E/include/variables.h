#ifndef INCLUDE_VARIABLES_H_
#define INCLUDE_VARIABLES_H_

#include "main.h"

#define MOTOR_BASE_LEFT1_ 4
#define MOTOR_BASE_LEFT2_ 5
#define MOTOR_BASE_RIGHT1_ 2
#define MOTOR_BASE_RIGHT2_ 3
/*#define MOTOR_LIFT_LEFT_ 5
#define MOTOR_LIFT_RIGHT_ 6
#define MOTOR_PINCE_LEFT_ 7
#define MOTOR_PINCE_RIGHT_ 8*/

void drive(int ch1, int ch2, int ch3, int ch4);
void lift(int speed);
void pince(int speed);
#endif
