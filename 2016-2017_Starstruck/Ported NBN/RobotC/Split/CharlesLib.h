//========================================================//
//         Charles Competition Library v2015.1.19
//========================================================//

//Tasks (Each task runs simultaneously in its own thread)

task shooterPID(); //PID loop for shooter wheels

//TODO: Update

//Functions

int speedLfromRPM(int i);
int speedRfromRPM(int i);
void joystickMap();
void beep();
void boop();
void soundSpeedUp();
void soundSpeedDown();


//Global Vars

int	shooter = 0, shooterOld = 0;//Shooter speed (tps)

float Kp = 300.0;
float bsFix = 2.0;

float speedL = 0;
float speedR = 0;//Calculated right flywheel speed (-127 to 127)

float errL = 0,
			errR = 0;


//int SPEED_CONV_FACTOR = 300;
int PID_THRESH = 30;

int timeElapsed = 0;

int driveX = 0,
		driveY = 0,
		driveH = 0; //Intake speed (-127 to 127)

int AUTON_PID_THRESH = 5;
