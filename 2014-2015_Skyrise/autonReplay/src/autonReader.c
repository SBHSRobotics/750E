/*
 * autonReader.c
 *
 *  Created on: Nov 7, 2014
 *      Author: Matthew
 */

#include "main.h"

/*
 * TODO: Pneumatics
 * TODO: Fix any hanging pointers
 * TODO: Pass all structs into functions using pointers - Done?
 * TODO: Get better malloc checks - Done?
 *
 * (Really far away) TODO: Currently, a lot of space is being sent towards
 * the structs and pointers for each MoveNode, which means that as much as
 * 50% of the space being used does not directly "contain" data. There are
 * a couple of ways to redesign the program to use less structs/pointers,
 * which will all take a significant amount of time refactoring and reorganizing.
 */

//#define PTR_SIZE(ptr) sizeof(ptr) / sizeof(ptr[0])

Encoder encoder;
Gyro gyro;

MotorSensorPair *pairMap;
int numPairs = 0;
int totalTreeSize = 0;
bool outOfMemory = false;
bool pairsLoaded = false;

unsigned char lastUniqueId = 0;
unsigned char lastUniquePairId = 0;
MoveNode *currNode = NULL, *rootNode = NULL;
// char * strcat ( char * destination, const char * source );

Mutex readerInit;
Mutex doNotRecord;

bool redrawDisplay;
bool movedCursor;

const bool DEBUG = false;
int flushCount = 0;

void printError(char *str) {
	printf("ERROR: %s\n\r", str);
	flushCount++;
	if (flushCount >= 15) {
		fflush(stdout);
		flushCount = 0;
	}
}

void printDebug(char *str) {
	if (DEBUG) {
		printf("DEBUG: %s\n\r", str);
		flushCount++;
		if (flushCount >= 15) {
			fflush(stdout);
			flushCount = 0;
		}
	}
}

void printSize(void) {
	if (!DEBUG) {
		/*while (movedCursor) {
			delay(20);
		}*/
		gotoxy(0, 4);
		print("                                  ");
		gotoxy(0, 4);
		movedCursor = true;
	}
	printf("DEBUG: Total Size - %2.3fKB\n\r", (float)totalTreeSize / (float)1000.0f);
}

void printVoltage(void) {
	if (!DEBUG) {
		gotoxy(0, 3);
		movedCursor = true;
	}
	printf("DEBUG: Main Batt - %1.3fV\n\r", (float)powerLevelMain() / 1000);
}

int getPattInd(int ind, MoveNode *node) {
	for (int patInd = 0; patInd < pairMap[node->nPairId].numPatterns; patInd++) {
		for (int rInd = 0; rInd < node->numRanges[patInd]; rInd++) {
			if (node->patternStartVals[patInd][rInd] <= ind &&
					node->patternStartVals[patInd][rInd] + node->patternRangeLens[patInd][rInd] >= ind) {
				return patInd;
			}
		}
	}
	return -1;
}

bool notAPatternIndex(int ind, MoveNode *node) {
	return getPattInd(ind, node) == -1;
}

void printNode(MoveNode *node) {
	printf("DEBUG: Node ID: %d, Pair ID: %d, Prev Pair ID: %d, Prev Point: %d, Start Val: ",
			node->nodeId, node->nPairId, node->prevPairId, node->startPoint);
	// getchar();
	if (node->nPairId == 0) {
		print("N/A; ");
	} else {
		for (int i = 0; i < pairMap[node->prevPairId].numSensors; i++) {
			printf("%d", node->startVal[i]);
			if (i == pairMap[node->prevPairId].numSensors - 1) {
				print("; ");
			} else {
				print(", ");
			}
			delay(1);
		}
	}
	delay(50);
	// getchar();
	print("Patterns: ");
	if (pairMap[node->nPairId].numPatterns == 0) {
		print("N/A;\n\r");
	} else {
		print("\n\r");
		for (int i = 0; i < pairMap[node->nPairId].numPatterns; i++) {
			for (int j = 0; j < pairMap[node->nPairId].numPorts; j++) {
				printf("%3.5f", pairMap[node->nPairId].pattern[i][j]);
				if (j == pairMap[node->nPairId].numPorts - 1) {
					print(";\n\r");
				} else {
					print(", ");
				}
				delay(50);
				// getchar();
			}
		}

		for (int i = 0; i < pairMap[node->nPairId].numPatterns; i++) {
			printf("%d", node->numRanges[i]);
			if (i == pairMap[node->nPairId].numPatterns - 1) {
				print(";\n\r");
			} else {
				print(", ");
			}
			// getchar();
		}
		delay(50);
		fflush(stdout);

		for (int i = 0; i < pairMap[node->nPairId].numPatterns; i++) {
			for (int j = 0; j < node->numRanges[i]; j++) {
				printf("%d, %d; ", node->patternStartVals[i][j], node->patternRangeLens[i][j]);
				// getchar();
			}
			print("\n\r");
		}
		delay(50);
	}
	delay(50);
	// getchar();
	print("Points:\n\r");
	if (node->numPoints == 0) {
		print("N/A; ");
	} else {
		for (int j = 0; j < node->numPoints; j++) {
			if (notAPatternIndex(j, node)) {
				// printDebug("Not a pat ind.");
				for (int i = 0; i < pairMap[node->nPairId].numPorts; i++) {
					printf("%d", node->points[j].motPower[i]);
					if (i == pairMap[node->nPairId].numPorts - 1) {
						print("; ");
					} else {
						print(", ");
					}
				}
			} else {
				printf("%d; ", node->points[j].motPower[0]);
			}

			for (int i = 0; i < pairMap[node->nPairId].numSensors; i++) {
				printf("%d", node->points[j].endSensorVal[i]);
				if (i == pairMap[node->nPairId].numSensors - 1) {
					print("\n\r");
				} else {
					print(", ");
				}
			}
			delay(1);
			fflush(stdout);
		}
	}
	delay(50);
	// getchar();

	print("Child ID: ");
	if (node->child != NULL) {
		printf("%d, ", node->child->nodeId);
	} else {
		print("N/A, ");
	}
	delay(50);

	// getchar();
	print("Sibling ID: ");
	if (node->sibling != NULL) {
		printf("%d\n\r", node->sibling->nodeId);
	} else {
		print("N/A\n\r");
	}
	// getchar();
	delay(50);

	//printDebug("Press any key to continue.\n\r");
	fflush(stdout);
	//getchar();
}

void printPair(MotorSensorPair *pair) {
	if (DEBUG) {
		printf("DEBUG: Pair ID: %d, Sensors: ", pair->pairId);
		for (int i = 0; i < pair->numSensors; i++) {
			printf("%d %d, ", pair->sensors[i].type, pair->sensors[i].port);
		}
		print("Motor Ports: ");
		for (int i = 0; i < pair->numPorts; i++) {
			printf("%d ", pair->motorPorts[i]);
		}
		print("\n\r");
		fflush(stdout);
	}
}

