#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, in1,    L_L,            sensorReflection)
#pragma config(Sensor, in2,    L_M,            sensorReflection)
#pragma config(Sensor, in3,    L_R,            sensorReflection)
#pragma config(Sensor, dgtl8,  SONAR,          sensorSONAR_cm)
#pragma config(Sensor, I2C_1,  IME_BR,         sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Sensor, I2C_2,  IME_FR,         sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Sensor, I2C_3,  IME_BL,         sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Sensor, I2C_4,  IME_FL,         sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Sensor, I2C_5,  IME_SLIDE,      sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Sensor, I2C_6,  IME_ARM,        sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Sensor, I2C_7,  IME_CON,        sensorNone)
#pragma config(Sensor, I2C_8,  IME_S,          sensorNone)
#pragma config(Motor,  port1,           W_FR,          tmotorVex393, openLoop)
#pragma config(Motor,  port2,           W_BR,          tmotorVex393, openLoop)
#pragma config(Motor,  port3,           W_FL,          tmotorVex393, openLoop)
#pragma config(Motor,  port4,           W_BL,          tmotorVex393, openLoop)
#pragma config(Motor,  port5,           SLIDE_R,       tmotorVex393, openLoop)
#pragma config(Motor,  port6,           SLIDE_L,       tmotorVex393, openLoop, encoder, encoderPort, I2C_5, 1000)
#pragma config(Motor,  port7,           ARM,           tmotorVex393, openLoop, encoder, encoderPort, I2C_6, 1000)
#pragma config(Motor,  port8,           S_L,           tmotorVex393, openLoop)
#pragma config(Motor,  port9,           S_R,           tmotorVex393, openLoop)
#pragma config(Motor,  port10,          CON,           tmotorVex393, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//Macros for index parameters (To be used in main task logic)
#define iW_FR 1
#define iW_BR 2
#define iW_FL 3
#define iW_BL 4
#define iSLIDE_R 5
#define iSLIDE_L 6
#define iARM 7
#define iS_L 8
#define iS_R 9
#define iCON 10

//Convert index to corresponding sensor parameter (for looping)
tSensors pidSensorIndex(int i) {
  return (i==1)?IME_FR:
         (i==2)?IME_BR:
         (i==3)?IME_FL:
         (i==4)?IME_BL:
         (i==5)?IME_SLIDE:
         (i==6)?IME_SLIDE:
         (i==7)?IME_ARM:
         (i==8)?IME_S:
         (i==9)?IME_S:
         (i==10)?IME_CON:
         dgtl12;
}



//Convert index to corresponding motor parameter (for looping)
tMotor pidMotorIndex(int i) {
  return (i==1)?W_FR:
         (i==2)?W_BR:
         (i==3)?W_FL:
         (i==4)?W_BL:
         (i==5)?SLIDE_R:
         (i==6)?SLIDE_L:
         (i==7)?ARM:
         (i==8)?S_L:
         (i==9)?S_R:
         (i==10)?CON:
         dgtl12;
}

//PID Constants
#define PID_SENSOR_SCALE    1
#define PID_MOTOR_SCALE     -1

#define PID_DRIVE_MAX       127
#define PID_DRIVE_MIN     (-127)

#define PID_INTEGRAL_LIMIT  50

//The following PID algorithm is based on JPearman's example code on the forums.

//Left as global vars for live editing - TODO: Encapsulate once tuned?
float  pid_Kp[10];
float  pid_Ki[10];
float  pid_Kd[10];

static float pidTarget[10]; //TODO: Why static? Jp prbbly had reason
static int   pidRunning = 1;

//Set gain constants
void pidTune(int index, float Kp, float Ki, float Kd) {
    pid_Kp[index-1] = Kp;
    pid_Ki[index-1] = Ki;
    pid_Kd[index-1] = Kd;
}

//Set target value for given motor index
void pidSetTarget(int index, float target) {
  pidTarget[index-1] = target;
}

//Increment target value for given motor (cannot set value after current is non-zero)
void pidMoveTarget(int index, float target) {
  pidTarget[index-1] = pidTarget[index-1]+target;
}

void pidMoveChassis(float left, float right) {
		pidMoveTarget(iW_FR,-right);
		pidMoveTarget(iW_BR,-right);
		pidMoveTarget(iW_FL,left);
		pidMoveTarget(iW_BL,left);
}

//Initialize the PID loop variables and data structures
void init() {
  for(int i = 1; i<=10; i++) {//For each motor index
    pidTune(i, 2.0, 0.04, 0);//Set the gain constants for the PID loops
    pidSetTarget(i, 0);//Initialize target

    // Clear encoders TODO: Clear all sensors?
    if( SensorType[ pidSensorIndex(i) ] == sensorQuadEncoder )
        SensorValue[ pidSensorIndex(i) ] = 0;
  }
}


