#ifndef INCLUDE_VARIABLES_H_
#define INCLUDE_VARIABLES_H_

#include "main.h"

#define drive_frontLeft 3
#define drive_backLeft 2
#define drive_backRight 4
#define drive_frontRight 5

//const unsigned char drive_frontLeft 2;
//const unsigned char drive_backLeft 3;
//const unsigned char drive_backRight 4;
//const unsigned char drive_frontRight 5;
//const unsigned char lift_left = 6;
//const unsigned char tank_right = 7;

void drive(int x, int y);
int joystickAxes(int x);

#endif
