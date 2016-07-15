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
long milliStoreTime[NUM_TIMERS];
long microStoreTime[NUM_TIMERS];
bool stopped[NUM_TIMERS];

// ------------------ TIMERS --------------------
void initTimers() {
	for (int i = 0; i < NUM_TIMERS; i++) {
		stopped[i] = true;
		milliStoreTime[i] = -1;
		microStoreTime[i] = -1;
	}
}

void startTimer(int num, bool useMilli) {
	if (useMilli) {
		milliTimers[num] = millis();
	} else {
		microTimers[num] = micros();
	}

	if (stopped[num]) {
		stopped[num] = false;
	}
}

long getTimer(int num, bool useMilli) {
	if (stopped[num]) {
		return 0;
	}

	if (useMilli) {
		if (milliStoreTime[num] == -1) {
			return millis() - milliTimers[num];
		} else {
			return milliStoreTime[num];
		}
	} else {
		if (microStoreTime[num] == -1) {
			return micros() - microTimers[num];
		} else {
			return microStoreTime[num];
		}
	}
}

long lapTimer(int num, bool useMilli) {
	if (stopped[num]) {
		return 0;
	}

	long returnVal = getTimer(num, useMilli);
	startTimer(num, useMilli);
	return returnVal;
}

void pauseTimer(int num, bool useMilli) {
	if (stopped[num]) {
		return;
	}

	if (useMilli) {
		if (milliStoreTime[num] != -1) {
			return;
		}
	} else {
		if (microStoreTime[num] != -1) {
			return;
		}
	}

	if (useMilli) {
		// printf("DEBUG: Stored time - %ld\n\r", getTimer(num, useMilli));
		milliStoreTime[num] = getTimer(num, useMilli);
	} else {
		microStoreTime[num] = getTimer(num, useMilli);
	}
}

void resumeTimer(int num, bool useMilli) {
	if (stopped[num]) {
		return;
	}

	if (useMilli) {
		if (milliStoreTime[num] == -1) {
			return;
		}
	} else {
		if (microStoreTime[num] == -1) {
			return;
		}
	}

	if (useMilli) {
		startTimer(num, useMilli);
		milliTimers[num] -= milliStoreTime[num];
		milliStoreTime[num] = -1;
		// printf("DEBUG: New time - %ld\n\r", getTimer(num, useMilli));
	} else {
		startTimer(num, useMilli);
		microTimers[num] -= microStoreTime[num];
		microStoreTime[num] = -1;
	}
}

void stopTimer(int num) {
	stopped[num] = true;
}
// ------------------ TIMERS --------------------
