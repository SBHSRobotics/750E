/*
 * autonRunner.h
 *
 *  Created on: Jan 16, 2015
 *      Author: Matthew
 */

#ifndef AUTONRUNNER_H_
#define AUTONRUNNER_H_

extern Mutex doNotRecord;

void runCurrentPath(MoveNode *startNode, short endId);

#endif /* AUTONRUNNER_H_ */