void addMotor(MotorSensorPair *pair, unsigned char motPort) {
	if (pair == NULL) {
		return;
	}

	if (pair->numPorts == 0) {
		pair->motorPorts = malloc(sizeof(*(pair->motorPorts)));
		if (pair->motorPorts == NULL) {
			printError("Failed to create port list!");
			outOfMemory = true;
			return;
		}
	} else {
		unsigned char *tempPorts = malloc((pair->numPorts + 1) * sizeof(*(pair->motorPorts)));
		if (tempPorts == NULL) {
			printError("Failed to resize port list!");
			outOfMemory = true;
			return;
		} else {
			for (int j = 0; j < pair->numPorts; j++) {
				tempPorts[j] = pair->motorPorts[j];
			}
			free(pair->motorPorts);
			pair->motorPorts = tempPorts;
		}
	}

	pair->motorPorts[pair->numPorts++] = motPort;
}

void addSensor(MotorSensorPair *pair, SensorType type, unsigned char port) {
	if (pair == NULL) {
		return;
	}

	if (pair->numSensors == 0) {
		pair->sensors = malloc(sizeof(*(pair->sensors)));
		if (pair->sensors == NULL) {
			printError("Failed to create sensor list!");
			outOfMemory = true;
			return;
		}
	} else {
		Sensor *tempSens = malloc((pair->numSensors + 1) * sizeof(*(pair->sensors)));
		if (tempSens == NULL) {
			printError("Failed to resize sensor list!");
			outOfMemory = true;
			return;
		} else {
			for (int j = 0; j < pair->numSensors; j++) {
				tempSens[j] = pair->sensors[j];
			}
			free(pair->sensors);
			pair->sensors = tempSens;
		}
	}

	pair->sensors[pair->numSensors++] = createSensor(type, port);
}

MotorSensorPair *initNewPair(void) {
	MotorSensorPair *pair = malloc(sizeof(*(pair)));
	if (pair == NULL) {
		printError("Failed to register pair!");
		outOfMemory = true;
		return NULL;
	}
	pair->numPorts = 0;
	pair->numSensors = 0;
	pair->motorPorts = NULL;
	pair->sensors = NULL;
	pair->pairId = lastUniquePairId++;
	pair->running = false;
	pair->pairStr = NULL;
	pair->numPatterns = 0;
	pair->basePattInds = NULL;
	pair->pattern = NULL;

	if (numPairs == 0) {
		pairMap = malloc(sizeof(*pairMap));
		if (pairMap == NULL) {
			free(pair);
			printError("Failed to initialize pair map!");
			outOfMemory = true;
			return NULL;
		}
	} else {
		MotorSensorPair *temp = malloc((numPairs + 1) * sizeof(*pairMap));
		if (temp == NULL) {
			free(pair);
			printError("Failed to resize pair map!");
			outOfMemory = true;
			return NULL;
		} else {
			for (int i = 0; i < numPairs; i++) {
				temp[i] = pairMap[i];
			}
			free(pairMap);
			pairMap = temp;
		}
	}
	pairMap[numPairs++] = *pair;

	pair = &pairMap[numPairs - 1];
	return pair;
}

void initSensorMap(void) {
	MotorSensorPair *newPair = initNewPair();
	/*newPair->pairStr = "test";
	addMotor(newPair, 2);
	addMotor(newPair, 3);
	addSensor(newPair, TIME, 1);*/
	newPair->pairStr = "chassis";
	addMotor(newPair, 1);
	addMotor(newPair, 2);
	addMotor(newPair, 3);
	addMotor(newPair, 8);
	addMotor(newPair, 9);
	addMotor(newPair, 10);
	addSensor(newPair, TIME, 1);
	addSensor(newPair, SHAFT_ENCODER, 1);
	// addSensor(newPair, TIME, 2);
	addSensor(newPair, GYROSCOPE, 4);
	encoder = newPair->sensors[1].enc;
	gyro = newPair->sensors[2].gyr;
	newPair = initNewPair();
	newPair->pairStr = "arm";
	addMotor(newPair, 4);
	addMotor(newPair, 5);
	addMotor(newPair, 6);
	addMotor(newPair, 7);
	// addSensor(newPair, POTENTIOMETER, 1);
	addSensor(newPair, POTENTIOMETER, 2);
	// addSensor(newPair, TIME, 2);
	/*newPair = initNewPair();
	newPair->pairStr = "strafe";
	addMotor(newPair, 3);
	addSensor(newPair, TIME, 1);*/
	newPair = NULL;
}

short *getAllSensorValues(MotorSensorPair *pair) {
	short *sensorVals = malloc(pair->numSensors * sizeof(*sensorVals));
	if (sensorVals == NULL) {
		printError("Failed to get sensor values!");
		outOfMemory = true;
		return NULL;
	}

	for (int i = 0; i < pair->numSensors; i++) {
		sensorVals[i] = getSensorValue(pair->sensors[i]);
		if (pair->sensors[i].type == SHAFT_ENCODER && DEBUG) {
			printf("%d\n\r", sensorVals[i]);
		}
	}
	return sensorVals;
}

MoveNode *createNode(MotorSensorPair *pair, signed char prevPairId, short *startVals, short startPoint) {
	if (startVals == NULL && prevPairId >= 0) {
		return NULL;
	}

	MoveNode *newNode = malloc(sizeof(*newNode));
	if (newNode == NULL) {
		printError("Failed to initialize node.");
		outOfMemory = true;
		return NULL;
	}

	newNode->nodeId = lastUniqueId++;
	newNode->nPairId = pair->pairId;
	newNode->prevPairId = prevPairId;
	newNode->startPoint = startPoint;
	newNode->numPoints = 0;
	newNode->points = NULL;
	newNode->child = NULL;
	newNode->sibling = NULL;

	if (pair->numPatterns == 0) {
		newNode->numRanges = NULL;
		newNode->patternStartVals = NULL;
		newNode->patternRangeLens = NULL;
	} else {
		newNode->numRanges = malloc(pair->numPatterns * sizeof(*(newNode->numRanges)));
		if (newNode->numRanges == NULL) {
			free(newNode);
			printError("Failed to initialize number of ranges!");
			outOfMemory = true;
			return NULL;
		}
		newNode->patternStartVals = malloc(pair->numPatterns * sizeof(*(newNode->patternStartVals)));
		if (newNode->patternStartVals == NULL) {
			free(newNode->numRanges);
			free(newNode);
			printError("Failed to initialize patt start vals!");
			outOfMemory = true;
			return NULL;
		}
		newNode->patternRangeLens = malloc(pair->numPatterns * sizeof(*(newNode->patternRangeLens)));
		if (newNode->patternRangeLens == NULL) {
			free(newNode->patternStartVals);
			free(newNode->numRanges);
			free(newNode);
			printError("Failed to initialize patt range lens!");
			outOfMemory = true;
			return NULL;
		}
		for (int i = 0; i < pair->numPatterns; i++) {
			newNode->numRanges[i] = 0;
			newNode->patternStartVals[i] = NULL;
			newNode->patternRangeLens[i] = NULL;
		}
	}

	newNode->startVal = malloc(pairMap[prevPairId].numSensors * sizeof(*(newNode->startVal)));
	if (newNode->startVal == NULL) {
		free(newNode->patternRangeLens);
		free(newNode->patternStartVals);
		free(newNode->numRanges);
		free(newNode);
		printError("Failed to initialize start values!");
		outOfMemory = true;
		return NULL;
	}
	for (int i = 0; i < pairMap[prevPairId].numSensors; i++) {
		newNode->startVal[i] = startVals[i];
	}
	free(startVals);

	return newNode;
}

