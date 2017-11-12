#include "variables.h"
#include "main.h"

void drive (int l, int r) {
    motorSet(drive_frontLeft, l);
    motorSet(drive_backLeft, -l);
    motorSet(drive_frontRight, r);
    motorSet(drive_backRight, -r);
}

void lift (bool u, bool d) {
    if (u && !d) {
      motorSet(liftBot, 127);
      motorSet(liftTop, 127)
    }
    else if (d && !u) {
      motorSet(liftBot, -127);
      motorSet(liftTop, -127);
    }
}

void clawFunct (bool u, bool d) {
    if (u && !d) {
      motorSet(claw, 127);
    }
    else if (d && !u) {
      motorSet(claw, -127);
    }
}

void slowDrive (int l, int r) {
  motorSet(drive_frontLeft, 60*sgn(l));
  motorSet(drive_backLeft, 60*sgn(-l));
  motorSet(drive_frontRight, 60*sgn(r));
  motorSet(drive_backRight, 60*sgn(-r));
}
int joystickAxes (int ch) {
  if (abs(joystickGetAnalog(1, ch)) > THRESH) {
    return joystickGetAnalog(1, ch);
  } else {
    return 0;
  }
}
int sgn (int i) {
  return ((i > 0) - (i < 0));
}
