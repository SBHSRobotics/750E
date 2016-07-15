#include "main.h"
#include "RobotLib.h"
#include <math.h>

static const double my_PI = 3.14159265358979323846;

int sust = 0;

float armValue;

float Kp_lift = 0.4;
float Ki_lift = 0.0000015;
float Kd_lift = 0.0;
float error_lift = 0;
float lastError_lift = 0;
long lastTime_lift = 0;
float derivative_lift = 0;

int pid_arm(float measurement, float target) {
	long time = micros();
	long elapsedTime = time - lastTime_lift;
	float thisError = measurement - target;
	error_lift += thisError * elapsedTime;
	float dError = (thisError - lastError_lift) / elapsedTime;

	lastTime_lift = time;
	lastError_lift = thisError;

	float proportional = thisError * Kp_lift;
	derivative_lift = dError * Kd_lift;
	if ((abs(proportional) > 40) || (derivative_lift > 0.2)) {
		error_lift = 0;
	}
	float integral = error_lift * Ki_lift;
	//printf("P: %3.3f\t I: %1.10f\t\n\r", proportional, integral);
	return -(proportional + integral + derivative_lift);
}

float Kp_wheels = 0.27;
float Ki_wheels = 0;
float Kd_wheels = 0.0;
float error_wheels = 0;
float lastError_wheels = 0;
long lastTime_wheels = 0;
float derivative_wheels = 0;

int pid_wheels(float measurement, float target) {
	long time = micros();
	long elapsedTime = time - lastTime_wheels;
	float thisError = measurement - target;
	error_wheels += thisError * elapsedTime;
	float dError = (thisError - lastError_wheels) / elapsedTime;

	lastTime_wheels = time;
	lastError_wheels = thisError;

	float proportional = thisError * Kp_wheels;
	derivative_wheels = dError * Kd_wheels;
	if ((abs(proportional) > 20) || (derivative_wheels > 0.2)) {
		error_wheels = 0;
	}
	float integral = error_lift * Ki_wheels;
	//printf("P: %3.3f\t I: %1.10f\t\n\r", proportional, integral);
	return -(proportional + integral + derivative_wheels);
}

void armTo(float value)
{
	error_lift = 0;
	lastError_lift = 0;
	lastTime_lift = 0;
	derivative_lift = 0;
	int motorPower = pid_arm(analogRead(POT_LEFT), value);
	if (motorPower < -127)
	{
		motorPower = -127;
	}
	if (motorPower > 127)
	{
		motorPower = 127;
	}
	bool done = false;
	long triggerTime = 0;
	while (!done)
	{
		if (joystickGetDigital(1, 8, JOY_LEFT))
		{
			done = true;
		}
		//printf("Potentiometer: %d		Motor: %d\n\r", analogRead(POT_LEFT), motorPower);
		if (triggerTime == 0)
		{
			if (abs(motorPower) < 50)
			{
				triggerTime = millis();
			}
		}
		else
		{
			if (abs(motorPower) < 50)
			{
				if (millis() - triggerTime > 1000) // must hold for 2 seconds
				{
					done = true;
				}
			}
			else
			{
				triggerTime = 0;
			}
		}

		printf("Potentiometer: %d		Motor: %d\n\r", analogRead(POT_LEFT), motorPower);
		motorSet(LB, -motorPower);
		motorSet(LF, -motorPower);
		motorSet(LT, motorPower);
		motorSet(RB, motorPower);
		motorSet(RF, motorPower);
		motorSet(RT, -motorPower);

		int pidd = pid_arm(analogRead(POT_LEFT), value);
		motorPower = pidd;
		if (motorPower < -127)
		{
			motorPower = -127;
		}
		if (motorPower > 127)
		{
			motorPower = 127;
		}
	}
	motorStop(LB);
	motorStop(LF);
	motorStop(LT);
	motorStop(RT);
	motorStop(RB);
	motorStop(RF);
}