void addMoveNode(MotorSensorPair *pair) {
	MoveNode *node;

	if (currNode == NULL) {
		node = createNode(pair, -1, NULL, -1);
		if (node != NULL) {
			currNode = node;
			rootNode = currNode;
		}
	} else if (currNode->child == NULL) {
		if (currNode->nPairId == 0) {
			short *noStartVal = malloc(sizeof(*(noStartVal)));
			if (noStartVal == NULL) {
				printError("Failed to initialize initial start value!");
				outOfMemory = true;
				return;
			}
			*noStartVal = 100;
			// printDebug("First node created!");
			node = createNode(pair, 0, noStartVal, -1);
		} else {
			node = createNode(pair, currNode->nPairId, getAllSensorValues(&pairMap[currNode->nPairId]), currNode->numPoints);
		}
		if (node != NULL) {
			currNode->child = node;
			// printDebug("Child created!");
			currNode = currNode->child;
		}
	} else {
		MoveNode *siblingNode = currNode->child;
		while (siblingNode->sibling != NULL) {
			siblingNode = siblingNode->sibling;
		}
		node = createNode(pair, currNode->nPairId, getAllSensorValues(&pairMap[currNode->nPairId]), currNode->numPoints);
		if (node != NULL) {
			siblingNode->sibling = node;
			// printDebug("Sibling created!");
			currNode = siblingNode->sibling;
		}
	}
	redrawDisplay = true;
}

MoveNode *findParent(MoveNode *targetChild) {
	MoveNode *innerFindParent(MoveNode* child, MoveNode* parent, short id) {
		if (child->nodeId == id) {
			return parent;
		}

		if (child->sibling != NULL) {
			if (child->sibling->nodeId <= id) {
				return innerFindParent(child->sibling, parent, id);
			}
		}

		if (child->child != NULL) {
			return innerFindParent(child->child, child, id);
		}

		return NULL;
	}

	return innerFindParent(rootNode, NULL, targetChild->nodeId);
}

unsigned char getBaseInd(signed char *currSpeed, MotorSensorPair *pair) {
	for (unsigned char i = 0; i < pair->numPorts; i++) {
		if (currSpeed[i] != 0) {
			return i;
		}
	}
	return 0;
}

