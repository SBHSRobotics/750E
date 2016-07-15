#include "main.h"
#include "RobotLib.h"

int autonPath = 0;
int flippy = 1;

static const short ABOVE_SKYRISE_HEIGHT = 1900;
static const short START_HEIGHT = 1600;

TaskHandle armTaskHandle;

void getToScoringPosition(void) {
	smove(LEFT, 300, 110);
	smove(RIGHT, 300, 110);
	gyroTo(-44);
	armTo(2200);
	armTo(ABOVE_SKYRISE_HEIGHT);
	encoderTo(FORWARD, 150);
	gyroTo(0);
	digitalWrite(SKYRISE_IN, HIGH);
	armTo(START_HEIGHT);
}

short getHeightAtLevel(unsigned char level) {
	short levelOffset = 0;
	switch (level) {
	case 0:
		levelOffset = 500;
		break;
	case 1:
		levelOffset = 700;
		break;
	case 2:
		levelOffset = 1000;
		break;
	case 3:
		levelOffset = 0;
		break;
	case 4:
		levelOffset = 0;
		break;
	case 5:
		levelOffset = 0;
		break;
	case 6:
		levelOffset = 0;
		break;
	case 7:
		levelOffset = 0;
		break;
	}

	return START_HEIGHT + levelOffset;
}

void scoreSkyriseAtLevel(unsigned char level) {
	digitalWrite(SKYRISE_IN, LOW);
	delay(100);
//	armTo(getHeightAtLevel(level));
	armValue = getHeightAtLevel(level);
	armTaskHandle = taskCreate(armTask, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT);
	delay(2000);
	gyroTo(-64 * flippy);
	if (level == 0) {
		taskDelete(armTaskHandle);
		armTo(START_HEIGHT);
	}
	digitalWrite(SKYRISE_IN, HIGH);
	delay(100);
	armValue = ABOVE_SKYRISE_HEIGHT;
	gyroTo(0 * flippy); // if we decide to move it so that it's not 0,
						// i just saved you typing (or forgetting) 8 characters
	taskDelete(armTaskHandle);
	armTo(START_HEIGHT);
}

void skyriseAuton() {
	static const unsigned char NUM_SKYRISES = 6;

	gyroReset(gyro);
	 getToScoringPosition();
	for (int i = 0; i < NUM_SKYRISES; i++) {
		scoreSkyriseAtLevel(i);
		delay(20);
	}
}

void newCubeAuton()
{
	digitalWrite(CUBE_IN, HIGH);
	armTo(2200);
	encoderTo(FORWARD, 0.5);
	armTo(1600);
	// digitalWrite(CUBE_IN, HIGH);
	armTo(2200);
	encoderTo(RIGHT, 0.5);
	encoderTo(FORWARD, 0.7);
	gyroTo(90);
	encoderTo(FORWARD, 0.5);
	armTo(3240);
	smove(FORWARD, 1000, 127);
	digitalWrite(CUBE_IN, LOW);
	delay(1000);
	digitalWrite(CUBE_IN, HIGH);
}

/*void cubeAuton()
{
	//	static const int FULL_DOWN = 1000;
	//	static const int JUST_ABOVE_CUBE = 1000;
	//	static const int SMALL_POLE = 1000;
	//	static const int MEDIUM_POLE = 1000;
	//
	//	static const unsigned char FRONT = 0;
	//
	//	encoderTo(FORWARD, 1000);
	//	armTo(JUST_ABOVE_CUBE);	// above cube level
	//	ultrasonicTo(FRONT, 8, 0); // backup: encoderTo(FORWARD, 1.2);
	//	armTo(FULL_DOWN); 		// base level
	//	digitalWrite(CUBE_IN, HIGH);
	//	armTo(JUST_ABOVE_CUBE); // above cube level
	//	goToLine(FORWARD);
	//	encoderTo(LEFT, 2 * flippy);
	//	encoderTo(LEFT_TURN, 180 * flippy);//180 degree turn
	//	ultrasonicTo(FRONT, 8, 0);	// backup: encoderTo(FORWARD, 0.8);
	//	armTo(FULL_DOWN); 		// base level
	//	// digitalWrite(CUBE_IN, HIGH);
	//	armTo(MEDIUM_POLE); // above cube level - actually now it's post level lelz
	//	encoderTo(RIGHT, 2 * flippy);
	//	// armTo(MEDIUM_POLE);//post level
	//	ultrasonicTo(FORWARD, 5, 0);//just enough to push against post
	//	// backup: encoderTo(FORWARD, 0.4);
	//	releaseCubes();//make this
	//	encoderTo(BACKWARD, -0.7);
	//	encoderTo(LEFT_TURN, 180 * flippy);
	//	ultrasonicTo(FORWARD, 8, 0);	// backup: encoderTo(FORWARD, 0.7);
	//	armTo(FULL_DOWN); 		// base level
	//	digitalWrite(CUBE_IN, HIGH);
	//	armTo(JUST_ABOVE_CUBE); // above cube level
	//	encoderTo(LEFT, 3 * flippy);
	//	encoderTo(LEFT_TURN, 180 * flippy);
	//	// armTo(3000);
	//	ultrasonicTo(FORWARD, 8, 0);	// backup: encoderTo(FORWARD, 0.8);
	//	armTo(FULL_DOWN); 		// base level
	//	// digitalWrite(CUBE_IN, HIGH);
	//	armTo(SMALL_POLE); // above cube level
	//	ultrasonicTo(FORWARD, 5, 0);	// backup: encoderTo(FORWARD, 0.4);
	//	releaseCubes();

	armTo(2200);
	encoderTo(FORWARD, 0.5);
	armTo(1600);
	armTo(2200);
	encoderTo(RIGHT, 0.5 * flippy);
	encoderTo(FORWARD, 0.8);
	gyroTo(90 * flippy);
	encoderTo(FORWARD, 0.5);
	armTo(3240);
	smove(FORWARD, 1000, 127);
	releaseCubes();
}*/

void autonomous() {
	taskDelete(autonSelectorTask);
	// newCubeAuton();

	switch (autonPath) {
	case RED_CUBE_1:
		flippy = -1;
		newCubeAuton();
		break;
	case RED_CUBE_2:
		flippy = -1;
		newCubeAuton();
		break;
	case RED_CUBE_3:
		flippy = -1;
		newCubeAuton();
		break;
	case RED_SKYRISE:
		flippy = -1;
		skyriseAuton();
		break;
	case BLUE_CUBE_1:
		flippy = 1;
		newCubeAuton();
		break;
	case BLUE_CUBE_2:
		flippy = 1;
		newCubeAuton();
		break;
	case BLUE_CUBE_3:
		flippy = 1;
		newCubeAuton();
		break;
	case BLUE_SKYRISE:
		flippy = 1;
		skyriseAuton();
		break;
	}
}

