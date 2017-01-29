/*
 * charlesLib.h
 *
 *  Created on: Nov 14, 2015
 *      Author: Mikel
 */

#ifndef CHARLESLIB_H_
#define CHARLESLIB_H_

//Wheel Ports
#define LF 1
#define RF 2
#define LB 9
#define RB 10

//Intake
#define INTAKE 3

//Conveyor
#define CONVEYOR 8

//Shooter

#define SHOOTER_TL 4
#define SHOOTER_TR 5
#define SHOOTER_BL 6
#define SHOOTER_BR 7

//Data
#define LCD uart1

//Sensor
#define DIST_A 1
#define DIST_B 2
#define ENC_L_A 11
#define ENC_L_B 12
#define ENC_R_A 3
#define ENC_R_B 4

#define GYRO 2

#define SELECTOR 5


//Drive Modes
#define STANDARD_DRIVE 1
#define RC_CAR 2
#define RC_CAR_P 3

#define NUM_DRIVES 3

Encoder eLeft, eRight;


#endif /* CHARLESLIB_H_ */
