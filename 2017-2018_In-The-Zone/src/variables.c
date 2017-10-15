#include "variables.h"
#include "main.h"

void drive (int x, int y) {
  if (abs(x) > abs(y)) {
    motorSet(drive_frontLeft, x);
    motorSet(drive_backLeft, x);
    motorSet(drive_frontRight, x);
    motorSet(drive_backLeft, x);
  } else {
    motorSet(drive_frontLeft, y);
    motorSet(drive_backLeft, y);
    motorSet(drive_frontRight, -y);
    motorSet(drive_backLeft, -y);
  }
}

int joystickAxes (int x) {
  if (abs(joystickGetAnalog(1, x)) > 5) {
    return joystickGetAnalog(1, x);
  } else {
    return 0;
  }
}
