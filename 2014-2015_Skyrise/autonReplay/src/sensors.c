/*
 * sensors.c
 *
 *  Created on: Sep 19, 2014
 *      Author: Matthew
 */

#include "main.h"

void imeReinitialize(){
	print("Starting IMEs....\n\r");
	imeShutdown();
	imeReset(0);
	imeReset(1);
	imeInitializeAll();
	print("Done!\n\r");
}

Sensor createSensor(SensorType type, int port) {
	static bool initIME = false;
	Sensor retSen = {.type = type, .port = port};
	switch (type) {
	case SHAFT_ENCODER:
		retSen.enc = encoderInit(port, port + 1, false);
		if (DEBUG) {
			print("encoder init");
			if (retSen.enc == NULL) {
				print(" FAILED");
			}
			getchar();
		}
		break;
	case GYROSCOPE:
		retSen.gyr = gyroInit(port, 0);
		break;
	case ULTRASONIC:
		retSen.ult = ultrasonicInit(port, port + 1);
		break;
	case IME:
		if (!initIME) {
			printDebug("Init IME!");
			// imeReinitialize();
			initIME = true;
			printDebug("Done");
		}
		break;
	default:
		break;
	}

	return retSen;
}

void shutdownSensor(Sensor *s) {
	switch (s->type) {
	case SHAFT_ENCODER:
		encoderShutdown(s->enc);
		// free(s->enc);
		// print("encoder shutdown");
		break;
	case GYROSCOPE:
		gyroShutdown(s->gyr);
		break;
	case ULTRASONIC:
		ultrasonicShutdown(s->ult);
		break;
	default:
		break;
	}
}

void startSensor(Sensor *s) {
	switch (s->type) {
	case SHAFT_ENCODER:
		encoderReset(s->enc);
		if (DEBUG) {
			print("reset encoder");
		}
		break;
	case GYROSCOPE:
		gyroReset(s->gyr);
		break;
	case TIME:
		//resetTimer(s->port, true);
		startTimer(s->port, true);
		break;
	case IME:
		imeReset(s->port);
		break;
	default:
		break;
	}
}

short getSensorValue(Sensor s) {
	short retVal = -1;
	switch (s.type) {
	case SHAFT_ENCODER:
		if (DEBUG) {
			print("got encoder value");
		}
		retVal = (short)encoderGet(s.enc);
		break;
	case GYROSCOPE:
		retVal = (short)gyroGet(s.gyr);
		break;
	case ULTRASONIC:
		retVal = (short)ultrasonicGet(s.ult);
		break;
	case ACCELEROMETER:
		retVal = (short)analogReadCalibratedHR(s.port);
		break;
	case BUMPER_SWITCH:
	case LIMIT_SWITCH:
		retVal = (short)digitalRead(s.port);
		break;
	case LINE_TRACKER:
	case POTENTIOMETER:
		retVal = (short)analogRead(s.port);
		break;
	case TIME:
		retVal = (short)getTimer(s.port, true);
		break;
	case IME: ; // so apparently due to history this ";" has to be here to declare
	// a variable inside a case statement?
	int imeVal = -1;
	imeGet(s.port, &imeVal);
	retVal = (short)imeVal;
	break;
	}
	return retVal;
}
