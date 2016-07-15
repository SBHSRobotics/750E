#include "main.h"
#include <math.h>

/*TODO:
 *~Turning
 *~Manipulator
*/

/*ACCEL RAMP CONSTANT*/
#define MAX_SPEED_DIFFERENCE 7


/*TASK MANAGEMENT*/
TaskHandle createAsyncTask(void code()){
	TaskHandle t = taskCreate(code, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT);
	return t;
}
//
void pauseTaskUntil(TaskHandle task, bool pauseCondition){
	void f(){
		taskSuspend(task);
		while(!pauseCondition);
		taskResume(task);
	};
	createAsyncTask(f);
}
//
void killTask(TaskHandle task){
	taskSuspend(task);
	taskDelete(task);
}
//


/*MOVEMENT*/
void setMotor(unsigned char channel, int speed){
	if(speed<=MOTOR_THRESH)
		speed=0;
	int oldSpeed = motorGet(channel); //get current speed
	int newSpeed = speed; //get change speed
	if(abs(newSpeed-oldSpeed)>MAX_SPEED_DIFFERENCE){
		newSpeed=(newSpeed>oldSpeed)? (oldSpeed+MAX_SPEED_DIFFERENCE):(oldSpeed-MAX_SPEED_DIFFERENCE);
		motorSet(channel,newSpeed);
		delay(10);//delay can not be too large or controls could lag. Reducing threshhold is better than increasing delay.
		setMotor(channel,speed);
		// if speed change is greater than threshhold, change speed by threshhold and recurse on delay
	}else{
		motorSet(channel,speed);
		//otherwise just set speed
	}
}
//Convert encoder reading to distance in inches
double encoderReadDistance(Encoder e){
	return (encoderGet(e)*3.141592653585)/90;
}
//
Displacement getDisplacement(Encoder eLeft, Encoder eRight){
	double ldist = encoderReadDistance(eLeft);
	double rdist = encoderReadDistance(eRight);
	Displacement d;
	d.x = ((ldist/(sqrt(2)))-(rdist/(sqrt(2))));
	d.y = ((ldist/sqrt(2))+(rdist/sqrt(2)));
	return d;
}
//
void move(int angle, int distance, Encoder eLeft, Encoder eRight){
	Displacement initial = getDisplacement(eLeft,eRight);
	void f(){
		double ldist = 0;
		double rdist = 0;
		Displacement d;
		d.y = (distance*sin(angle));//Get vertical displacement
		d.x = (distance*cos(angle));//Get horizontal displacement
		d.y/=sqrt(2);//Change to wheel displacement component (wheel is at 45 degree angle)
		d.x/=sqrt(2);//^

		if(angle<=90){//Forward-Right (None Inverted)
			ldist = ( d.y + d.x);
			rdist = ( d.y - d.x);//(+/-): Right Wheel : inverted (opp. horizontal displacement)
		}else if(angle<=180){//Forward-Left (X Inverted)
			angle-=90;//Get angle from axis
			ldist = ( d.y + (-d.x) );
			rdist = ( d.y - (-d.x) );
		}else if(angle<=270){//Backward-Left (X&Y Inverted)
			angle-=180;
			ldist = ( (-d.y) + (-d.x));//
			rdist = ( (-d.y) - (-d.x));
		}else if(angle<=360){//BackWard-Right (Y Inverted)
			angle-=270;
			ldist = ( (-d.y)  + d.x);
			rdist = ( (-d.y)  - d.x);
		}
		int rspeed = 127,lspeed = 127;//Set default speed
		if(rdist>ldist){
			rspeed=127;
			lspeed=127*(ldist/rdist);//Reduce LW proportional to distance difference
		}else if(ldist>rdist){
			lspeed=127;
			rspeed=127*(rdist/ldist);//Reduce RW proportional to distance difference
		}
		double vspeed = (sqrt(2)*lspeed) + (sqrt(2)*rspeed);
		while( ((getDisplacement(eLeft,eRight).y-initial.y)<d.y)
				|| ((getDisplacement(eLeft,eRight).x-initial.x)<d.x) ){
			setMotor(frontLeft,lspeed);
			setMotor(backRight,lspeed);
			setMotor(frontRight,rspeed);
			setMotor(backLeft,rspeed);
			setMotor(middleLeft,vspeed);
			setMotor(middleRight,vspeed);
		}
	}
	createAsyncTask(f);
}
//


/*File IO*/
void fsend(char fileName[], char content[]){
	FILE* f = fopen(fileName,"w");
	fprint(content,f);
	fclose(f);
	printf("written");
}
//
char fgetChar(char fileName[], int pos){
	FILE* f = fopen(fileName,"r");
	char c = '?';
	for(int i=0; i<=pos;i++)
		c = fgetc(f);
	fclose(f);
	printf("written");
	return c;
}
//
