/*
 * timers.h
 *
 *  Created on: Oct 8, 2014
 *      Author: Matthew
 */

#ifndef TIMERS_H_
#define TIMERS_H_

#include "main.h"

void initTimers();
void stopTimer(int num);
void startTimer(int num, bool useMilli);
long getTimer(int num, bool useMilli);
long lapTimer(int num, bool useMilli);
void resumeTimer(int num, bool useMilli);
void pauseTimer(int num, bool useMilli);

#endif /* TIMERS_H_ */
