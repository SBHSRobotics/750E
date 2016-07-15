/*
 * autonRunner.c
 *
 *  Created on: Jan 13, 2015
 *      Author: Matthew
 */

#include "main.h"

bool outOfMemory;
Mutex doNotRecord;

void setMotorSpeeds(MoveNode *node, int point) {
	for (int patInd = 0; patInd < pairMap[node->nPairId].numPatterns; patInd++) {
		for (int rInd = 0; rInd < node->numRanges[patInd]; rInd++) {
			if (node->patternStartVals[patInd][rInd] <= point &&
					node->patternStartVals[patInd][rInd] + node->patternRangeLens[patInd][rInd] >= point) {

				/*print("DEBUG: ");
				int baseInd = pairMap[node->nPairId].basePattInds[patInd];
				printf("%d, %d, ", point, baseInd);*/
				for (int k = 0; k < pairMap[node->nPairId].numPorts; k++) {
					// printf("%d, %3.2f, ", node->points[point].motPower[0], pairMap[node->nPairId].pattern[patInd][k]);
					motorSet(pairMap[node->nPairId].motorPorts[k], (int)round(node->points[point].motPower[0] * pairMap[node->nPairId].pattern[patInd][k]));
				}
				// print("\n\r");
				return;
			}
		}
	}

	// print("DEBUG: ");
	for (int i = 0; i < pairMap[node->nPairId].numPorts; i++) {
		motorSet(pairMap[node->nPairId].motorPorts[i], node->points[point].motPower[i]);
		// printf("%d, ", node->points[point].motPower[i]);
	}
	// print("\n\r");
}

/*bool sensorsReachedPoint(MoveNode *currNode, short *sensorPoints, short *sensorVals, int *dir) {
	for (int i = 0; i < pairMap[currNode->nPairId].numSensors; i++) {
		if (!inBounds(currNode)) {
			continue;
		}

		// printf("DEBUG: %d %d %d\n\r", dir[i], sensorVals[i], sensorPoints[i]);
		if (dir[i] * abs(sensorVals[i]) <
				dir[i] * (abs(sensorPoints[i]) - (sensorPoints[i] >= 0 ? 1 : -1) * 5)) {
			free(dir);
			return false;
		}
	}
	free(dir);
	return true;
}*/

bool reachedPoint(MoveNode *currNode, short *sensorPoints, short *prevPoints) {
	int *dir = malloc(pairMap[currNode->nPairId].numSensors * sizeof(*(dir)));
	if (dir == NULL) {
		outOfMemory = true;
		return false;
	}
	for (int i = 0; i < pairMap[currNode->nPairId].numSensors; i++) {
		if (sensorPoints[i] == prevPoints[i]) {
			dir[i] = 0;
			continue;
		}
		dir[i] = (abs(sensorPoints[i])
				- abs(prevPoints[i]) > 0 ? 1 : -1);
	}

	short *sensorVals = getAllSensorValues(&pairMap[currNode->nPairId]);
	if (sensorVals == NULL) {
		outOfMemory = true;
		return false;
	}

	for (int i = 0; i < pairMap[currNode->nPairId].numSensors; i++) {
		// if (DEBUG) {
		printf("DEBUG: %d %d %d %d\n\r", i, dir[i], sensorVals[i], sensorPoints[i]);
		// }
		if (dir[i] == 0) {
			continue;
		}

		if (dir[i] * abs(sensorVals[i]) <
				dir[i] * (abs(sensorPoints[i]) - 5)) {
			free(dir);
			free(sensorVals);
			return false;
		}
	}
	free(dir);
	free(sensorVals);
	return true;
}

bool inMotion() {
	for (int i = 1; i <= 10; i++) {
		// printf("DEBUG: %d %d\n\r", i, motorGet(i));
		if (motorGet(i) != 0) {
			return true;
		}
	}
	return false;
}

