/** @file opcontrol.c
 * @brief File for operator control code
 *
 * This file should contain the user operatorControl() function and any functions related to it.
 *
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/
 *
 * PROS contains FreeRTOS (http://www.freertos.org) whose source code may be
 * obtained from http://sourceforge.net/projects/freertos/files/ or on request.
 */

#include "variables.h"
#include "main.h"

/*
 * Runs the user operator control code. This function will be started in its own task with the
 * default priority and stack size whenever the robot is enabled via the Field Management System
 * or the VEX Competition Switch in the operator control mode. If the robot is disabled or
 * communications is lost, the operator control task will be stopped by the kernel. Re-enabling
 * the robot will restart the task, not resume it from where it left off.
 *
 * If no VEX Competition Switch or Field Management system is plugged in, the VEX Cortex will
 * run the operator control task. Be warned that this will also occur if the VEX Cortex is
 * tethered directly to a computer via the USB A to A cable without any VEX Joystick attached.
 *
 * Code running in this task can take almost any action, as the VEX Joystick is available and
 * the scheduler is operational. However, proper use of delay() or taskDelayUntil() is highly
 * recommended to give other tasks (including system tasks such as updating LCDs) time to run.
 *
 * This task should never exit; it should end with some kind of infinite loop, even if empty.
 */


int ch2, ch3;
bool ch5u, ch5d, ch6u, ch6d, ch7l, slowToggle;
slowToggle = false;

void operatorControl() {
  while(true) {
    ch2 = joystickAxes(2);
    ch3 = joystickAxes(3);
    ch5u = joystickGetDigital(1, 5, JOY_UP);
    ch5d = joystickGetDigital(1, 5, JOY_DOWN);
    ch6u = joystickGetDigital(1, 6, JOY_UP);
    ch6d = joystickGetDigital(1, 6, JOY_DOWN);
    ch7l = joystickGetDigital(1, 7, JOY_LEFT);

    if (ch7l) {
      if (slowToggle) {
        slowToggle = false;
      } else {
        slowToggle = true;
      }
    }

    if (slowToggle) {
      slowDrive(ch3, ch2);
    } else {
      drive(ch3, ch2);
    }

    if (ch6u || ch6d) {
      lift(ch6u, ch6d);
    }
    if (ch5u || ch5d) {
      clawFunct(ch5u, ch5d);
    }
  }
}
