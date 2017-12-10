#ifndef INCLUDE_VARIABLES_H_
#define INCLUDE_VARIABLES_H_

#include "main.h"

#define MOTOR_BASE_LF 2
#define MOTOR_BASE_LB 1
#define MOTOR_BASE_RF 3
#define MOTOR_BASE_RB 4
#define LIFT_RT 5
#define LIFT_RB 6
#define LIFT_LT 7
#define LIFT_LB 8
#define CLAW 9
int THRESH;

void drive(int A2, int A3);

void lift(int U6, int D6);

void claw(int U5, int D5);

#endif
