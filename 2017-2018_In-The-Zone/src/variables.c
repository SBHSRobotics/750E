#include "variables.h"
#include "main.h"

int THRESH = 20;
void drive(int A2, int A3) {
  motorSet(MOTOR_BASE_LB, -A3);
  motorSet(MOTOR_BASE_LF, A3);
  motorSet(MOTOR_BASE_RB, -A2);
  motorSet(MOTOR_BASE_RF, -A2);
}

void lift(int U6, int D6) {
    motorSet(LIFT_LB, (U6-D6)*65);
    motorSet(LIFT_LT, (U6-D6)*65);
    motorSet(LIFT_RB, -(U6-D6)*65);
    motorSet(LIFT_RT, -(U6-D6)*65);
}

void claw(int U5, int D5) {
  motorSet(CLAW, -(U5-D5)*127);
}
