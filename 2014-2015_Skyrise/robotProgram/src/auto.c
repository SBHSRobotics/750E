#include "main.h"
#include "RobotLib.h"

void strafe(int dir, int t);

void autonomous() {
/*//	strafe(LEFT,200);
//	strafe(RIGHT,200);
//	strafe(FORWARD,800);
//	strafe(BACKWARD,500);
//	motorStopAll();
//	lift(UP);
//	delay(1250);
//	while(analogRead(1)<210)
//		lift(UP);
//	lift(STOP);
//	strafe(BACKWARD,700);
//	lift(UP);
//	delay(500);
//	lift(DOWN);
//	strafe(FORWARD,320);
//	motorStopAll();
//	strafe(RIGHT_TURN,300);
//	motorStopAll();
//	lift(DOWN);
//	delay(1200);
//	lift(STOP);
//	strafe(FORWARD,500);
//	motorStopAll();
	lift(UP);
	delay(3000);
	lift(DOWN);
	delay(3000);
	lift(STOP);
//	strafe(FORWARD,12000);
//	strafe(BACKWARD,1200);
	strafe(FORWARD,1000);*/

	/*strafe(LEFT, 240);
	smove(FORWARD, -116);
	moveArm(3743);
	smove(BACKWARD, 180);
	moveArm(3500);
	smove(FORWARD, -269);
	turn(-65);
	smove(BACKWARD, 134);
	moveArm(3970);
	smove(FORWARD, -217);
	moveArm(4095);
	strafe(LEFT, 160);
	turn(44);
	strafe(LEFT, 160);
	smove(FORWARD, -195);
	moveArm(3690);
	strafe(RIGHT, 440);
	moveArm(3494);
	turn(-150);
	smove(FORWARD, -96);
	moveArm(3908);
	smove(BACKWARD, 107);
	moveArm(4095);
	turn(-350);
	moveArm(3743);
	smove(BACKWARD, 679);
	moveArm(3429);
	smove(FORWARD, -435);*/
	lift(UP);
	delay(3000);
	lift(DOWN);
	delay(3000);
	// moveArm();
	stopLift();
	strafe(FORWARD, 1000);
}

void strafe(int dir, int t) {
	if (dir == FORWARD) {
		motorSet(FL, 127);
		motorSet(ML, -127);
		motorSet(BL, 127);
		motorSet(FR, -127);
		motorSet(MR, 127);
		motorSet(BR, -127);
	} else if (dir == BACKWARD) {
		motorSet(FL, -127);
		motorSet(ML, 127);
		motorSet(BL, -127);
		motorSet(FR, 127);
		motorSet(MR, -127);
		motorSet(BR, 127);
	} else if (dir == RIGHT) {
		motorSet(FL, 127);
		motorSet(ML, 0);
		motorSet(BL, -127);
		motorSet(FR, 127);
		motorSet(MR, 0);
		motorSet(BR, -127);
	} else if (dir == LEFT) {
		motorSet(FL, -127);
		motorSet(ML, 0);
		motorSet(BL, -127);
		motorSet(FR, -127);
		motorSet(MR, 0);
		motorSet(BR, -127);
	} else if (dir == LEFT_TURN) {
		motorSet(FL, 127);
		motorSet(ML, -127);
		motorSet(BL, 127);
		motorSet(FR, -127);
		motorSet(MR, 127);
		motorSet(BR, -127);
	} else if (dir == RIGHT_TURN) {
		motorSet(FL, 127);
		motorSet(ML, -127);
		motorSet(BL, 127);
		motorSet(FR, 127);
		motorSet(MR, -127);
		motorSet(BR, 127);
	}
	delay(t);
	stop();
}

