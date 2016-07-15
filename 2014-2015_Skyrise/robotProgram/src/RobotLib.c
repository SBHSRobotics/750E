#include "main.h"
#include "RobotLib.h"
#include <math.h>

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
		motorSet(ML,-s);
		motorSet(BL,-s);
		motorSet(FR,-s);
		motorSet(MR,s);
		motorSet(BR,s);
	}else if(dir==BACKWARD){
		motorSet(FL,-s);
		motorSet(ML,s);
		motorSet(BL,s);
		motorSet(FR,s);
		motorSet(MR,-s);
		motorSet(BR,-s);
	}else if(dir==LEFT){
		motorSet(FL,s);
		motorSet(ML,0);
		motorSet(BL,s);
		motorSet(FR,s);
		motorSet(MR,0);
		motorSet(BR,s);
	}else if(dir==RIGHT){
		motorSet(FL,-s);
		motorSet(ML,0);
		motorSet(BL,-s);
		motorSet(FR,-s);
		motorSet(MR,0);
		motorSet(BR,-s);
	}else if(dir==LEFT_TURN){
		motorSet(FL,s);
		motorSet(ML,-s);
		motorSet(BL,-s);
		motorSet(FR,-s);
		motorSet(MR,s);
		motorSet(BR,s);
	}else if(dir==RIGHT_TURN){
		motorSet(FL,s);
		motorSet(ML,-s);
		motorSet(BL,-s);
		motorSet(FR,-s);
		motorSet(MR,s);
		motorSet(BR,s);
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
	smove(0,0,0);
}

//LIFT
void slift(int dir,int speed){
	motorSet(LB,speed*dir);
	motorSet(LF,speed*dir);
	motorSet(RB,-speed*dir);
	motorSet(RF,-speed*dir);
}
void lift(int dir){
	slift(dir,127);
}

void stopLift(){
	slift(0,0);
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

	if(joystickGetDigital(1,5,JOY_UP)){
		RA_X/=2;
		RA_Y/=2;
		LA_X/=2;
		LA_Y/=2;
	}else{
		if (abs(LA_Y) < ANALOG_THRESH) {
			LA_Y = 0;
		}
		if (abs(LA_X) < ANALOG_THRESH*1.5) {
			LA_X = 0;
		}
		if (abs(RA_Y) < ANALOG_THRESH) {
			RA_Y = 0;
		}
		if (abs(RA_X) < ANALOG_THRESH) {
			RA_X = 0;
		}

		if (abs(LA_Y) > abs(RA_Y)) {
			RA_Y = 0;
		} else {
			LA_Y = 0;
		}
	}
	motorSet(BL, (LA_Y + RA_Y + RA_X - LA_X));
	motorSet(ML, -(LA_Y + RA_Y + RA_X));
	motorSet(FL, (LA_Y + RA_Y + LA_X + RA_X));
	motorSet(BR, -(LA_Y + RA_Y - RA_X + LA_X));
	motorSet(MR, (LA_Y + RA_Y - RA_X));
	motorSet(FR, -(LA_Y + RA_Y - LA_X - RA_X));
}

void liftMap(int stick){
	if(joystickGetDigital(stick,7,JOY_LEFT)){

	}else if(joystickGetDigital(stick,7,JOY_UP)){
		motorSet(LB,50);
		motorSet(LF,50);
	}else if(joystickGetDigital(stick,7,JOY_DOWN)){
		motorSet(LB,-50);
		motorSet(LF,-50);
	}else if(joystickGetDigital(stick,8,JOY_UP)){
		motorSet(RB,-50);
		motorSet(RF,-50);
	}else if(joystickGetDigital(stick,8,JOY_DOWN)){
		motorSet(RB,50);
		motorSet(RF,50);
	}else if(joystickGetDigital(stick,5,JOY_DOWN)){
		motorSet(LB,joystickGetAnalog(stick,3));
		motorSet(RB,-joystickGetAnalog(stick,2));
		motorSet(LF,joystickGetAnalog(stick,3));
		motorSet(RF,-joystickGetAnalog(stick,2));
	}else{
		if (abs(joystickGetAnalog(stick, 2)) > 20) {
			motorSet(LB,joystickGetAnalog(stick,2));
			motorSet(RB,-joystickGetAnalog(stick,2));
			motorSet(LF,joystickGetAnalog(stick,2));
			motorSet(RF,-joystickGetAnalog(stick,2));
		} else {
			motorStop(LB);
			motorStop(RB);
			motorStop(LF);
			motorStop(RF);
		}
	}
}
