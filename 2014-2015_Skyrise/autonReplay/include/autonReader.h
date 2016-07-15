/*
 * autonReader.h
 *
 *  Created on: Nov 7, 2014
 *      Author: Matthew
 */

#ifndef AUTONREADER_H_
#define AUTONREADER_H_

typedef struct SpeedPoint {
	signed char *motPower;
	short *endSensorVal;
} SpeedPoint;

typedef struct MotorSensorPair {
	char *pairStr;
	unsigned char *motorPorts;
	Sensor *sensors;
	float **pattern;
	unsigned char *basePattInds;
	short numPatterns;
	unsigned char pairId;
	unsigned char numSensors;
	unsigned char numPorts;
	bool running;
} MotorSensorPair;

typedef struct MoveNode {
	struct MoveNode *child;
	struct MoveNode *sibling;
	SpeedPoint *points;
	short *numRanges;
	short **patternStartVals;
	short **patternRangeLens;
	short *startVal;
	short startPoint;
	short numPoints;
	short nodeId;
	unsigned char nPairId;
	signed char prevPairId;
} MoveNode;

extern MotorSensorPair *pairMap;
extern int numPairs;

extern MoveNode *currNode;
extern MoveNode *rootNode;

extern Mutex readerInit;

extern unsigned char lastUniqueId;
extern const bool DEBUG;

extern int totalTreeSize;

void restartTree(void);
void freePairs(void);
void runAutonReader(void *ignore);

bool notAPatternIndex(int ind, MoveNode *node);
int getPattInd(int ind, MoveNode *node);

int getNodeSize(MoveNode* node);
int getPairSize(MotorSensorPair pair);

void printVoltage(void);
void printSize(void);
void printDebug(char *str);
void printError(char *str);
void printPair(MotorSensorPair *pair);
void printNode(MoveNode *node);
void printTree(void);
void createTestTree(void);

void initReplayer(void);
MotorSensorPair *initNewPair(void);
void addMotor(MotorSensorPair *pair, unsigned char motPort);
void addSensor(MotorSensorPair *pair, SensorType type, unsigned char port);

short *getAllSensorValues(MotorSensorPair *pair);

int measureAvailableMemory(void);

MoveNode *findParent(MoveNode *targetChild);

extern Encoder encoder;
extern Gyro gyro;

#endif /* AUTONREADER_H_ */