//=====================================================//
//              Le PID Task                            //
//=====================================================//

task pidController() {
	//Declarations
    float  pidSensorCurrentValue;

    float  pidError = 0.0;
    float  pidLastError = 0.0;
    float  pidIntegral = 0.0;
    float  pidDerivative = 0.0;
    float  pidDrive = 0.0;

    tSensors PID_SENSOR_INDEX;
    tMotor PID_MOTOR_INDEX;

    float  Kp;
    float  Ki;
    float  Kd;

    while( true ) {
        if( pidRunning ) {
            for(int i = 5; i<=7; i++) {//Loop through first 7 ports to run PID logic
                PID_MOTOR_INDEX = pidMotorIndex(i);
                PID_SENSOR_INDEX = pidSensorIndex(i);
                Kp = pid_Kp[i];
                Ki = pid_Ki[i];
                Kd = pid_Kd[i];

                // Read the sensor value and scale
                pidSensorCurrentValue = SensorValue[ PID_SENSOR_INDEX ] * PID_SENSOR_SCALE;

                // calculate error
                pidError = pidSensorCurrentValue - pidTarget[i-1];

                // integral - if Ki is not 0
                if( Ki != 0 ) {
                    // If we are inside controlable window then integrate the error
                    if( abs(pidError) < PID_INTEGRAL_LIMIT )
                        pidIntegral = pidIntegral + pidError;
                    else
                        pidIntegral = 0;
                    }
                else
                    pidIntegral = 0;

                // calculate the derivative
                pidDerivative = pidError - pidLastError;
                pidLastError  = pidError;

                // calculate drive
                pidDrive = (Kp * pidError) + (Ki * pidIntegral) + (Kd * pidDerivative);

                // limit drive
                if( pidDrive > PID_DRIVE_MAX )
                    pidDrive = PID_DRIVE_MAX;
                if( pidDrive < PID_DRIVE_MIN )
                    pidDrive = PID_DRIVE_MIN;

                // send to motor
                motor[ PID_MOTOR_INDEX ] = pidDrive * PID_MOTOR_SCALE;
            }

        } else {
            // clear all
            pidError      = 0;
            pidLastError  = 0;
            pidIntegral   = 0;
            pidDerivative = 0;
            motor[ PID_MOTOR_INDEX ] = 0;
        }

        // Run at 50Hz
        wait1Msec( 25 );
    }
}

//Move chassis at given speed for ms milliseconds
void move(int left,int right,int ms){
		motor[W_BL]=left;
		motor[W_FL]=left;
		motor[W_BR]=-right;
		motor[W_FR]=-right;
		wait1Msec(ms);
		motor[W_BL]=0;
		motor[W_FL]=0;
		motor[W_BR]=0;
		motor[W_FR]=0;
}

//Pulse the conveyor forward
void pulseConveyor(int speed = 127) {
	  motor[CON] = -speed;
    wait10Msec(20);
    motor[CON] = 0;
    wait10Msec(50);
}

//Set or stop shooter speed
void shooter(int speed=0) {
	motor[S_L] = speed;
	motor[S_R] = -speed;
}
//=====================================================//
//              Le Main Task                           //
//=====================================================//

task main() {
 		init();
 		StartTask(pidController);//Start PID Control

 		pidSetTarget(iARM,-169);//Move arm up to tile shot position
 		move(-127,127,100);//Turn towards net
 		wait1Msec(100);
 		shooter(100);//Run shooter for first ball
 		motor[CON] = 127;//Start conveyor to spit out preload 1
 		wait1Msec(100);
 		motor[CON] = -127;//Spit out second and third preloads
 		shooter(127);//Increase shooter to full speed
 		wait1Msec(1000);
 		motor[CON] = 127;//Set conveyor for preloads

 		wait1Msec(30000);//Wait 30 seconds

 		motor[CON] = 127;//Set conveyor again for good luck
 		move(127,-127,658);//Turn right
 		pidSetTarget(iARM,5);//Bring down arm
 		shooter(-127);//Reverse for intake (Thank God this isn't a real robot or the gears would skip to oblivion)
 		move(127,127,3800);//Move to blue tile
 		pidSetTarget(iARM, -171);//Bring arm back up
 		move(-127,127,583);//Turn towards net
 		wait1Msec(1000);
 		shooter(127);//Fire first field load
 		wait1Msec(500);
 		pidSetTarget(iARM, -169);//Bring arm down a little
 		wait1Msec(100);
 		shooter(127);//Set speed for preloads
		while(true); //Hold motor speeds til end of match

}