void runCurrentPath(MoveNode *startNode, short endId) {
	bool needToStart(MoveNode *currNode, MoveNode *currChild) {
		short *sensorVals = getAllSensorValues(&pairMap[currNode->nPairId]);
		if (sensorVals == NULL) {
			outOfMemory = true;
			return false;
		}
		for (int i = 0; i < pairMap[currNode->nPairId].numSensors; i++) {
			bool goingUp = abs(currNode->points[currChild->startPoint].endSensorVal[i])
										 - abs(currNode->points[currChild->startPoint - 1].endSensorVal[i])
										 >= 0;
			if (goingUp) {
				if (abs(sensorVals[i]) < abs(currChild->startVal[i])) {
					free(sensorVals);
					return false;
				}
			} else {
				if (abs(sensorVals[i]) > abs(currChild->startVal[i])) {
					free(sensorVals);
					return false;
				}
			}
		}
		free(sensorVals);
		return true;
	}

	void runNode(void *data) {
		MoveNode *currNode = (MoveNode *)data;
		MoveNode *currChild = currNode->child;

		if (currNode->nodeId == NULL) {
			return;
		}

		for (int i = 0; i < pairMap[currNode->nPairId].numSensors; i++) {
			Sensor *sensor = &pairMap[currNode->nPairId].sensors[i];
			// createSensor(sensor);
			/*if (sensor->type == SHAFT_ENCODER) {
				sensor->enc = encoderInit(sensor->port, sensor->port + 1, false);
			}*/
			startSensor(sensor);
			sensor = NULL;
		}

		signed char *saveState = NULL;

		printDebug("Started node.");
		// printf("Started node %d.\n\r", currNode->nodeId);
		// printf("Node's child: %d\n\r", currChild->nodeId);
		int nextPoint = 0;
		setMotorSpeeds(currNode, nextPoint);
		nextPoint++;
		while (nextPoint < currNode->numPoints) {
			// printf("DEBUG: %d\n\r", nextPoint);
			while (joystickGetDigital(1, 5, JOY_UP)) { // pause
				if (saveState == NULL) {
					saveState = malloc(pairMap[currNode->nPairId].numPorts * sizeof(*(saveState)));
					if (saveState == NULL) {
						return; // break completely
					}
					for (int i = 0; i < pairMap[currNode->nPairId].numPorts; i++) {
						saveState[i] = motorGet(pairMap[currNode->nPairId].motorPorts[i]);
						motorStop(pairMap[currNode->nPairId].motorPorts[i]);
					}
					for (int i = 0; i < pairMap[currNode->nPairId].numSensors; i++) {
						if (pairMap[currNode->nPairId].sensors[i].type == TIME) {
							pauseTimer(pairMap[currNode->nPairId].sensors[i].port, true);
						}
					}
					printDebug("Paused!");
				}

				if (joystickGetDigital(1, 8, JOY_DOWN)) { // stop it entirely
					printDebug("Stopping!");
					for (int i = 0; i < pairMap[currNode->nPairId].numSensors; i++) {
						if (pairMap[currNode->nPairId].sensors[i].type == TIME) {
							resumeTimer(pairMap[currNode->nPairId].sensors[i].port, true);
						}
					}
					free(saveState);
					saveState = NULL;
					return;
				}
				delay(20);
			}

			if (outOfMemory) {
				return;
			}

			if (saveState != NULL) {
				for (int i = 0; i < pairMap[currNode->nPairId].numPorts; i++) {
					motorSet(pairMap[currNode->nPairId].motorPorts[i], saveState[i]);
				}

				for (int i = 0; i < pairMap[currNode->nPairId].numSensors; i++) {
					if (pairMap[currNode->nPairId].sensors[i].type == TIME) {
						resumeTimer(pairMap[currNode->nPairId].sensors[i].port, true);
					}
				}
				free(saveState);
				saveState = NULL;
			}

			if (reachedPoint(currNode, currNode->points[nextPoint].endSensorVal, currNode->points[nextPoint - 1].endSensorVal)) {
				setMotorSpeeds(currNode, nextPoint);
				nextPoint++;
			}

			if (currChild != NULL) {
				// printf("DEBUG: %d %d %d\n\r", currChild->nodeId, nextPoint, currChild->startPoint);
				if (nextPoint + 1 >= currChild->startPoint && needToStart(currNode, currChild)) {
					void *param = (void *)currChild;
					taskCreate(runNode, TASK_DEFAULT_STACK_SIZE / 2, param, TASK_PRIORITY_DEFAULT);
					currChild = currChild->sibling;
				}
			}
			delay(5);
		}

		printDebug("Finished node.");
		// printf("Finished node %d.", currNode->nodeId);
		if (findParent(currNode)->nodeId == rootNode->nodeId) {
			while (inMotion()) {
				delay(20);
			}
			if (currNode->sibling != NULL) {
				delay(currNode->sibling->startVal[0]);
				runNode(currNode->sibling);
			} else {
				printDebug("Done.");
				delay(500);
			}
		}
	}
	// mutexTake(doNotRecord, 1000);

	// MoveNode *startNode = rootNode->child;
	if (startNode != NULL) {
		outOfMemory = false;
		runNode(startNode);
		startNode = NULL;
	}

	/*while (inMotion()) {
		delay(20);
	}*/

	//printDebug("waiting");
	//printDebug("Hi");

	// mutexGive(doNotRecord);
}
