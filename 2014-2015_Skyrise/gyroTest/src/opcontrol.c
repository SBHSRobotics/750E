/** @file opcontrol.c
 * @brief File for operator control code
 *
 * This file should contain the user operatorControl() function and any functions related to it.
 *
 * Copyright (c) 2011-2014, Purdue University ACM SIG BOTS.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of Purdue University ACM SIG BOTS nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL PURDUE UNIVERSITY ACM SIG BOTS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Purdue Robotics OS contains FreeRTOS (http://www.freertos.org) whose source code may be
 * obtained from http://sourceforge.net/projects/freertos/files/ or on request.
 */

#include "main.h"

// size_t strlen ( const char * str );

void testArm() {
	while(true) {
		const int power = 127;

		char c = getchar();

		if (c == 'w') {
			motorSet(2, power);
			motorSet(3, power);
		} else if (c == 's') {
			motorSet(2, -power);
			motorSet(3, -power);
		} else {
			motorSet(2, 0);
			motorSet(3, 0);
		}
	}
}

typedef enum SensorType {
	GYROSCOPE,
	LINE_TRACKER,
	ULTRASONIC,
	SHAFT_ENCODER,
	BUMPER_SWITCH,
	LIMIT_SWITCH,
	POTENTIOMETER,
	ACCELEROMETER,
	TIME,
	IME
} SensorType;

typedef struct Sensor {
	union {
		Encoder enc;
		Gyro gyr;
		Ultrasonic ult;
	};
	SensorType type;
	unsigned char port;
} Sensor;

typedef struct SpeedPoint {
	signed char *motPower;
	short *endSensorVal;
} SpeedPoint;

typedef struct MotorSensorPair {
	char *pairStr;
	Sensor *sensors;
	char *motorPorts;
	float **pattern;
	short numPatterns;
	char pairId;
	char numSensors;
	char numPorts;
	bool running;
} MotorSensorPair;

/*typedef struct MoveNode {
	int nodeId;
	MotorSensorPair *pair;
	int prevPairId;
	int *startVal;
	int startPoint;
	int *numRanges;
	int **patternStartVals;
	int **patternRangeLens;
	SpeedPoint *points;
	int numPoints;
	struct MoveNode *child;
	struct MoveNode *sibling;
} MoveNode;*/

typedef struct MoveNode {
	struct MoveNode *child;
	struct MoveNode *sibling;
	SpeedPoint *points;
	short *numRanges;
	short **patternStartVals;
	short **patternRangeLens;
	short *startVal;
	short startPoint;
	short numPoints;
	short nodeId;
	unsigned char pairId;
	unsigned char prevPairId;
} MoveNode;

//Kalman Object Definition
typedef struct{
	unsigned char ch;
	double p;//process noise covariance
	double k;//measurement noise covariance
	double x;//value
	double q;//estimation error covariance
	double r;//kalman gain
}Kalman;

//Kalman Initialize Function
Kalman filterInit(unsigned char channel){
	Kalman k;
	k.ch=channel;
	k.p=2;
	k.k=0;
	k.x=0;
	k.q=0.1;
	k.r=10;
	return k;
}

//Read Function
int accelRead(Kalman k){
	int measurement = analogReadCalibratedHR(k.ch) / 16;
	k.p += k.q;
	k.k = (k.p / (k.p + k.r));
	k.x += k.k * (measurement - k.x);
	k.p *= (1 - k.k);

	return k.x;
}

/*
 * Runs the user operator control code. This function will be started in its own task with the
 * default priority and stack size whenever the robot is enabled via the Field Management System
 * or the VEX Competition Switch in the operator control mode. If the robot is disabled or
 * communications is lost, the operator control task will be stopped by the kernel. Re-enabling
 * the robot will restart the task, not resume it from where it left off.
 *
 * If no VEX Competition Switch or Field Management system is plugged in, the VEX Cortex will
 * run the operator control task. Be warned that this will also occur if the VEX Cortex is
 * tethered directly to a computer via the USB A to A cable without any VEX Joystick attached.
 *
 * Code running in this task can take almost any action, as the VEX Joystick is available and
 * the scheduler is operational. However, proper use of delay() or taskDelayUntil() is highly
 * recommended to give other tasks (including system tasks such as updating LCDs) time to run.
 *
 * This task should never exit; it should end with some kind of infinite loop, even if empty.
 */
