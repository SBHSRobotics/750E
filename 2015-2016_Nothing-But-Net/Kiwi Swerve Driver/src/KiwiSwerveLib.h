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
#define LR 2
#define LW 3
#define MR 4
#define MW 5
#define RR 6
#define RW 7
  //Conveyor
#define CY 8
#define C 1
  //Shooter
#define SY 9
#define S 10

#define LP 1
#define MP 2
#define RP 3

static const int PID_MOTOR_SCALE = 112;
static const int PID_THRESH = 50;

//typedef struct DriverConfiguration DriverConfiguration;

typedef struct DriverConfiguration {
	int leftWheel;
	int middleWheel;
	int rightWheel;
}DriverConfiguration;

extern DriverConfiguration kiwiConfig;
extern DriverConfiguration lock;

void activateDriveConfig(DriverConfiguration config);

void driveInit();
void driveKill();

#endif /* KIWISWERVELIB_H_ */
