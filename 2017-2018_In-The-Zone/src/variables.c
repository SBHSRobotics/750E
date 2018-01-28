#include "variables.h"
#include "main.h"

int THRESH = 20;
void drive(int A1, int A3) {
  if (A1 == 0) {
    motorSet(MOTOR_BASE_LB, -A3);
    motorSet(MOTOR_BASE_LM, -A3);
    motorSet(MOTOR_BASE_LF, A3);
    motorSet(MOTOR_BASE_RB, -A3);
    motorSet(MOTOR_BASE_RM, A3);
    motorSet(MOTOR_BASE_RF, -A3);
  }
  else {
    motorSet(MOTOR_BASE_LB, -A1);
    motorSet(MOTOR_BASE_LM, -A1);
    motorSet(MOTOR_BASE_LF, A1);
    motorSet(MOTOR_BASE_RB, A1);
    motorSet(MOTOR_BASE_RM, -A1);
    motorSet(MOTOR_BASE_RF, A1);
  }
}

void testAuton() {
    lift(-127, 127);
    delay(600);
    lift(0, 0);

    drive(0, 127);
    delay(2400);
    drive(0, 0);
    lift(127, -127);
    delay(300);
    lift(0, 0);

    //10 pt auton
    drive(127, 0); //L
    //drive(-127,0); //R
    delay(1400);
    drive(0, 127);
    delay(1300);

    lift(-127, 127);
    delay(200);
    lift(0, 0);
    drive(0, 0);
}

void lift(int U6, int D6) {
    motorSet(LIFT_L, (U6-D6)*65);
    motorSet(LIFT_R, -(U6-D6)*65);
}