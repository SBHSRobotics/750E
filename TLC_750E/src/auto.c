/** @file auto.c
 * @brief File for autonomous code
 *
 * This file should contain the user autonomous() function and any functions related to it.
 *
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/
 *
 * PROS contains FreeRTOS (http://www.freertos.org) whose source code may be
 * obtained from http://sourceforge.net/projects/freertos/files/ or on request.
 */

#include "main.h"
#include "variables.h"


<<<<<<< HEAD
const int turnSpeed = 1;
=======
const int turnSpeed = 180;
>>>>>>> 00e4c2d3fa23ede678178f28594b597f2d82eda4
/*
 * Runs the user autonomous code. This function will be started in its own task with the default
 * priority and stack size whenever the robot is enabled via the Field Management System or the
 * VEX Competition Switch in the autonomous mode. If the robot is disabled or communications is
 * lost,  the autonomous task will be stopped by the kernel. Re-enabling the robot will restart
 * the task, not re-start it from where it left off.
 *
 * Code running in the autonomous task cannot access information from the VEX Joystick. However,
 * the autonomous function can be invoked from another task if a VEX Competition Switch is not
 * available, and it can access joystick information if called in this way.
 *
 * The autonomous task may exit, unlike operatorControl() which should never exit. If it does
 * so, the robot will await a switch to another mode or disable/enable cycle.
 */

//+ = fwd, - = bckwd
void moveY(int mod) {
  motorSet(MOTOR_BASE_LEFT1_, mod*127);
  motorSet(MOTOR_BASE_LEFT2_, mod*127);
  motorSet(MOTOR_BASE_RIGHT1_, mod*127);
  motorSet(MOTOR_BASE_RIGHT2_, mod*127);
}
//+ = rght, - = lft
void moveX(int mod) {
  motorSet(MOTOR_BASE_LEFT1_, mod*127);
  motorSet(MOTOR_BASE_LEFT2_, -mod*127);
  motorSet(MOTOR_BASE_RIGHT1_, -mod*127);
  motorSet(MOTOR_BASE_RIGHT2_, mod*127);
}
//+ = rght, - = lft
void turn(int mod) {
  motorSet(MOTOR_BASE_LEFT1_, mod*127);
  motorSet(MOTOR_BASE_LEFT2_, mod*127);
  motorSet(MOTOR_BASE_RIGHT1_, -mod*127);
  motorSet(MOTOR_BASE_RIGHT2_, -mod*127);
}
<<<<<<< HEAD
=======

>>>>>>> 00e4c2d3fa23ede678178f28594b597f2d82eda4
int turnDur(int deg) {
  return deg/turnSpeed;
}
//+ = up, - = dwn
void lift(int mod) {
  motorSet(MOTOR_LIFT_LEFT_, mod*127);
<<<<<<< HEAD
  motorSet(MOTOR_LIFT_RIGHT, mod*127)
}

void pince(int mod) {
  motorSet(MOTOR_PINCE_LEFT_, mod*127);
  motorSet(MOTOR_PINCE_RIGHT_, mod*127);
=======
  motorSet(MOTOR_LIFT_RIGHT_, mod*127);
}

void pince(int mod) {
  motorSet(MOTOR_PINCE_, mod*127);
>>>>>>> 00e4c2d3fa23ede678178f28594b597f2d82eda4
}

void delayAndStop(int del) {
  delay(del);
  stopMotors();
}
/*Here is the pinnacle of shitty auton. Due to not using any sensors
 *in our build, this is our alternative to it
 */
void autonomous() {
<<<<<<< HEAD
  /*moveY(1);
  delayAndStop(3200);
  turn(-1);
  delayAndStop(turnDur(45));
  moveY(1);
  delayAndStop(2200);
  lift(1);
  delayAndStop(1000);
  moveY(1);
  delayAndStop(500);
  moveY(-1);
  delayAndStop(2700);
  turn(1);
  delayAndStop(turnDur(45));
  moveY(1);
  delayAndStop(3200);*/
  recorder rec = new recorder();
=======
  for (int i = 0; i < 4; i++) {
    moveY(1);
    delayAndStop(2500);

    turn(-1);
    delayAndStop(turnDur(90));
  }

  lift(1);
  delayAndStop(1000);

  pince(1);
  delayAndStop(500);
  pince(-1);
  delayAndStop(500);
>>>>>>> 00e4c2d3fa23ede678178f28594b597f2d82eda4
}
