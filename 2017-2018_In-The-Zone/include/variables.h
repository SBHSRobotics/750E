#ifndef INCLUDE_VARIABLES_H_
#define INCLUDE_VARIABLES_H_

#include "main.h"

const unsigned char drive_frontLeft = 1;
const unsigned char drive_backLeft = 2;
const unsigned char drive_frontRight = 3;
const unsigned char drive_backRight = 4;
//const unsigned char lift_left = 5;
//const unsigned char tank_right = 6;

void drive(int x, int y);
void joystickAxes(int x);

#endif
