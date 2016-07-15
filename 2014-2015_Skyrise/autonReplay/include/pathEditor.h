/*
 * pathEditor.h
 *
 *  Created on: Dec 12, 2014
 *      Author: Matthew
 */

#ifndef PATHEDITOR_H_
#define PATHEDITOR_H_

extern bool redrawDisplay;
extern bool movedCursor;
extern Mutex doNotDrive;

void runEditor(void *ignore);

void gotoxy(int x, int y);

#endif /* PATHEDITOR_H_ */
