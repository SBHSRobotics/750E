#ifndef INCLUDE_VARIABLES_H_
#define INCLUDE_VARIABLES_H_

#include "main.h"

#define MOTOR_BASE_RIGHT1_ 2
#define MOTOR_BASE_RIGHT2_ 3
#define MOTOR_BASE_LEFT1_ 4
#define MOTOR_BASE_LEFT2_ 5
#define MOTOR_LIFT_LEFT_ 6
#define MOTOR_LIFT_RIGHT_ 7
#define MOTOR_PINCE_LEFT_ 8
#define MOTOR_PINCE_RIGHT_ 9
#define ENCODER_LEFT1_TOP_ 1
#define ENCODER_RIGHT1_TOP_ 2
#define ENCODER_LEFT1_BOT_ 3
#define ENCODER_RIGHT1_BOT_ 4
#define ENCODER_LEFT2_TOP_ 5
#define ENCODER_RIGHT2_TOP_ 6
#define ENCODER_LEFT2_BOT_ 7
#define ENCODER_RIGHT2_BOT_ 8

Encoder encoderLeft1;
Encoder encoderRight1;
Encoder encoderLeft2;
Encoder encoderRight2;

void drive(int ch1, int ch2, int ch3, int ch4);
void lift(int speed);
void pince(int speed);
#endif