void updateMotorSpeeds(SpeedPoint *point, MoveNode *node, signed char *currSpeed) {
	MotorSensorPair *pair = &pairMap[node->nPairId];

	void addMotorVals(SpeedPoint *pt, signed char *speeds, bool isPattern) {
		if (pt->motPower != NULL) {
			free(pt->motPower);
		}

		if (isPattern) {
			pt->motPower = malloc(sizeof(*(pt->motPower)));
		} else {
			pt->motPower = malloc(pair->numPorts * sizeof(*(pt->motPower)));
		}

		if (pt->motPower == NULL) {
			printError("Failed to allocate extra motor speeds!");
			outOfMemory = true;
			return;
		}

		if (isPattern) {
			unsigned char baseInd = getBaseInd(speeds, pair);
			pt->motPower[0] = speeds[baseInd];
		} else {
			for (int i = 0; i < pair->numPorts; i++) {
				pt->motPower[i] = speeds[i];
			}
		}
		free(speeds);
	}

	// Returns boolean that says whether or not it found a preexisting pattern
	bool saveSpeedsAsPattern(signed char *speeds, int patInd) {
		printDebug("Trying to save!");
		unsigned char baseInd = pair->basePattInds[patInd];
		for (int j = 0; j < pair->numPorts; j++) {
			// TODO: Get a better equality check here
			if (speeds[j] != speeds[baseInd] * pair->pattern[patInd][j]) {
				break; // go to next pattern
			} else if (j == pair->numPorts - 1 && pair->numPorts > 1) {
				int rInd = 0;
				do {
					if (node->numRanges[patInd] != 0 && node->patternStartVals[patInd][rInd] + node->patternRangeLens[patInd][rInd] == node->numPoints - 1) {
						printDebug("Resized range!");
						node->patternRangeLens[patInd][rInd]++; // it exists
						break;
					} else if (node->numRanges[patInd] == 0 || rInd == node->numRanges[patInd] - 1) {
						// create a new one
						printDebug("Making new range!");
						short *newStartVals = malloc((node->numRanges[patInd] + 1) * sizeof(*(newStartVals)));
						if (newStartVals == NULL) {
							printError("Failed to allocate new start values!");
							outOfMemory = true;
							return true; // this will stop the update loop immediately
						}
						for (int k = 0; k < node->numRanges[patInd]; k++) {
							newStartVals[k] = node->patternStartVals[patInd][k];
						}
						if (node->patternStartVals[patInd] != NULL) {
							free(node->patternStartVals[patInd]);
						}
						node->patternStartVals[patInd] = newStartVals;
						node->patternStartVals[patInd][node->numRanges[patInd]] = node->numPoints;

						short *newRanges = malloc((node->numRanges[patInd] + 1) * sizeof(*(newRanges)));
						if (newRanges == NULL) {
							free(node->patternStartVals[patInd]);
							printError("Failed to allocate new range length!");
							outOfMemory = true;
							return true;
						}
						for (int k = 0; k < node->numRanges[patInd]; k++) {
							newRanges[k] = node->patternRangeLens[patInd][k];
						}
						if (node->patternRangeLens[patInd] != NULL) {
							free(node->patternRangeLens[patInd]);
						}
						node->patternRangeLens[patInd] = newRanges;
						node->patternRangeLens[patInd][node->numRanges[patInd]] = 0;

						node->numRanges[patInd]++;
						break;
					}
					rInd++;
				} while (rInd < node->numRanges[patInd]);

				printDebug("Added pattern values!");
				addMotorVals(point, speeds, true);
				return true;
			}
		}
		return false;
	}

	float *getPattern(signed char *currSpeed, unsigned char baseInd) {
		float *newPatt = malloc(pair->numPorts * sizeof(*(newPatt)));
		if (newPatt == NULL) {
			printError("Failed to allocate new pattern!");
			outOfMemory = true;
			return NULL;
		}

		if (pair->numPorts > 0) {
			for (int i = 0; i < pair->numPorts; i++) {
				if (currSpeed[baseInd] != 0) {
					// TODO: Round later?
					newPatt[i] = (float)currSpeed[i] / (float)currSpeed[baseInd];
				} else {
					newPatt[i] = 0;
				}
				// printf("%3.5f\n\r", newPatt[i]);
			}
		}

		return newPatt;
	}

	bool patternsMatch(float *patt1, float *patt2) {
		for (int ind = 0; ind < pair->numPorts; ind++) {
			// TODO: Better equality check
			if (patt1[ind] != patt2[ind]) {
				return false;
			}
		}
		return true;
	}

	void updateNodePatterns(MoveNode *node, unsigned char pairId, short skipId) {
		if (node->nodeId != skipId && node->nPairId == pairId) {
			if (DEBUG) {
				printf("DEBUG: Updated ranges for node %d\n\r", node->nodeId);
			}
			short *temp = malloc(pairMap[node->nPairId].numPatterns * sizeof(*(temp)));
			if (temp == NULL) {
				printError("Failed to create new number of ranges during update!");
				outOfMemory = true;
				return;
			}
			for (int i = 0; i < pairMap[node->nPairId].numPatterns - 1; i++) {
				temp[i] = node->numRanges[i];
			}
			temp[pairMap[node->nPairId].numPatterns - 1] = 0;
			free(node->numRanges);
			node->numRanges = temp;

			short **tempStartVals = malloc(pairMap[node->nPairId].numPatterns * sizeof(*(tempStartVals)));
			if (tempStartVals == NULL) {
				printError("Failed to create new start values during update!");
				outOfMemory = true;
				return;
			}
			for (int i = 0; i < pairMap[node->nPairId].numPatterns - 1; i++) {
				if (node->numRanges[i] != 0) {
					// tempStartVals = malloc(node->numRanges[i] * sizeof(*(tempStartVals)));
					tempStartVals[i] = node->patternStartVals[i];
					// free(node->patternStartVals[i]);
				} else {
					tempStartVals[i] = NULL;
				}
			}
			tempStartVals[pairMap[node->nPairId].numPatterns - 1] = NULL;
			free(node->patternStartVals);
			node->patternStartVals = tempStartVals;

			short **tempRangeLens = malloc(pairMap[node->nPairId].numPatterns * sizeof(*(tempRangeLens)));
			if (tempRangeLens == NULL) {
				printError("Failed to create new range lengths during update!");
				outOfMemory = true;
				return;
			}
			for (int i = 0; i < pairMap[node->nPairId].numPatterns - 1; i++) {
				if (node->numRanges[i] != 0) {
					// tempRangeLens[i] = malloc(node->numRanges[i] * sizeof(*(tempRangeLens[i])));
					tempRangeLens[i] = node->patternRangeLens[i];
					// free(node->patternRangeLens[i]);
				} else {
					tempRangeLens[i] = NULL;
				}
			}
			tempRangeLens[pairMap[node->nPairId].numPatterns - 1] = NULL;
			free(node->patternRangeLens);
			node->patternRangeLens = tempRangeLens;
			// printDebug("Updated ranges!");
		}

		if (node->child != NULL) {
			updateNodePatterns(node->child, pairId, skipId);
		}

		if (node->sibling != NULL) {
			updateNodePatterns(node->sibling, pairId, skipId);
		}
	}

	//for (int i = 0; i < node->numPatterns; i++) {
	int i = 0;
	do {
		if (pair->numPatterns != 0 && saveSpeedsAsPattern(currSpeed, i)) {
			// we're done
			printDebug("Saved as pattern!");
			return;
		} else if (pair->numPatterns == 0 || i == pair->numPatterns - 1) {
			// try to find a new pattern
			printDebug("No pattern!");
			float **newPatterns = malloc(sizeof(*(newPatterns)));
			if (newPatterns == NULL) {
				printError("Failed to create new pattern list!");
				outOfMemory = true;
				return;
			}
			newPatterns[0] = getPattern(currSpeed, getBaseInd(currSpeed, pair));
			if (newPatterns[0] == NULL) {
				return;
			}
			int numNewPatterns = 1;

			for (int j = 0; j < node->numPoints; j++) {
				printDebug("Checking point!");
				// getchar();
				if (notAPatternIndex(j, node)) {
					unsigned char newInd = getBaseInd(node->points[j].motPower, pair);
					float *newPatt = getPattern(node->points[j].motPower, newInd);
					if (newPatt == NULL) {
						return;
					}
					bool matched = false;

					for (int k = 0; k < numNewPatterns; k++) {
						// printDebug("Checking pattern!");
						// getchar();
						if (patternsMatch(newPatterns[k], newPatt)) {
							printDebug("New pattern!");
							// getchar();
							// there is a new pattern
							float **temp = malloc((pair->numPatterns + 1) * sizeof(*(temp)));
							if (newPatterns == NULL) {
								free(newPatterns);
								printError("Failed to create new pattern after finding a new pattern!");
								outOfMemory = true;
								return;
							}
							for (int l = 0; l < pair->numPatterns; l++) {
								temp[l] = pair->pattern[l];
							}
							temp[pair->numPatterns] = malloc(pair->numPorts * sizeof(*(temp[pair->numPatterns])));
							if (temp[pair->numPatterns] == NULL) {
								free(newPatterns);
								free(temp);
								printError("Failed to create new pattern after finding a new pattern!");
								outOfMemory = true;
								return;
							}

							if (pair->pattern != NULL) {
								free(pair->pattern);
							}
							pair->pattern = temp;
							pair->pattern[pair->numPatterns++] = newPatt;

							int pattSize = pair->numPatterns - 1;
							unsigned char *tempBaseInds = malloc((pattSize + 1) * sizeof(*(tempBaseInds)));
							if (tempBaseInds == NULL) {
								free(newPatterns);
								free(pair->pattern[pattSize]);
								pair->numPatterns--;
								printError("Failed to create new base index after finding a new pattern!");
								outOfMemory = true;
								return;
							}
							for (int l = 0; l < pattSize; l++) {
								tempBaseInds[l] = pair->basePattInds[l];
							}
							if (pair->basePattInds != NULL) {
								free(pair->basePattInds);
							}
							pair->basePattInds = tempBaseInds;
							pair->basePattInds[pattSize] = newInd;

							short *tempRanges = malloc((pattSize + 1) * sizeof(*(tempRanges)));
							if (tempRanges == NULL) {
								free(newPatterns);
								free(pair->pattern[pattSize]);
								pair->numPatterns--;
								printError("Failed to create new number of ranges after finding a new pattern!");
								outOfMemory = true;
								return;
							}
							for (int l = 0; l < pattSize; l++) {
								tempRanges[l] = node->numRanges[l];
							}
							if (node->numRanges != NULL) {
								free(node->numRanges);
							}
							node->numRanges = tempRanges;
							if (j + 1 == node->numPoints) {
								node->numRanges[pattSize] = 1;
							} else {
								node->numRanges[pattSize] = 2;
							}

							short **tempStartVals = malloc((pattSize + 1) * sizeof(*(tempStartVals)));
							if (tempStartVals == NULL) {
								free(newPatterns);
								free(pair->pattern[pattSize]);
								pair->numPatterns--;
								printError("Failed to create new start value after finding a new pattern!");
								outOfMemory = true;
								return;
							}
							for (int l = 0; l < pattSize; l++) {
								tempStartVals[l] = malloc(node->numRanges[l] * sizeof(*(tempStartVals[l])));
								if (tempStartVals[l] == NULL) {
									free(newPatterns);
									free(pair->pattern[pattSize]);
									pair->numPatterns--;
									for (int m = 0; m < l; m++) {
										free(tempStartVals[m]);
									}
									free(tempStartVals);
									printError("Failed to create new start value after finding a new pattern!");
									outOfMemory = true;
									return;
								}
								for (int m = 0; m < node->numRanges[l]; m++) {
									tempStartVals[l][m] = node->patternStartVals[l][m];
								}
								free(node->patternStartVals[l]);
							}
							/*tempStartVals[pattSize] = malloc(sizeof(int));*/

							if (node->patternStartVals != NULL) {
								free(node->patternStartVals);
							}
							node->patternStartVals = tempStartVals;
							// node->patternStartVals[pattSize][0] = j;
							if (j + 1 != node->numPoints) {
								// free(node->patternStartVals[pattSize]);
								node->patternStartVals[pattSize] = malloc(2 * sizeof(*(node->patternStartVals[pattSize])));
								if (node->patternStartVals[pattSize] == NULL) {
									free(newPatterns);
									free(pair->pattern[pattSize]);
									pair->numPatterns--;
									printError("Failed to create new start value after finding a new pattern!");
									outOfMemory = true;
									return;
								}
								node->patternStartVals[pattSize][0] = j;
								node->patternStartVals[pattSize][1] = node->numPoints;
							} else {
								node->patternStartVals[pattSize] = malloc(sizeof(*(node->patternStartVals[pattSize])));
								if (node->patternStartVals[pattSize] == NULL) {
									free(newPatterns);
									free(pair->pattern[pattSize]);
									pair->numPatterns--;
									printError("Failed to create new start value after finding a new pattern!");
									outOfMemory = true;
									return;
								}
								node->patternStartVals[pattSize][0] = j;
							}

							short **tempRangeLens = malloc((pattSize + 1) * sizeof(*(tempRangeLens)));
							if (tempRangeLens == NULL) {
								free(newPatterns);
								free(pair->pattern[pattSize]);
								pair->numPatterns--;
								free(node->patternStartVals[pattSize]);
								printError("Failed to create new range length after finding a new pattern!");
								outOfMemory = true;
								return;
							}
							for (int l = 0; l < pattSize; l++) {
								tempRangeLens[l] = malloc(node->numRanges[l] * sizeof(*(tempRangeLens[l])));
								if (tempRangeLens[l] == NULL) {
									free(newPatterns);
									free(pair->pattern[pattSize]);
									pair->numPatterns--;
									free(node->patternStartVals[pattSize]);
									for (int m = 0; m < l; m++) {
										free(tempRangeLens[m]);
									}
									free(tempRangeLens);
									printError("Failed to create new range length after finding a new pattern!");
									outOfMemory = true;
									return;
								}
								for (int m = 0; m < node->numRanges[l]; m++) {
									tempRangeLens[l][m] = node->patternRangeLens[l][m];
								}
								free(node->patternRangeLens[l]);
							}
							// tempRangeLens[pattSize] = malloc(sizeof(int));

							if (node->patternRangeLens != NULL) {
								free(node->patternRangeLens);
							}
							node->patternRangeLens = tempRangeLens;
							if (j + 1 == node->numPoints) {
								node->patternRangeLens[pattSize] = malloc(sizeof(*(node->patternRangeLens[pattSize])));
								if (node->patternRangeLens[pattSize] == NULL) {
									free(newPatterns);
									free(pair->pattern[pattSize]);
									pair->numPatterns--;
									free(node->patternStartVals[pattSize]);
									printError("Failed to create new range length after finding a new pattern!");
									outOfMemory = true;
									return;
								}
								node->patternRangeLens[pattSize][0] = 1;
							} else {
								// free(node->patternRangeLens[pattSize]);
								node->patternRangeLens[pattSize] = malloc(2 * sizeof(*(node->patternRangeLens[pattSize])));
								if (node->patternRangeLens[pattSize] == NULL) {
									free(newPatterns);
									free(pair->pattern[pattSize]);
									pair->numPatterns--;
									free(node->patternStartVals[pattSize]);
									printError("Failed to create new range length after finding a new pattern!");
									outOfMemory = true;
									return;
								}
								node->patternRangeLens[pattSize][0] = 0;
								node->patternRangeLens[pattSize][1] = 0;
							}

							signed char *speedsCopy = malloc(pair->numPorts * sizeof(*(speedsCopy)));
							if (speedsCopy == NULL) {
								free(newPatterns);
								free(pair->pattern[pattSize]);
								pair->numPatterns--;
								free(node->patternRangeLens[pattSize]);
								free(node->patternStartVals[pattSize]);
								printError("Failed to create a copy of speeds after finding a new pattern!");
								outOfMemory = true;
								return;
							}
							for (int l = 0; l < pair->numPorts; l++) {
								speedsCopy[l] = node->points[j].motPower[l];
							}

							printDebug("Finished resizing!");
							// getchar();
							addMotorVals(&(node->points[j]), speedsCopy, true);
							addMotorVals(point, currSpeed, true);
							updateNodePatterns(rootNode->child, node->nPairId, node->nodeId);
							matched = true;
							printDebug("Added new values!");
							break;
						}
					}

					if (!matched) {
						printDebug("New (non-matched) pattern!");
						// add as a new pattern
						float **temp = malloc((numNewPatterns + 1) * sizeof(*(temp)));
						if (temp == NULL) {
							printError("Failed to create new pattern during pattern matching!");
							outOfMemory = true;
							return;
						}
						for (int k = 0; k < numNewPatterns; k++) {
							temp[k] = newPatterns[k];
						}

						free(newPatterns);
						newPatterns = temp;
						newPatterns[numNewPatterns++] = newPatt;
					} else {
						// TODO: maybe not actually done, need to reupdate all nodes?
						// savings are negligible (saving a solo pattern in each node,
						// which is highly unlikely)
						// updateNodePatterns(rootNode->child, node->nodeId);
						for (int j = 0; j < numNewPatterns; j++) {
							free(newPatterns[j]);
						}
						free(newPatterns);
						printDebug("Freed memory!");
						return;
					}
				}
			}

			// free stored memory
			for (int j = 0; j < numNewPatterns; j++) {
				free(newPatterns[j]);
			}
			free(newPatterns);
			printDebug("Freed memory!");
		}
		i++;
	} while (i < pair->numPatterns);

	// no pattern
	addMotorVals(point, currSpeed, false);
}