void ultrasonicTo(int frontback, float value, float heading)
{
	static const float CONST = 1;

	Kp_wheels = 6.5;
	// Ki_wheels =
	error_wheels = 0;
	lastError_wheels = 0;
	lastTime_wheels = 0;
	derivative_wheels = 0;

	value = value * CONST *
			2.0 * // feet per tile
			12.0 * // inches per feet
			2.54; // centimeters per inch

	int motorPower = -pid_wheels(ultrasonicGet(frontback == 0 ? front : back), value);
	bool done = false;
	long triggerTime = 0;
	while (!done)
	{
		if (joystickGetDigital(1, 8, JOY_LEFT))
		{
			done = true;
		}
		//printf("Potentiometer: %d		Motor: %d\n\r", analogRead(POT_LEFT), motorPower);
		if (triggerTime == 0)
		{
			if (abs(motorPower) < 50)
			{
				triggerTime = millis();
			}
		}
		else
		{
			if (abs(motorPower) < 50)
			{
				if (millis() - triggerTime > 1000) // must hold for 2 seconds
				{
					done = true;
				}
			}
			else
			{
				triggerTime = 0;
			}
		}

		int ultraVal = 0;
		while (ultraVal == 0) {
			ultraVal = ultrasonicGet(frontback == 0 ? front : back);
			delay(1);
		}

		static const float GYRO_CONST = 4;

		printf("value: %3.3f\n\r", value);
		printf("Ultrasonic: %d		Motor: %d    Gyro: %d\n\r", ultraVal, motorPower, gyroGet(gyro));
		motorSet(FL, motorPower - GYRO_CONST * gyroGet(gyro));
		motorSet(FR, -motorPower + GYRO_CONST * gyroGet(gyro));
		motorSet(BL, motorPower - GYRO_CONST * gyroGet(gyro));
		motorSet(BR, -motorPower + GYRO_CONST * gyroGet(gyro));

		int pidd = -pid_wheels(ultraVal, value);
		if (abs(pidd - motorPower) < 10)
		{
			motorPower = pidd;
		}
		if (motorPower < -127)
		{
			motorPower = -127;
		}
		if (motorPower > 127)
		{
			motorPower = 127;
		}
	}
	motorStop(FL);
	motorStop(FR);
	motorStop(BL);
	motorStop(BR);
}

void encoderTo(int dir, float value)
{
	static const double CONST = 1.17;
	static const unsigned char WHEEL_RADIUS = 2;
	value = value * CONST *
			2 * // feet per tile
			12 * // inches per feet
			(sqrt(2)) * // on a diagonal
			(1.0f / (2 * my_PI * WHEEL_RADIUS)) * // rotations per distance
			360; // degrees in a rotation

	Kp_wheels = 0.27;
	error_wheels = 0;
	lastError_wheels = 0;
	lastTime_wheels = 0;
	derivative_wheels = 0;
	encoderReset(encoder);
	int motorPower = pid_wheels(abs(encoderGet(encoder)), value);
	if (motorPower < -127)
	{
		motorPower = -127;
	}
	if (motorPower > 127)
	{
		motorPower = 127;
	}
	bool done = false;
	long triggerTime = 0;
	while (!done)
	{
		int blPower = motorPower;
		if (joystickGetDigital(1, 8, JOY_LEFT))
		{
			done = true;
		}
		if (triggerTime == 0)
		{
			if (abs(motorPower) < 50)
			{
				blPower = motorPower + (abs(motorPower) / motorPower) * 10;
				triggerTime = millis();
			}
		}
		else
		{
			if (abs(motorPower) < 50)
			{
				if (millis() - triggerTime > 1000) // must hold for 2 seconds
				{
					done = true;
				}
			}
			else
			{
				blPower = motorPower;
				triggerTime = 0;
			}
		}

		printf("Value: %3.3f\n\r", value);
		printf("Encoder: %d		Motor: %d\n\r", encoderGet(encoder), motorPower);
		if (dir == FORWARD)
		{
			motorSet(FL, motorPower);
			motorSet(FR, -motorPower);
			motorSet(BL, blPower);
			motorSet(BR, -motorPower);
		}
		else if (dir == BACKWARD)
		{
			motorSet(FL, -motorPower);
			motorSet(FR, motorPower);
			motorSet(BL, -blPower);
			motorSet(BR, motorPower);
		}
		else if (dir == RIGHT)
		{
			motorSet(FL, motorPower);
			motorSet(FR, motorPower);
			motorSet(BL, -blPower);
			motorSet(BR, -motorPower);
		}
		else if (dir == LEFT)
		{
			motorSet(FL, -motorPower);
			motorSet(FR, -motorPower);
			motorSet(BL, blPower);
			motorSet(BR, motorPower);
		}
		else if (dir == RIGHT_TURN)
		{
			motorSet(FL, motorPower);
			motorSet(FR, motorPower);
			motorSet(BL, blPower);
			motorSet(BR, motorPower);
		}
		else if (dir == LEFT_TURN)
		{
			motorSet(FL, -motorPower);
			motorSet(FR, -motorPower);
			motorSet(BL, -blPower);
			motorSet(BR, -motorPower);
		}

		int pidd = pid_wheels(abs(encoderGet(encoder)), value);
		motorPower = pidd;
		if (motorPower < -127)
		{
			motorPower = -127;
		}
		if (motorPower > 127)
		{
			motorPower = 127;
		}
	}
	motorStop(FL);
	motorStop(FR);
	motorStop(BL);
	motorStop(BR);
}

