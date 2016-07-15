/*
 * timers.c
 *
 *  Created on: Jul 8, 2014
 *      Author: Matthew
 */

#include "main.h"

#define NUM_TIMERS 10
long milliTimers[NUM_TIMERS];
long microTimers[NUM_TIMERS];

void resetTimer(int num, bool useMilli) {
	if (useMilli) {
		milliTimers[num] = 0;
	} else {
		microTimers[num] = 0;
	}
}

void resetAllTimers(bool useMilli) {
	for (int i = 0; i < NUM_TIMERS; i++) {
		resetTimer(i, useMilli);
	}
}

void startTimer(int num, bool useMilli) {
	if (useMilli) {
		milliTimers[num] = millis();
	} else {
		microTimers[num] = micros();
	}
}

long getTimer(int num, bool useMilli) {
	if (useMilli) {
		return millis() - milliTimers[num];
	} else {
		return micros() - microTimers[num];
	}
}

long lapTimer(int num, bool useMilli) {
	long returnVal = getTimer(num, useMilli);
	resetTimer(num, useMilli);
	return returnVal;
}
