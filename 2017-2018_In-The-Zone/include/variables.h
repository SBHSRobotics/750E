#ifndef INCLUDE_VARIABLES_H_
#define INCLUDE_VARIABLES_H_

#include "main.h"

#define MOTOR_BASE_LF 2
#define MOTOR_BASE_LB 5
#define MOTOR_BASE_RF 3
#define MOTOR_BASE_RB 4
#define LIFT_L 6
#define LIFT_R 7

int fileNumber;

void drive(int A2, int A3);

void lift(int U6, int D6);

void claw(int U5, int D5);

void moveFunction();

#endif