void gyroTo(float value){
	Kp_wheels = 1.9;
	error_wheels = 0;
	lastError_wheels = 0;
	lastTime_wheels = 0;
	derivative_wheels = 0;
	int motorPower = pid_wheels(gyroGet(gyro), value);
	bool done = false;
	long triggerTime = 0;
	while (!done)
	{
		if (joystickGetDigital(1, 8, JOY_LEFT))
		{
			done = true;
		}
		//printf("Potentiometer: %d		Motor: %d\n\r", analogRead(POT_LEFT), motorPower);
		if (triggerTime == 0)
		{
			if (abs(motorPower) < 50)
			{
				triggerTime = millis();
			}
		}
		else
		{
			if (abs(motorPower) < 50)
			{
				if (millis() - triggerTime > 1000) // must hold for 2 seconds
				{
					done = true;
				}
			}
			else
			{
				triggerTime = 0;
			}
		}

		printf("Gyro: %d		Motor: %d\n\r", gyroGet(gyro), motorPower);
		motorSet(FL, -motorPower);
		motorSet(FR, -motorPower);
		motorSet(BL, -motorPower);
		motorSet(BR, -motorPower);

		int pidd = pid_wheels(gyroGet(gyro), value);
		if (abs(pidd - motorPower) < 10)
		{
			motorPower = pidd;
		}
		if (motorPower < -127)
		{
			motorPower = -127;
		}
		if (motorPower > 127)
		{
			motorPower = 127;
		}
		delay(20);
	}
	motorStop(FL);
	motorStop(FR);
	motorStop(BL);
	motorStop(BR);
}

void armTask(void *ignore){
	error_lift = 0;
	lastError_lift = 0;
	lastTime_lift = 0;
	derivative_lift = 0;
	int motorPower = pid_arm(analogRead(POT_LEFT), armValue);
	if (motorPower < -127)
	{
		motorPower = -127;
	}
	if (motorPower > 127)
	{
		motorPower = 127;
	}
	while (1)
	{
		if (joystickGetDigital(1, 8, JOY_LEFT))
		{
			break;
		}
		//printf("Potentiometer: %d		Motor: %d\n\r", analogRead(POT_LEFT), motorPower);
		printf("Potentiometer: %d		Motor: %d\n\r", analogRead(POT_LEFT), motorPower);
		motorSet(LB, -motorPower);
		motorSet(LF, -motorPower);
		motorSet(LT, motorPower);
		motorSet(RB, motorPower);
		motorSet(RF, motorPower);
		motorSet(RT, -motorPower);

		int pidd = pid_arm(analogRead(POT_LEFT), armValue);
		motorPower = pidd;
		if (motorPower < -127)
		{
			motorPower = -127;
		}
		if (motorPower > 127)
		{
			motorPower = 127;
		}
		delay(20);
	}
}

void releaseCubes(){
	digitalWrite(CUBE_IN, HIGH);
	delay(1000);
	digitalWrite(CUBE_IN, LOW);
}

void goToLine(int dir){
	while (analogRead(LINE_LEFT) >= 1000)
	{
		if (joystickGetDigital(1, 8, JOY_LEFT))
		{
			break;
		}
		motorSet(FL,dir == FORWARD ? 127 : -127);
		motorSet(BL,dir == FORWARD ? 127 : -127);
		motorSet(FR,dir  == FORWARD ? -127 : 127);
		motorSet(BR,dir == FORWARD ? -127 : 127);
	}
	motorStop(FL);
	motorStop(BL);
	motorStop(FR);
	motorStop(BR);
}

//TODO: LCD, IME, Movement,

/*IME*/
	int eLeft,eRight;

	void imeReinitialize(){
		imeShutdown();
		imeReset(eLeft);
		imeReset(eRight);
		imeInitializeAll();
	}

	int imeRead(int ime){
		if(ime==IME_LEFT){
			imeGet(IME_LEFT,&eLeft);
			return eLeft;
		}else if(ime==IME_RIGHT){
			imeGet(IME_RIGHT,&eRight);
			eRight=-eRight;
			return eRight;
		}
		return 0;
	}

