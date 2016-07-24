/*
 * KiwiSwerveLib.h
 *
 *  Created on: Aug 2, 2015
 *      Author: Mikel
 */

#ifndef KIWISWERVELIB_H_
#define KIWISWERVELIB_H_

//Motors
  //Drive
#define LR 10
#define RR 1
#define LW 0
#define RW 0
#define LF 9
#define LB 8
#define RF 3
#define RB 2

  //Conveyor
#define CY 0
#define C 0
  //Shooter
#define SY 0
#define S 0

#define LP 8
#define MP 0
#define RP 1

#define ID_HOLO 0
#define ID_TANK 1

static const int PID_MOTOR_SCALE = 70;
static const int PID_THRESH = 80;

//typedef struct DriverConfiguration DriverConfiguration;

typedef struct DriverConfiguration {
	unsigned char id;
	int leftWheel;
	int middleWheel;
	int rightWheel;
}DriverConfiguration;

extern DriverConfiguration kiwiConfig;
extern DriverConfiguration lock;
extern DriverConfiguration currentConfig;

void activateDriveConfig(DriverConfiguration config);

void driveInit();
void driveKill();

#endif /* KIWISWERVELIB_H_ */
