#include "variables.h"
#include "main.h"

int THRESH = 20;
void drive(int A1, int A3) {
  if (A1 == 0) {
    motorSet(MOTOR_BASE_LB, -A3);
    motorSet(MOTOR_BASE_LF, A3);
    motorSet(MOTOR_BASE_RB, -A3);
    motorSet(MOTOR_BASE_RF, -A3);
  }
  else {
    motorSet(MOTOR_BASE_LB, -A1);
    motorSet(MOTOR_BASE_LF, A1);
    motorSet(MOTOR_BASE_RB, A1);
    motorSet(MOTOR_BASE_RF, A1);
  }
}

void testAuton() {
    //Move lift down
    /*lift(-127, 127);
    delay(600);
    lift(0, 0);

    //Drive forward
    drive(0, 127);
    delay(2200);
    drive(0, 0);
    lift(127, -127);
    delay(600);
    lift(0, 0);

    //Move back
    drive(0, -127);
    delay(1000);
    drive(0, 0);

    //10 pt auton
    //drive(127, 0); //L
    drive(-127,0); //R
    delay(1500);
    drive(0, 127);
    //600
    delay(400);
    drive(0, 0);

    //Drop mobile goal
    lift(-127, 127);
    delay(200);
    lift(0, 0);

    //Pull out
    drive(0, -127);
    delay(600);
    drive(0, 0);*/
}

void lift(int U6, int D6) {
  int potRead = analogRead(POT);
  delay(100);
  printf("%d ", potRead);
  if ((potRead < 370 && U6 > D6) || (potRead > 10 && D6 > U6)) {
    motorSet(LIFT_L, (U6-D6)*65);
    motorSet(LIFT_R, -(U6-D6)*65);
  }
  else {
    motorSet(LIFT_L, 0);
    motorSet(LIFT_R, 0);
  }
}