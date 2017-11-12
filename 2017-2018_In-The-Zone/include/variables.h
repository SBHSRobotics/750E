#ifndef INCLUDE_VARIABLES_H_
#define INCLUDE_VARIABLES_H_

#include "main.h"

#define drive_frontLeft 2
#define drive_backLeft 3
#define drive_backRight 4
#define drive_frontRight 5
#define liftBot 6
#define liftTop 7
#define claw 8
int THRESH = 20;

void drive(int l, int r);
void lift(bool u, bool d);
void clawFunct(bool u, bool d);
void slowDrive(int l, int r);
int sgn(int i);
int joystickAxes(int ch);

#endif