/*TASK MANAGEMENT*/
	TaskHandle createAsyncTask(void code()){
		TaskHandle t = taskCreate(code, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT);
		return t;
	}

	void pauseTaskUntil(TaskHandle t, bool b){
		void f(){
			taskSuspend(t);
			while(!b);
			taskResume(t);
		};
		createAsyncTask(f);
	}

	void killTask(TaskHandle task){
		taskSuspend(task);
		taskDelete(task);
	}

/*MOVEMENT*/
	void setMotor(unsigned char channel, int speed){
		int oldSpeed = motorGet(channel); //get current speed
		motorSet(channel,(oldSpeed+((speed-oldSpeed)/MAX_SPEED_DIF)-((speed-oldSpeed)%MAX_SPEED_DIF)));
	}

	//DRIVE
	void smove(int dir, int t, int s){
		if(dir==FORWARD){
			motorSet(FL,s);
//			motorSet(ML,-s);
			motorSet(BL,s);
			motorSet(FR,-s);
//			motorSet(MR,s);
			motorSet(BR,-s);
		}else if(dir==BACKWARD){
			motorSet(FL,-s);
//			motorSet(ML,s);
			motorSet(BL,-s);
			motorSet(FR,s);
//			motorSet(MR,-s);
			motorSet(BR,s);
		}else if(dir==LEFT){
			motorSet(FL,s);
//			motorSet(ML,0);
			motorSet(BL,-s);
			motorSet(FR,s);
//			motorSet(MR,0);
			motorSet(BR,-s);
		}else if(dir==RIGHT){
			motorSet(FL,-s);
//			motorSet(ML,0);
			motorSet(BL,s);
			motorSet(FR,-s);
//			motorSet(MR,0);
			motorSet(BR,s);
		}else if(dir==LEFT_TURN){
			motorSet(FL,s);
//			motorSet(ML,-s);
			motorSet(BL,s);
			motorSet(FR,-s);
//			motorSet(MR,s);
			motorSet(BR,-s);
		}else if(dir==RIGHT_TURN){
			motorSet(FL,s);
//			motorSet(ML,-s);
			motorSet(BL,s);
			motorSet(FR,-s);
//			motorSet(MR,s);
			motorSet(BR,-s);
		}else if(dir==STOP) {
			stop();
			return;
		}
		delay(t);
		stop();
	}

	void move(int dir, int t){
			smove(dir,t,127);
		}

	void stop(){
		motorSet(FL,0);
//		motorSet(ML,0);
		motorSet(BL,0);
		motorSet(FR,0);
//		motorSet(MR,0);
		motorSet(BR,0);
	}

	//LIFT
	void slift(int dir, int t, int speed){
		motorSet(LB,speed*dir);
		motorSet(LF,speed*dir);
		motorSet(RB,-speed*dir);
		motorSet(RF,-speed*dir);
		motorSet(LT,speed*dir);
		motorSet(RT,-speed*dir);


		delay(t);
		stopLift();
	}


	void lift(int dir, int t){
			slift(dir,t,127);
		}

	void stopLift(){
		motorSet(LB,0);
		motorSet(LF,0);
		motorSet(RB,0);
		motorSet(RF,0);
		motorSet(LT,0);
		motorSet(RT,0);
	}

/*File IO*/
	void fsend(char fileName[], char content[]){
		FILE* f = fopen(fileName,"w");
		fprint(content,f);
		fclose(f);
		printf("written");
	}

	char fgetChar(char fileName[], int pos){
		FILE* f = fopen(fileName,"r");
		char c = '?';
		for(int i=0; i<=pos;i++)
			c = fgetc(f);
		fclose(f);
		printf("written");
		return c;
	}

