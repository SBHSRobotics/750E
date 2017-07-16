#include "variables.h"
#include "main.h"

void drive(int ch1, int ch2, int ch3, int ch4){//a corresponds to the individual analog button axes
  motorSet(MOTOR_BASE_LEFT1_, ch3 + ch2 + ch4 + ch1);
  motorSet(MOTOR_BASE_LEFT2_, ch3 + ch2 - ch4 + ch1);
  motorSet(MOTOR_BASE_RIGHT1_, -(ch3 + ch2 - ch4 - ch1));
  motorSet(MOTOR_BASE_RIGHT2_, -(ch3 + ch2 + ch4 - ch1));
}
/*void lift(int speed) {
  motorSet(MOTOR_LIFT_LEFT_, -speed);
  motorSet(MOTOR_LIFT_RIGHT_, speed);
}
void pince(int speed) {
  motorSet(MOTOR_PINCE_RIGHT_, -speed);
  motorSet(MOTOR_PINCE_RIGHT_, speed);
}*/
