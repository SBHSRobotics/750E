/* Robot-Independent Library Header File */
#ifndef ROBOT_LIB_EVO_H_
#define ROBOT_LIB_EVO_H_
/* Structure to store analog values from a joystick */
typedef struct{
  int lx; //Left stick x-axis
  int ly; //Left stick y-axis
  int rx; //Right stick x-axis
  int ry; //Right stick y-axis
}AnalogChannel;


/* Create an AnalogChannel struct for each joystick */
AnalogChannel joyMain,joyPartner;

//Functions

void joyRefreshAnalog();
void pulseMotor(unsigned char);
void powerOnSelfTest();

#endif
