//================================================================//
//  Robot-independent modules   (NOTE: Will not compile directly) //
//        Add general utility functions here.                     //
//================================================================//

#include "main.h"

/* Refresh joystick control structures */
void joyRefreshAnalog() {
  joyMain.lx = joystickGetAnalog(1,4);
  joyMain.ly = joystickGetAnalog(1,3);
  joyMain.rx = joystickGetAnalog(1,1);
  joyMain.ry = joystickGetAnalog(1,2);
  joyPartner.lx = joystickGetAnalog(2,4);
  joyPartner.ly = joystickGetAnalog(2,3);
  joyPartner.rx = joystickGetAnalog(2,1);
  joyPartner.ry = joystickGetAnalog(2,2);
}

//=== Motor Control and Testing ===//

/* Runs a motor 0.5sec in each direction to make sure it's working */
void pulseMotor(unsigned char motor) {
  motorSet(motor,127);
  delay(250);
  motorSet(motor,-127);
  delay(250);
  motorSet(motor,0);
}


/* Pulses all motors and verifies that sensors are connected */
void powerOnSelfTest() {
  lcdClear(uart1);
  lcdPrint(0,0,"~Running POST...");
  lcdPrint(0,1,"DO NOT USE CTRL!");
  pulseMotor(1);
  pulseMotor(2);
  pulseMotor(3);
  pulseMotor(4);
  pulseMotor(5);
  pulseMotor(6);
  pulseMotor(7);
  pulseMotor(8);
  pulseMotor(9);
  pulseMotor(10);
  //TODO: Check sensors before and after 1s delay and make sure values are the same (this will tell if a sensor is connected or not)
}
