#include "variables.h"
#include "main.h"

void drive(int ch1, int ch2) {
  motorSet(MOTOR_BASE_LEFT_, ch1 + ch2);
  motorSet(MOTOR_BASE_RIGHT_, -(ch1 - ch2));
}
