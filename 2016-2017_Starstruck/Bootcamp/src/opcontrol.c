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

#include "main.h"

#define LB 8
#define RB 2
#define LF 1
#define RF 10

#define ARM 3
#define CLAW 5

void driveOp(){
    // Begin holo code
    int thresh = 20;
    int ch1 = joystickGetAnalog(1, 1);
    int ch2 = joystickGetAnalog(1, 2);
    int ch3 = joystickGetAnalog(1, 3);
    int ch4 = joystickGetAnalog(1, 4);

    printf("ch1: %d\t ch2: %d\t ch3: %d\t ch4: %d\n",ch1,ch2,ch3,ch4);

    // if ((abs(ch3) > thresh) || (abs(ch4) > thresh) || (abs(ch2) > thresh) || (abs(ch1) > thresh)) {
      if (abs(ch3) < thresh) {
        ch3 = 0;
      }
      if (abs(ch4) < thresh) {
        ch4 = 0;
      }
      if (abs(ch2) < thresh) {
        ch2 = 0;
      }
      if (abs(ch1) < thresh) {
        ch1 = 0;
      }
      if (abs(ch3) > abs(ch2)) {
        ch2 = 0;
      }
      else {
        ch3 = 0;
      }

      motorSet(LB, -ch3 + ch2 - ch1 + ch4);
      motorSet(LF, -(ch3 + ch2 + ch4 + ch1));
      motorSet(RB, (ch3 + ch2 - ch1 + ch4));
      motorSet(RF, -(ch3 + ch2 - ch4 - ch1));

    // } else {
    //   stopDrive();
    // }
    printf("LF: %d\t LB: %d\t RF: %d\t RB: %d\n",motorGet(LF),motorGet(LB),motorGet(RF),motorGet(RB));
    // End holo code
  }

  void stopDrive(){
    motorStop(LB);
    motorStop(LF);
    motorStop(RB);
    motorStop(RF);
  }
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
void operatorControl() {
	while (1) {
		motorSet(1,-joystickGetAnalog(1,3));
		motorSet(3,joystickGetAnalog(1,3));
		motorSet(5,-joystickGetAnalog(1,3));
		motorSet(2,joystickGetAnalog(1,2));
		motorSet(4,-joystickGetAnalog(1,2));
		motorSet(6,joystickGetAnalog(1,2));
		// driveOp();
		// motorSet(3, joystickGetDigital(1, 5, JOY_UP) ? 127 : joystickGetDigital(1, 5, JOY_DOWN) ? -127 : 0);
		// motorSet(6, joystickGetDigital(1, 6, JOY_UP) ? 127 : joystickGetDigital(1, 6, JOY_DOWN) ? -127 : 0);
		delay(20);
	}
}