void operatorControl() {
	// malloc test
	/*char *blah = malloc(50000 * sizeof(char));
	if (blah != NULL) {
		print("it worked\n\r");
	} else {
		print("nope\n\r");
	}
	free(blah);
	getchar();*/

	// Size tests
	/*char *str = malloc(0);
	if (str != NULL) {
		printf("Size of str: %d\n\r", sizeof(*str));
	} else {
		print("Null\n\r");
	}


	print("Beginning size tests...\n\r");
	printf("Size of MoveNode: %d\n\r", sizeof(MoveNode));
	printf("Size of MotorSensorPair: %d\n\r", sizeof(MotorSensorPair));
	printf("Size of SpeedPoint: %d\n\r", sizeof(SpeedPoint));
	printf("Size of Sensor: %d\n\r", sizeof(Sensor));
	printf("Size of SensorType: %d\n\r", sizeof(SensorType));
	print("Ending size tests.\n\r");
	print("\n\r");

	// Casting tests - passed
	print("Beginning casting tests...\n\r");
	int intNum = -127;
	signed char testChar = (signed char)intNum;
	printf("%d\n\r", testChar);
	intNum = 127;
	testChar = (signed char)intNum;
	printf("%d\n\r", testChar);
	intNum = 0;
	testChar = (signed char)intNum;
	printf("%d\n\r", testChar);
	intNum = 200;
	testChar = (signed char)intNum;
	printf("%d\n\r", testChar);
	print("Ending casting tests.\n\r");*/

	// Encoder enc = encoderInit(1, 2, false);
	/*int values[31];
	for (int i = 0; i < 31; i++) {
		values[i] = 0;
	}
	long count = 0;
	long FINISH = 100000;*/
	/*const int NUM_VALUES = 30;
	int recordedValues[20];
	for (int i = 0; i < NUM_VALUES; i++) {
		recordedValues[i] = -1;
	}*/
	/*analogCalibrate(1);
	print("Finished calibrating.\n\r");
	Kalman filter = filterInit(1);*/

	/*int sum = 0;
	for (int i = 0; i < NUM_VALUES; i++) {
		recordedValues[i] = analogReadCalibratedHR(1) / 16;
		sum += recordedValues[i];
		delay(1);
	}
	printf("%d\n\r", sum / NUM_VALUES);

	int replaceIndex = 0;*/
	Gyro gyro = gyroInit(4, 0);
	gyroReset(gyro);
	while (1) {
		printf("Gyro: %d\n\r", gyroGet(gyro));
		//printf("Pot: %d\n\r", analogRead(1));
		/*if (!imeGet(1, &imeVal)) {
			print("Screwed up\n\r");
		}
		printf("%d\n\r", imeVal);*/

		// printf("%d\n\r", encoderGet(enc));

		/*int val = analogReadCalibratedHR(1) / 16;
		sum += val - recordedValues[replaceIndex];
		printf("%d\n\r", sum / NUM_VALUES);
		recordedValues[replaceIndex] = val;
		replaceIndex = (replaceIndex == NUM_VALUES - 1 ? 0 : replaceIndex + 1);*/

		// printf("%d\n\r", accelRead(filter));
		// printf("%d\n\r", analogReadCalibratedHR(1) / 16);

		/*if (count < FINISH) {
			int accelVal = analogReadCalibratedHR(1) / 16;
			// printf("%d\n\r", accelVal);
			if (abs(accelVal) <= 15) {
				values[accelVal + 15]++;
				count++;
			} else {
				printf("Weird value: %d\n\r", accelVal);
			}
		} else {
			for (int i = 0; i < 31; i++) {
				printf("Value: %d\tNum: %d\n\r", i - 15, values[i]);
				delay(50);
			}
			print("Done.\n\r");
			while (1) { delay(20); }
		}*/
		delay(20);

		/*if (abs(joystickGetAnalog(1, 2)) > 10) {
			motorSet(3, joystickGetAnalog(1, 2));
		} else {
			motorStop(3);
		}*/

		/*char *testStr = malloc(5 * sizeof(char));
		sprintf(testStr, "%d", analogRead(1));
		strcat(testStr, "asdf");
		//memcpy(testStr, "a", strlen(testStr));
		printf("Pot: %s\n\r", testStr);
		delay(50);*/

		/*motorSet(1, 127);
		delay(1000);
		motorSet(1, -127);
		delay(1000);
		motorSet(1, 0);
		delay(5000);*/

		// testing sizeof
		/*Encoder outerEnc = encoderInit(1, 2, false);
		Sensor sen = {.type = SHAFT_ENCODER, .port = 1, .enc = outerEnc};
		int *portList = malloc(2 * sizeof(int));
		portList[0] = 1;
		portList[1] = 2;
		MotorSensorPair pair = {.pairId = 0, .sensor = sen, .motorPorts = portList, .numPorts = 2};
		MotorSensorPair *pairPointer = &pair;
		printf("Simple size of pair: %d\n\r", sizeof(*pairPointer));
		printf("Size of pair struct: %d\n\r", sizeof(MotorSensorPair));
		printf("Size of pair: %d\n\r", sizeof(pair));
		printf("Size of pairId: %d\n\r", sizeof(pair.pairId));
		printf("Size of pair sensor: %d\n\r", sizeof(pair.sensor));
		printf("Size of pair motor ports: %d\n\r", sizeof(pair.motorPorts));
		printf("Size of pair num ports: %d\n\r", sizeof(pair.numPorts));
		printf("Size of port list: %d\n\r", sizeof(*portList));
		printf("Size of sensor struct: %d\n\r", sizeof(Sensor));
		printf("Size of sensor: %d\n\r", sizeof(sen));
		printf("Size of sensor type struct: %d\n\r", sizeof(SensorType));
		printf("Size of sensor type: %d\n\r", sizeof(sen.type));
		printf("Size of sensor port: %d\n\r", sizeof(sen.port));
		printf("Size of sensor enc: %d\n\r", sizeof(sen.enc));
		printf("Size of enc struct: %d\n\r", sizeof(Encoder));
		printf("Size of enc: %d\n\r", sizeof(outerEnc));
		delay(10000000);*/
		// own version of realloc - works?
		/*resize(listOfStructs, numStructs);
		delay(5000);*/

		// so free works
		/*TestStruct *num = malloc(sizeof(TestStruct));
		num->i = 10;
		num->f = 1.0f;
		printf("%d %f\n\r", num->i, num->f);
		free(num);*/

		// realloc doesn't?
	}
}