void updateSpeeds(MotorSensorPair pair, signed char* currSpeed, short* sensorVals) {
	if (sensorVals == NULL) {
		return;
	}

	if (DEBUG) {
		print("DEBUG: Stored ");
		for (int i = 0; i < pair.numPorts; i++) {
			printf("%d ", currSpeed[i]);
		}
		print("\n\r");
		print("DEBUG: Read ");
		for (int i = 0; i < pair.numPorts; i++) {
			printf("%d ", motorGet(pair.motorPorts[i]));
		}
		print("\n\r");
	}

	bool changedRecently(MoveNode *changeNode) {
		if (changeNode->numPoints == 1) {
			return false;
		}

		bool isPrevPatt = !notAPatternIndex(changeNode->numPoints - 2, changeNode);
		bool isCurrPatt = !notAPatternIndex(changeNode->numPoints - 1, changeNode);

		SpeedPoint prev = changeNode->points[changeNode->numPoints - 2];
		SpeedPoint curr = changeNode->points[changeNode->numPoints - 1];

		if (isPrevPatt && isCurrPatt) {
			return prev.motPower[0] != curr.motPower[0];
		}

		for (int i = 0; i < pairMap[changeNode->nPairId].numPorts; i++) {
			if (isPrevPatt) {
				int pInd = getPattInd(changeNode->numPoints - 2, changeNode);
				unsigned char baseInd = pairMap[changeNode->nPairId].basePattInds[pInd];
				if (prev.motPower[baseInd] * pairMap[changeNode->nPairId].pattern[pInd][i] == curr.motPower[i]) {
					return false;
				}
			} else if (isCurrPatt) {
				int pInd = getPattInd(changeNode->numPoints - 1, changeNode);
				unsigned char baseInd = pairMap[changeNode->nPairId].basePattInds[pInd];
				if (prev.motPower[i] == curr.motPower[baseInd] * pairMap[changeNode->nPairId].pattern[pInd][i]) {
					return false;
				}
			} else {
				if (prev.motPower[i] == curr.motPower[i]) {
					return false;
				}
			}
		}
		return true;
	}

	bool noChange(MoveNode *changeNode, signed char *currSpeed) {
		if (notAPatternIndex(changeNode->numPoints - 1, changeNode)) {
			for (int i = 0; i < pairMap[changeNode->nPairId].numPorts; i++) {
				if (currSpeed[i] != changeNode->points[changeNode->numPoints - 1].motPower[i]) {
					return false;
				}
			}
			return true;
		} else {
			unsigned char baseInd = getBaseInd(currSpeed, &pairMap[changeNode->nPairId]);
			return currSpeed[baseInd] == changeNode->points[changeNode->numPoints - 1].motPower[0];
		}
	}

	MoveNode* changeNode = currNode;
	while (pairMap[changeNode->nPairId].pairId != pair.pairId) {
		changeNode = findParent(changeNode);
	}

	if (changeNode->numPoints == 0) {   // if we've just started, create the list
		// printDebug("Making new point!");
		changeNode->points = malloc(sizeof(*(changeNode->points)));
		if (changeNode->points == NULL) {
			printError("Failed to initialize speed points!");
			outOfMemory = true;
			return;
		}
	} else if (noChange(changeNode, currSpeed) && !changedRecently(changeNode)) {
		// printDebug("Edited point!");
		for (int i = 0; i < pair.numSensors; i++) {
			changeNode->points[changeNode->numPoints - 1].endSensorVal[i] = sensorVals[i];
		}
		free(sensorVals);
		return;
	} else { // otherwise, we have to add a new point and resize
		// printDebug("Added new point!");
		SpeedPoint* temp = malloc((changeNode->numPoints + 1) * sizeof(*(changeNode->points)));
		if (temp == NULL) {
			printError("Failed to resize speed points!");
			outOfMemory = true;
			return;
		} else {
			for (int i = 0; i < changeNode->numPoints; i++) {
				temp[i] = changeNode->points[i];
			}
			free(changeNode->points);
			changeNode->points = temp;
		}
	}
	SpeedPoint point = {.endSensorVal = malloc(pair.numSensors * sizeof(*(point.endSensorVal))), .motPower = NULL};
	if (point.endSensorVal == NULL) {
		printError("Failed to allocate sensor values for new point!");
		outOfMemory = true;
		return;
	}

	updateMotorSpeeds(&point, changeNode, currSpeed);

	/*for (int i = 0; i < pair.numPorts; i++) {
		point.motPower[i] = currSpeed[i];
		//printf("%d\n\r", currSpeed[i]);
	}
	free(currSpeed);*/

	for (int i = 0; i < pair.numSensors; i++) {
		point.endSensorVal[i] = sensorVals[i];
		//printf("%d\n\r", sensorVals[i]);
	}
	free(sensorVals);

	changeNode->points[(changeNode->numPoints)++] = point;
	//printf("%d %d\n\r", changeNode->points[(changeNode->numPoints) - 1].motPower[0], changeNode->points[(changeNode->numPoints) - 1].endSensorVal[0]);
	//testPoints(*changeNode);
}