/*TeleOp*/
	int RA_X,RA_Y,LA_X,LA_Y;

	void driveMap(int stick){
			RA_X = joystickGetAnalog(stick, 1);
			RA_Y = joystickGetAnalog(stick, 2);
			LA_Y = joystickGetAnalog(stick, 3);
			LA_X = joystickGetAnalog(stick, 4);

			if(joystickGetDigital(stick,5,JOY_UP)){
				RA_X/=2;
				RA_Y/=2;
				LA_X/=2;
				LA_Y/=2;
			}else{
				if (abs(LA_Y) < ANALOG_THRESH) {
					LA_Y = 0;
				}
				if ((abs(LA_X) < ANALOG_THRESH*2.5)&&(abs(LA_Y) > 95)) {
					LA_X = 0;
				}
				if (abs(RA_Y) < ANALOG_THRESH*3) {
					RA_Y = 0;
				}
				if (abs(RA_X) < ANALOG_THRESH*0.75) {
					RA_X = 0;
				}

				if (abs(LA_Y) > abs(RA_Y)) {
					RA_Y = 0;
				} else {
					LA_Y = 0;
				}
			}
			motorSet(BL, (LA_Y + RA_Y + RA_X - LA_X));
			motorSet(FL, (LA_Y + RA_Y + LA_X + RA_X));
			motorSet(BR, -(LA_Y + RA_Y - RA_X + LA_X));
			motorSet(FR, -(LA_Y + RA_Y - LA_X - RA_X));
		}

	void liftMap(int stick){
		if(joystickGetDigital(stick,8,JOY_LEFT)&&joystickGetDigital(stick,8,JOY_RIGHT)){
			sust= (sust)?0:1;
		}else if(joystickGetDigital(stick,8,JOY_LEFT)){
			sust= 0;
		}else if(joystickGetDigital(stick,8,JOY_RIGHT)){
			sust=1;
		}else if(joystickGetDigital(stick,7,JOY_UP)){
			motorSet(LB,-50);
			motorSet(LF,-50);
			motorSet(LT,50);
		}else if(joystickGetDigital(stick,7,JOY_DOWN)){
			motorSet(LB,50);
			motorSet(LF,50);
			motorSet(LT,-50);
		}else if(joystickGetDigital(stick,8,JOY_UP)){
			motorSet(RB,50);
			motorSet(RF,50);
			motorSet(RT,-50);
		}else if(joystickGetDigital(stick,8,JOY_DOWN)){
			motorSet(RB,-50);
			motorSet(RF,-50);
			motorSet(RT,50);
		}else if(joystickGetDigital(stick,5,JOY_DOWN)){
			motorSet(LB,-joystickGetAnalog(stick,3));
			motorSet(RB,joystickGetAnalog(stick,2));
			motorSet(LF,-joystickGetAnalog(stick,3));
			motorSet(RF,joystickGetAnalog(stick,2));
			motorSet(LT,joystickGetAnalog(stick,2));
			motorSet(RT,-joystickGetAnalog(stick,3));
		}else{
			if(abs(joystickGetAnalog(stick,2))>=(ANALOG_THRESH*.75)){
				motorSet(LB,-joystickGetAnalog(stick,2));
				motorSet(RB,joystickGetAnalog(stick,2));
				motorSet(LF,-joystickGetAnalog(stick,2));
				motorSet(RF,joystickGetAnalog(stick,2));
				motorSet(LT,joystickGetAnalog(stick,2));
				motorSet(RT,-joystickGetAnalog(stick,2));
			}else if(sust){
				motorSet(LB,-25);
				motorSet(RB,25);
				motorSet(LF,-25);
				motorSet(RF,25);
				motorSet(LT,25);
				motorSet(RT,-25);
			}else{
				motorSet(LB,0);
				motorSet(RB,0);
				motorSet(LF,0);
				motorSet(RF,0);
				motorSet(LT,0);
				motorSet(RT,0);
			}
		}

		if(joystickGetDigital(stick,5,JOY_UP)){
			int frontReading = 0;

			if(ultrasonicGet(front)!=0){
				frontReading = ultrasonicGet(front);
				printf("%d\n\r",frontReading);
			}
			if(frontReading<=70&&frontReading>=68&&frontReading!=0){
				speakerPlayRtttl("beep:d=4,o=5,b=200:16f");
			}
			else if(frontReading<=5&&frontReading!=0){
				speakerPlayRtttl("beep:d=4,o=5,b=200:16a#");
			}
		}

		if(joystickGetAnalog(stick,4)>100){
			digitalWrite(SKYRISE_IN,HIGH);
		}else if(joystickGetAnalog(stick,4)<-100){
			digitalWrite(SKYRISE_IN,LOW);
		}

		if(joystickGetAnalog(stick,3)>100){
			digitalWrite(CUBE_IN,HIGH);
		}else if(joystickGetAnalog(stick,3)<-100){
			digitalWrite(CUBE_IN,LOW);
		}
	}
