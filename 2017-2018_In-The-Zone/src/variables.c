#include "variables.h"
#include "main.h"

void drive (int x, int y) {
    motorSet(drive_frontLeft, x);
    motorSet(drive_backLeft, x);
    motorSet(drive_frontRight, y);
    motorSet(drive_backRight, -y);
}

int joystickAxes (int x) {
  if (abs(joystickGetAnalog(1, x)) > 15) {
    return joystickGetAnalog(1, x);
  } else {
    return 0;
  }
}