int getPairSize(MotorSensorPair pair) {
	int totalSize = 0;
	totalSize += sizeof(pair);
	totalSize += 10 * sizeof(*(pair.pairStr));
	totalSize += pair.numPorts * sizeof(*(pair.motorPorts));
	totalSize += pair.numSensors * sizeof(*(pair.sensors));
	totalSize += pair.numPatterns * sizeof(*(pair.pattern));
	totalSize += pair.numPatterns * pair.numPorts * sizeof(*(pair.pattern[0]));
	totalSize += pair.numPatterns * sizeof(*(pair.basePattInds));

	return totalSize;
}

int getNodeSize(MoveNode* node) {
	int totalSize = 0;

	for (int i = 0; i < node->numPoints; i++) {
		totalSize += pairMap[node->nPairId].numSensors * sizeof(*(node->points[i].endSensorVal));
		if (notAPatternIndex(i, node)) {
			totalSize += pairMap[node->nPairId].numPorts * sizeof(*(node->points[i].motPower));
		} else {
			totalSize += sizeof(*(node->points[i].motPower));
		}
	}

	totalSize += pairMap[node->nPairId].numPatterns * sizeof(*(node->numRanges));
	totalSize += pairMap[node->nPairId].numPatterns * sizeof(*(node->patternStartVals));
	totalSize += pairMap[node->nPairId].numPatterns * sizeof(*(node->patternRangeLens));
	for (int i = 0; i < pairMap[node->nPairId].numPatterns; i++) {
		totalSize += node->numRanges[i] * sizeof(*(node->patternStartVals[i]));
		totalSize += node->numRanges[i] * sizeof(*(node->patternRangeLens[i]));
	}

	totalSize += pairMap[node->prevPairId].numSensors * sizeof(*(node->startVal));
	totalSize += sizeof(*node);
	return totalSize;
}

/**
 * Initializes the recorder by adding the root node and resetting the timer.
 * Also restarts the node ID counter.
 *
 * Parameters: N/A
 * Returns: N/A
 */
void initReplayer(void) {
	initTimers();
	lastUniqueId = 0;							// used in createNode
	MotorSensorPair *timePair = &pairMap[0];	// first pair is always set by the program
	addMoveNode(timePair);

	print("blah");
	// getchar();
	totalTreeSize = 0;
	totalTreeSize += getNodeSize(rootNode);
	for (int i = 0; i < numPairs; i++) {
		totalTreeSize += getPairSize(pairMap[i]);
	}
	if (DEBUG) {
		printSize();
	}
	print("blechsh");
	// getchar();

	startTimer(0, true);						// start waiting for driver action
	pairsLoaded = true;
}

