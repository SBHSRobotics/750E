//================================================================//
//  Robot-independent modules   (NOTE: Will not compile directly) //
//        Add general utility functions here.                     //
//================================================================//


//=== Joystick Control Abstraction ===//


/* Structure to store analog values from a joystick */
typedef struct{
  int lx; //Left stick x-axis
  int ly; //Left stick y-axis
  int rx; //Right stick x-axis
  int ry; //Right stick y-axis
}AnalogChannel;


/* Create an AnalogChannel struct for each joystick */
AnalogChannel joyMain,joyPartner;


/* Refresh joystick control structures */
void joyRefreshAnalog() {
  joyMain.lx = vexRT[Ch4];
  joyMain.ly = vexRT[Ch3];
  joyMain.rx = vexRT[Ch1];
  joyMain.ry = vexRT[Ch2];
  joyPartner.lx = vexRT[Ch4Xmtr2];
  joyPartner.ly = vexRT[Ch3Xmtr2];
  joyPartner.rx = vexRT[Ch1Xmtr2];
  joyPartner.ry = vexRT[Ch2Xmtr2];
}


//=== LCD Debugging ===//


/* Clear the LCD Screen */
void lcdClear() {
  clearLCDLine(0);
  clearLCDLine(1);
}


//=== Motor Control and Testing ===//


/* Sets a motor to specified speed. Stops motor if @param speed is omitted */
void motorSet(tMotor index, int speed = 0) {
  motor[index] = speed;
}


/* Runs a motor 0.5sec in each direction to make sure it's working */
void pulseMotor(tMotor motor) {
  StopTask(usercontrol);
  motorSet(motor,127);
  wait1Msec(250);
  motorSet(motor,-127);
  wait1Msec(250);
  motorSet(motor);
  StartTask(usercontrol);
}


/* Pulses all motors and verifies that sensors are connected */
void powerOnSelfTest() {
  lcdClear();
  displayLCDString(0,0,"~Running POST...");
  displayLCDString(0,1,"DO NOT USE CTRL!");
  pulseMotor(port1);
  pulseMotor(port2);
  pulseMotor(port3);
  pulseMotor(port4);
  pulseMotor(port5);
  pulseMotor(port6);
  pulseMotor(port7);
  pulseMotor(port8);
  pulseMotor(port9);
  pulseMotor(port10);
  boop(); beep(); boop();
  //TODO: Check sensors before and after 1s delay and make sure values are the same (this will tell if a sensor is connected or not)
}
