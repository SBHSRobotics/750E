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

int A1, A3, D6, U6, U7;
//int U7;
//bool slowDrive;
void operatorControl() {
//  slowDrive = false;
  while(true){
      int THRESH = 20;
      if((abs)(joystickGetAnalog(1, 1)) > THRESH) {
        A1 = joystickGetAnalog(1, 1);
      }
      else A1 = 0;
      if((abs)(joystickGetAnalog(1, 3)) > THRESH) {
        A3 = joystickGetAnalog(1, 3);
      }
      else A3 = 0;
      D6 = joystickGetDigital(1, 6, JOY_DOWN);
      U6 = joystickGetDigital(1, 6, JOY_UP);
      
      U7 = joystickGetDigital(1, 7, JOY_UP);

      if (U7) {
        testAuton();
      }
      /*U7 = joystickGetDigital(1, 7, JOY_UP);
      if(U7 == 1){
        if(slowDrive) {
          slowDrive = false;
        }
        else slowDrive = true;
      }

      if(slowDrive) {
        A2 = 60*A2/(abs)(A2);
        A3 = 60*A3/(abs)(A3);
      }*/
      drive(A1, A3);

      lift(U6, D6);
  }
}