/**
 * Recursively prints the tree using the printNode method, ordered
 * by nodeId.
 *
 * Parameters: N/A
 * Returns: N/A
 */
void printTree(void) {
	void printSelfAndChildren(MoveNode *pNode) {
		printNode(pNode);
		if (pNode->child != NULL) {
			printSelfAndChildren(pNode->child);
		}

		if (pNode->sibling != NULL) {
			printSelfAndChildren(pNode->sibling);
		}
	}

	if (DEBUG) {
		printSelfAndChildren(rootNode);
	}
}

void freePairs(void) {
	pairsLoaded = false;

	for (int i = 0; i < numPairs; i++) {
		MotorSensorPair *pair = &pairMap[i];
		for (int j = 0; j < pair->numPatterns; j++) {
			free(pair->pattern[j]);
		}
		free(pair->pattern);
		// free(pair->pairStr);
		free(pair->motorPorts);
		for (int j = 0; j < pair->numSensors; j++) {
			shutdownSensor(&(pair->sensors[j]));
		}
		free(pair->sensors);
		free(pair->basePattInds);
		if (DEBUG) {
			printf("DEBUG: Freed pointers for pair %d\n\r", pair->pairId);
			getchar();
		}
	}

	free(pairMap);
	numPairs = 0;
	lastUniquePairId = 0;
}

/**
 * Recursvely clears the tree. Frees all allocated memory and restarts
 * the root node along with its timer. NOTE: Does not clear any registered
 * MotorSensorPairs.
 *
 * Parameters: N/A
 * Returns: N/A
 */
void restartTree(void) {
	void freeSelfAndRelatives(MoveNode *fNode) {
		if (fNode->child != NULL) {		// free referenced objects first
			freeSelfAndRelatives(fNode->child);
		}

		if (fNode->sibling != NULL) {
			freeSelfAndRelatives(fNode->sibling);
		}

		/*printf("Freeing node %d\n\r", fNode->nodeId);
		getchar();*/

		if (fNode->startVal != NULL) {
			free(fNode->startVal);			// free variable array of start values
		}
		for (int i = 0; i < pairMap[fNode->nPairId].numPatterns; i++) {
			if (fNode->numRanges[i] != 0) {
				free(fNode->patternStartVals[i]);
				free(fNode->patternRangeLens[i]);
			}
		}
		if (fNode->numRanges != NULL) {
			free(fNode->numRanges);
		}
		if (fNode->patternStartVals != NULL) {
			free(fNode->patternStartVals);
			free(fNode->patternRangeLens);
		}
		for (int i = 0; i < fNode->numPoints; i++) {
			free(fNode->points[i].motPower);
			free(fNode->points[i].endSensorVal);
		}
		if (fNode->points != NULL) {
			free(fNode->points);
		}
		free(fNode);					// free all stored memory related to the node
	}

	freeSelfAndRelatives(rootNode);		// start at root node
	rootNode = NULL;					// clear memory addresses (probably not necessary)
	currNode = NULL;
	delay(100);
	/*print("Freed everything.");
	getchar();*/
	initReplayer();						// add new root node and restart timer
	redrawDisplay = false;
	delay(50);
	if (DEBUG) {
		print("Restarted replayer!");
		getchar();
	}
	// redrawDisplay = true;
}

/**
 * Function responsible for recording motor data when necessary. For
 * every pair registered, it uses several conditionals and a built-in
 * method to determine when to add a node to the tree and update its data.
 *
 * Parameters: N/A
 * Returns: N/A
 */
void recordData(void) {
	bool inMotion(int numPorts, signed char *currSpeed) {	// convenience method for seeing if the robot
		for (int i = 0; i < numPorts; i++) {				// is moving any motor
			if (currSpeed[i] != 0) {						// checks all ports to see if any motors are
				return true;								// non-zero
			}
		}
		return false;
	}

	/**
	 * Convenience method for determining whether or not any MotorSensorPair
	 * is known to be moving through the built-in boolean variable. This does
	 * not check any motor speeds.
	 *
	 * Parameters: N/A
	 * Returns: N/A
	 */
	bool noMotion(void) {
		for (int i = 0; i < numPairs; i++) {
			if (pairMap[i].running) {
				return false;
			}
		}
		return true;
	}

	// print("recording");
	MotorSensorPair *pair;
	for (int i = 1; i < numPairs; i++) {			// starting with pair after default time
		pair = &pairMap[i];
		signed char *currSpeed = malloc(pair->numPorts * sizeof(*(currSpeed)));
		if (currSpeed == NULL) {
			printError("Failed to allocate for current speed check!");
			outOfMemory = true;
			return;
		}
		for (int j = 0; j < pair->numPorts; j++) {	// get the current speed of all registered motors
			int speed = motorGet(pair->motorPorts[j]);
			if (speed == 128) { // PROS is retarded
				currSpeed[j] = 127;
			} else {
				currSpeed[j] = motorGet(pair->motorPorts[j]);
			}
		}
		if (inMotion(pair->numPorts, currSpeed)) {	// if any of the motors are running
			if (!pair->running) {					// if it wasn't running previously,
				//printDebug("Added pair!");		// add it to the tree
				addMoveNode(pair);
				for (int i = 0; i < pair->numSensors; i++) {
					Sensor *sensor = &(pair->sensors[i]);
					startSensor(sensor);			// and start all sensors
					sensor = NULL;
				}
				pair->running = true;				// mark it as currently running
			}
			updateSpeeds(*pair, currSpeed, getAllSensorValues(pair));	// update motor data
			if (outOfMemory) {
				return;
			}
		} else if (pair->running) {
			updateSpeeds(*pair, currSpeed, getAllSensorValues(pair));	// put that last row of 0s in
			if (outOfMemory) {
				return;
			}
			pair->running = false;					// otherwise, it's stopped moving
			totalTreeSize += getNodeSize(currNode);
			if (DEBUG) {
				printSize();
			}
			while (!pairMap[currNode->nPairId].running) {		// find the correct parent node
				currNode = findParent(currNode);
				printDebug("Reverted to parent!");
				if (currNode->nPairId == pairMap[0].pairId) {
					break;							// don't go past the root node
				}
			}
			if (noMotion()) {						// if there is no motion at all,
				printDebug("Reset timer!");			// start the default timer
				startTimer(0, true);
			}
		} else {
			free(currSpeed);	// to avoid memory leaks
		}
	}
	mutexGive(doNotRecord);
}

/**
 * Debug purposes only. Measures the amount of memory left in the Cortex by
 * repeatedly allocating memory in blocks of 10^n until a NULL pointer is
 * returned. May not be the best way, but it works. Frees all memory after
 * function is done.
 *
 * Parameters: N/A
 * Returns: # of available memory in bytes.
 */
