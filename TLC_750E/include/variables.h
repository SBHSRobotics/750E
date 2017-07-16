#ifndef INCLUDE_VARIABLES_H_
#define INCLUDE_VARIABLES_H_

#include "main.h"

#define MOTOR_BASE_LEFT1_ 4
#define MOTOR_BASE_LEFT2_ 5
#define MOTOR_BASE_RIGHT1_ 2
#define MOTOR_BASE_RIGHT2_ 3
#define MOTOR_LIFT_LEFT_ 6
#define MOTOR_LIFT_RIGHT_ 7
<<<<<<< HEAD
#define MOTOR_PINCE_LEFT_ 8
#define MOTOR_PINCE_RIGHT_ 9
=======
#define MOTOR_PINCE_ 9
>>>>>>> 00e4c2d3fa23ede678178f28594b597f2d82eda4
#define BASE_ENC_RIGHT_TOP 1
#define BASE_ENC_RIGHT_BOTTOM 2
#define BASE_ENC_LEFT_TOP 3
#define BASE_ENC_LEFT_BOTTOM 4

void drive(int ch1, int ch2, int ch3, int ch4);
void lift(int speed);
void pince(int speed);
void stopMotors();
#endif
