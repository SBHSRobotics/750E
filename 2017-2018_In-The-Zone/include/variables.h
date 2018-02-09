#ifndef INCLUDE_VARIABLES_H_
#define INCLUDE_VARIABLES_H_

#include "main.h"

#define MOTOR_BASE_LF 2
#define MOTOR_BASE_LB 3
#define MOTOR_BASE_RF 4
#define MOTOR_BASE_RB 5
#define LIFT_L 6
#define LIFT_R 7

#define POT 1

void drive(int A1, int A3);

void testAuton();

void lift(int U6, int D6);

#endif