int measureAvailableMemory(void) {
	int amount = 1000;				// initial amount: 1 KB
	int powTen = 1000;				// var to store the power of 10 to decrease the
	void *p = malloc(amount);		// storage by
	while (powTen > 0) {
		if (p != NULL) {
			free(p);				// if we can, free the previous block
			delay(5);				// and allocate more memory
			amount += powTen;
			p = malloc(amount);
			delay(5);
		} else {					// else, decrease the amount and power of ten
			amount -= powTen;		// and keep going
			powTen /= 10;
			p = malloc(amount);
			delay(5);
		}
	}
	free(p);						// free all used memory
	return amount;					// return the total amount used
}

void createTestTree(void) {
	MotorSensorPair *testPair1 = &pairMap[1];
	addMoveNode(testPair1);

	signed char *currSpeed = malloc(sizeof(*(currSpeed)) * 6);
	currSpeed[0] = 127;
	currSpeed[1] = 0;
	currSpeed[2] = 127;
	currSpeed[3] = 0;
	currSpeed[4] = 127;
	currSpeed[5] = 0;
	short *currSensors = malloc(sizeof(*(currSensors)) * 2);
	currSensors[0] = 100;
	currSensors[1] = 100;
	short *startVal = malloc(sizeof(*(startVal)));
	startVal[0] = 20;
	currNode->startVal = startVal;
	updateSpeeds(pairMap[1], currSpeed, currSensors);

	currSpeed = malloc(sizeof(*(currSpeed)) * 6);
	currSpeed[0] = 64;
	currSpeed[1] = 0;
	currSpeed[2] = 64;
	currSpeed[3] = 0;
	currSpeed[4] = 64;
	currSpeed[5] = 0;
	currSensors = malloc(sizeof(*(currSensors)) * 2);
	currSensors[0] = 200;
	currSensors[1] = 200;
	updateSpeeds(pairMap[1], currSpeed, currSensors);

	currSpeed = malloc(sizeof(*(currSpeed)) * 6);
	currSpeed[0] = -127;
	currSpeed[1] = -127;
	currSpeed[2] = -127;
	currSpeed[3] = 0;
	currSpeed[4] = 127;
	currSpeed[5] = 0;
	currSensors = malloc(sizeof(*(currSensors)) * 2);
	currSensors[0] = 120;
	currSensors[1] = 120;
	updateSpeeds(pairMap[1], currSpeed, currSensors);

	/*currNode = findParent(currNode);
	MotorSensorPair *testPair2 = &pairMap[1];
	addMoveNode(testPair2);
	startVal = malloc(sizeof(*(startVal)));
	startVal[0] = 20;
	currNode->startVal = startVal;*/

	MotorSensorPair *testPair4 = &pairMap[2];
	addMoveNode(testPair4);
	startVal = malloc(2 * sizeof(*(startVal)));
	startVal[0] = 20;
	startVal[1] = 1000;
	currNode->startVal = startVal;

	currSpeed = malloc(sizeof(*(currSpeed)) * 4);
	currSpeed[0] = -127;
	currSpeed[1] = -127;
	currSpeed[2] = -127;
	currSpeed[3] = 0;
	currSensors = malloc(2 * sizeof(*(currSensors)));
	currSensors[0] = 120;
	currSensors[1] = 120;
	updateSpeeds(pairMap[2], currSpeed, currSensors);

	currNode = findParent(currNode);
	currNode = findParent(currNode);
	MotorSensorPair *testPair3 = &pairMap[1];
	addMoveNode(testPair3);
	startVal = malloc(sizeof(*(startVal)));
	startVal[0] = 20;
	currNode->startVal = startVal;

	currSpeed = malloc(sizeof(*(currSpeed)) * 6);
	currSpeed[0] = 127;
	currSpeed[1] = -127;
	currSpeed[2] = 127;
	currSpeed[3] = -127;
	currSpeed[4] = 127;
	currSpeed[5] = -127;
	currSensors = malloc(sizeof(*(currSensors)) * 2);
	currSensors[0] = 160;
	currSensors[1] = 160;
	updateSpeeds(pairMap[1], currSpeed, currSensors);
	/*free(currSpeed);
	free(currSensors);*/

	/*	for (int i = 0; i < 30; i++) {
		currNode = findParent(currNode);
		MotorSensorPair *testPair3 = &pairMap[1];
		addMoveNode(testPair3);
		startVal = malloc(sizeof(*(startVal)));
		startVal[0] = 20;
		currNode->startVal = startVal;

		currSpeed = malloc(sizeof(*(currSpeed)) * 6);
		currSpeed[0] = 127;
		currSpeed[1] = -127;
		currSpeed[2] = 127;
		currSpeed[3] = -127;
		currSpeed[4] = 127;
		currSpeed[5] = -127;
		currSensors = malloc(sizeof(*(currSensors)) * 2);
		currSensors[0] = 160;
		currSensors[1] = 160;
		updateSpeeds(pairMap[1], currSpeed, currSensors);
	}*/

	currSpeed = NULL;
	currSensors = NULL;
}

void runAutonReader(void *ignore) {
	// signal to other threads that
	// the reader is initializing
/*	readerInit = mutexCreate();
	if (!mutexTake(readerInit, -1)) {
		printError("Timing screwed up."); // this should never print
	}*/

	// debug battery voltage and remaining memory
	// printVoltage();

	print("hi");
	// getchar();
	// set up the required time sensor and other defined sensors
	MotorSensorPair *newPair = initNewPair();
	addSensor(newPair, TIME, 0);
	newPair = NULL;
	print("hi again");
	// getchar();
	// print("Load previous config? (y / n) ");
	initReplayer();
	print("all done");
	/*char answer = getchar();
	if (answer == 'n') {
		initSensorMap();
		printDebug("Started sensors!");

		// debug the registered info
		for (int i = 0; i < numPairs; i++) {
			printPair(&pairMap[i]);
		}
	} else {
		loadTreeFromFile("b");
	}*/
	// initTimers();

	printDebug("Started replayer!");
	// lcdSetBacklight(uart2, true);
	/*if (DEBUG) {
		lcdPrint(uart2, 1, "Started");
		while (lcdReadButtons(uart2) == 0) {
			delay(20);
		}
		lcdPrint(uart2, 1, "Driving");
	}*/

	// signal to other threads that they can start
	// mutexGive(readerInit);
	/*printNode(currNode);
	MotorSensorPair *testPair1 = &pairMap[1];
	addMoveNode(testPair1);
	printNode(currNode);
	MotorSensorPair *testPair2 = &pairMap[2];
	addMoveNode(testPair2);
	printNode(currNode);
	currNode = findParent(currNode);
	printNode(currNode);
	MotorSensorPair *testPair3 = &pairMap[1];
	addMoveNode(testPair3);
	printNode(currNode);
	currNode = findParent(currNode);
	printNode(currNode);*/

	// keep recording
	/*while (!outOfMemory) {
		// if (mutexTake(doNotRecord, 20)) {
			// printDebug("Waiting!");
			if (pairsLoaded) {
				recordData();
			}
			// mutexGive(doNotRecord);
		// }
		delay(20);*/
	//}

	printDebug("Out of memory! Stopping replayer...");
}
