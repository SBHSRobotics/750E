/*
 * pathEditor.c
 *
 *  Created on: Dec 12, 2014
 *      Author: Matthew
 */

#include "main.h"

MoveNode *selectedNode, *firstNode;
MoveNode *startNode, *endNode;
int displayStartPoint, displayCurrPoint;
int maxId;
bool focused = false;

Mutex doNotDrive;
Mutex doNotMoveCursor;

bool redrawDisplay;
bool movedCursor;

const int X_POS = 3, Y_POS = 8;
const int WIDTH = 55;
const int HEIG = 15;

void gotoxy(int x, int y) {
	printf("%c[%d;%dH", (char)27, y, x); // no clue why the order is flipped
	fflush(stdout);
}

void clrscr() {
	printf("%c[2J", (char)27);
}

void clrline(int y, int* currX, int* currY) {
	gotoxy(0, y);
	for (int i = 0; i < X_POS + WIDTH; i++) {
		print(" ");
	}
	delay(50);
	gotoxy(*currX, *currY);
}

void clrrect(int stY, int heig, int* currX, int *currY) {
	for (int i = stY; i < stY + heig; i++) {
		gotoxy(0, i);
		for (int j = 0; j < X_POS + WIDTH; j++) {
			print(" ");
		}
		delay(50);
	}
	gotoxy(*currX, *currY);
}

int depth(MoveNode *node) {
	if (node->nodeId == rootNode->nodeId) {
		return 0;
	}

	int depth = 1;
	while (findParent(node)->nodeId != rootNode->nodeId) {
		depth++;
		node = findParent(node);
	}
	return depth;
}

MoveNode* findNodeWithId(MoveNode *node, int id) {
	if (node->nodeId == id) {
		return node;
	}

	if (node->sibling != NULL) {
		if (node->sibling->nodeId <= id) {
			return findNodeWithId(node->sibling, id);
		}
	}

	if (node->child != NULL) {
		return findNodeWithId(node->child, id);
	}

	return NULL;
}

void chooseFirstNode(int *cursorX, int *cursorY) {
	MoveNode *findNewFirstNode(MoveNode *targetNode, int idOffset) {
		if (targetNode->nodeId + idOffset < 0) {
			return rootNode;
		}

		if (targetNode->nodeId + idOffset > maxId) {
			return findNodeWithId(rootNode, maxId);
		}

		return findNodeWithId(rootNode, targetNode->nodeId + idOffset);
	}

	if (focused) {
		int initId = firstNode->nodeId;
		firstNode = selectedNode;
		if (firstNode->nodeId - initId != 0) {
			*cursorY -= firstNode->nodeId - initId;
			clrrect(Y_POS, Y_POS + HEIG, cursorX, cursorY);
			redrawDisplay = true;
		} else if ((displayCurrPoint - displayStartPoint) / (HEIG - 1) > 0) {
			*cursorY -= HEIG - 1;
			clrrect(Y_POS, Y_POS + HEIG, cursorX, cursorY);
			// int oldPoint = displayCurrPoint;
			while ((displayCurrPoint - displayStartPoint) / (HEIG - 1) > 0) {
				displayStartPoint += (HEIG - 1);
			}
			// displayCurrPoint = oldPoint;
			redrawDisplay = true;
			// print("hi");
		} else if (displayCurrPoint < displayStartPoint) {
			*cursorY += HEIG - 1;
			clrrect(Y_POS, Y_POS + HEIG, cursorX, cursorY);
			while (displayCurrPoint < displayStartPoint) {
				displayStartPoint -= HEIG - 1;
			}
			redrawDisplay = true;
			// print("bye");
		}
		// printf(" %d %d \n\r", displayCurrPoint, displayStartPoint);
	} else if (selectedNode->nodeId < firstNode->nodeId) {
		firstNode = findNewFirstNode(firstNode, -HEIG);
		if (selectedNode->nodeId < firstNode->nodeId) {
			selectedNode = firstNode;
			// *cursorY += 1;
		}
		*cursorY += selectedNode->nodeId - firstNode->nodeId + 1;
		// selectedNode = findNodeWithId(rootNode, firstNode->nodeId + HEIG - 1);

		clrrect(Y_POS, Y_POS + HEIG, cursorX, cursorY);
		redrawDisplay = true;
	} else if (selectedNode->nodeId >= firstNode->nodeId + HEIG){
		firstNode = findNewFirstNode(firstNode, HEIG);
		*cursorY -= HEIG - (selectedNode->nodeId - firstNode->nodeId);
		clrrect(Y_POS, Y_POS + HEIG, cursorX, cursorY);
		redrawDisplay = true;
	}
}

void displayTree() {
	void ptabs(int nTabs) {
		for (int i = 0; i < X_POS + nTabs; i++) {
			print(" ");
		}
	}

	void writeNode(MoveNode *mn, int nTabs, int numLines, int startId) {
		// getchar();
		// maxId = mn->nodeId;

		// getchar();
		if (mn->nodeId >= startId) {
			if (nTabs != 0) {
				// print tabs first
				ptabs(nTabs);

				if (mn->sibling != NULL) {
					print("|");
				} else {
					print("\\");
				}
				print("->");
			}

			char *pairName = malloc(20 * sizeof(char));
			if (pairName != NULL) {
				if (pairMap[mn->nPairId].pairStr == NULL) {
					sprintf(pairName, "pair %d", pairMap[mn->nPairId].pairId);
				} else {
					pairName = pairMap[mn->nPairId].pairStr;
				}

				printf("Move %s (ID: %d)", pairName, mn->nodeId);
				if (startNode != NULL && mn->nodeId == startNode->nodeId) {
					print(" **Start**");
				} else if (endNode != NULL && mn->nodeId == endNode->nodeId) {
					print(" **Stop**");
				}
				print("\n\r");
				if (++numLines >= HEIG) {
					return;
				}

				if (focused && selectedNode->nodeId == mn->nodeId) {
					if (displayStartPoint == 0) {
						ptabs(nTabs + 3);
						print("start val: ");
						for (int i = 0; i < pairMap[selectedNode->prevPairId].numSensors; i++) {
							printf("%d", selectedNode->startVal[i]);
							if (i != pairMap[selectedNode->prevPairId].numSensors - 1) {
								print(", ");
							} else {
								print("\n\r");
							}
						}
						if (++numLines >= HEIG) {
							return;
						}
					}
					delay(5);
					int startInd = (displayStartPoint == 0 ? 0 : displayStartPoint - 1);
					for (int j = startInd; j < selectedNode->numPoints; j++) {
						ptabs(nTabs + 3);
						if (notAPatternIndex(j, selectedNode)) {
							for (int i = 0; i < pairMap[selectedNode->nPairId].numPorts; i++) {
								printf("%d", selectedNode->points[j].motPower[i]);
								if (i == pairMap[selectedNode->nPairId].numPorts - 1) {
									print("; ");
								} else {
									print(", ");
								}
							}
						} else {
							int patInd = getPattInd(j, selectedNode);
							if (pairMap[selectedNode->nPairId].numPorts > 1) {
								printf("%d (", selectedNode->points[j].motPower[0]);
								for (int i = 0; i < pairMap[selectedNode->nPairId].numPorts; i++) {
									printf("%d", (int)(selectedNode->points[j].motPower[0] * pairMap[selectedNode->nPairId].pattern[patInd][i]));
									if (i == pairMap[selectedNode->nPairId].numPorts - 1) {
										print("); ");
									} else {
										print(", ");
									}
								}
							} else {
								printf("%d; ", selectedNode->points[j].motPower[0]);
							}
						}
						delay(5);

						for (int i = 0; i < pairMap[selectedNode->nPairId].numSensors; i++) {
							printf("%d", selectedNode->points[j].endSensorVal[i]);
							if (i == pairMap[selectedNode->nPairId].numSensors - 1) {
								print("\n\r");
							} else {
								print(", ");
							}
						}
						delay(5);
						if (++numLines >= HEIG) {
							return;
						}
					}
				}
				if (pairMap[mn->nPairId].pairStr == NULL) {
					free(pairName);
				}
				fflush(stdout);
				delay(100);
			} else {
				printError("Out of memory!");
				return;
			}
		}

		if (mn->child != NULL) {
			writeNode(mn->child, nTabs + 1, numLines, startId);
		}

		if (mn->sibling != NULL) {
			writeNode(mn->sibling, nTabs, numLines, startId);
		}
	}

	gotoxy(X_POS, Y_POS);
	for (int i = 0; i < WIDTH / 2 - 1; i++) {
		print(" ");
	}
	if (firstNode->nodeId > rootNode->nodeId) {
		print("/\\");
	} else {
		print("  ");
	}
	for (int i = WIDTH / 2 + 1; i < WIDTH; i++) {
		print(" ");
	}
	gotoxy(X_POS, Y_POS + 1);
	for (int i = 0; i < WIDTH; i++) {
		print("=");
	}
	// chooseFirstNode();
	gotoxy(0, Y_POS + 2);
	writeNode(rootNode, 0, 0, firstNode->nodeId);
	gotoxy(X_POS, Y_POS + HEIG + 2);
	for (int i = 0; i < WIDTH; i++) {
		print("=");
	}
	gotoxy(X_POS, Y_POS + HEIG + 3);
	for (int i = 0; i < WIDTH / 2 - 1; i++) {
		print(" ");
	}
	if (firstNode->nodeId + HEIG < maxId) {
		print("\\/");
	} else {
		print("  ");
	}
	for (int i = WIDTH / 2 + 1; i < WIDTH; i++) {
		print(" ");
	}
}

int numChildren(MoveNode *target) {
	int nChildren = 0;
	if (target->child != NULL) {
		MoveNode *child = target->child;
		do {
			nChildren++;
			// child = target->child;
			nChildren += numChildren(child);
			child = child->sibling;
		} while (child != NULL);
		/*numChildren++;
		MoveNode *child = target->child;
		numChildren += numChildren(child);
		while (child->sibling != NULL) {
			numChildren++;
			child = child->sibling;
		}*/
	}
	return nChildren;
}

int selfIndex(MoveNode *target) {
	int ind = 0;
	MoveNode *startNode = findParent(target)->child;
	if (startNode->nodeId != target->nodeId) {
		ind++;
		while (startNode->sibling->nodeId != target->nodeId) {
			ind++;
			startNode = startNode->sibling;
		}
	}
	return ind;
}

MoveNode *findPrevSibling(MoveNode *target) {
	MoveNode *innerFindSibling(MoveNode *curr, int id) {
		if (curr->sibling != NULL) {
			if (curr->sibling->nodeId == id) {
				return curr;
			}

			if (curr->sibling->nodeId <= id) {
				return innerFindSibling(curr->sibling, id);
			}
		}

		if (curr->child != NULL) {
			return innerFindSibling(curr->child, id);
		}

		return NULL;
	}

	return innerFindSibling(rootNode, target->nodeId);
}

int getNewVal(int *cX, int *cY) {
	gotoxy(0, 6);
	print("Enter new value: ");
	bool negative = false;
	int retVal = 0;
	int currLen = 0;
	while (true) {
		char currChar = getchar();
		if (currChar == 13) { // enter
			break;
		} else if (currChar == 8 && currLen > 0) { // backspace
			if (retVal != 0) {
				retVal = (retVal - retVal % 10) / 10;
			} else {
				negative = false;
			}
			currLen--;
			print("\b \b");
		} else if (abs(retVal) < 1000) {
			if (retVal == 0 && currChar == '-') {
				negative = true;
				currLen++;
				print("-");
			}
			int charNumValue = (int)currChar - 48;
			if (charNumValue >= 0 && charNumValue <= 9) {
				retVal = retVal * 10 + (charNumValue); // '0' = 48
				currLen++;
				printf("%d", charNumValue);
			}
		}
	}
	gotoxy(0, 6);
	print("                             ");

	gotoxy(*cX, *cY);
	if (negative) {
		return -retVal;
	}
	return retVal;
}

int getNumWidth(short num) {
	int width = 1;
	while (abs(num) >= 10) {
		num /= 10;
		width++;
	}
	return width + (num < 0 ? 1 : 0); // take care of that negative sign
}

void copyNodesToEnd(MoveNode *startNode, MoveNode *endNode, MoveNode *destNode, bool isSibling) {
	void attachNewNode(MoveNode *node, int endId, MoveNode *destNode, MoveNode *extraNode, bool isSibling) {
		// printf("Attaching node %d\n\r", node->nodeId);
		/*getchar();*/
		if (node->nodeId > endId) {
			return;
		}

		MoveNode *newNode = malloc(sizeof(*(newNode)));
		newNode->startPoint = node->startPoint;
		newNode->numPoints = node->numPoints;
		newNode->nPairId = node->nPairId;
		newNode->prevPairId = node->prevPairId;
		newNode->points = node->points;
		newNode->numRanges = node->numRanges;
		newNode->patternStartVals = node->patternStartVals;
		newNode->patternRangeLens = node->patternRangeLens;
		for (int i = 0; i < pairMap[newNode->nPairId].numPatterns; i++) {
			newNode->patternStartVals[i] = node->patternStartVals[i];
			newNode->patternRangeLens[i] = node->patternRangeLens[i];
		}
		newNode->child = NULL;
		newNode->sibling = NULL;

		MotorSensorPair pair = pairMap[newNode->prevPairId];
		if (isSibling) {
			destNode->sibling = newNode;
			MotorSensorPair parentPair = pairMap[destNode->sibling->prevPairId];
			if (pair.numSensors == parentPair.numSensors) {
				newNode->startVal = node->startVal;
			} else {
				newNode->startVal = malloc(parentPair.numSensors * sizeof(*(newNode->startVal)));
				for (int i = 0; i < parentPair.numSensors; i++) {
					newNode->startVal[i] = 0;
				}
			}
			newNode->nodeId = destNode->nodeId + numChildren(destNode) + 1;
		} else {
			destNode->child = newNode;
			MotorSensorPair parentPair = pairMap[destNode->child->prevPairId];
			if (pair.numSensors == parentPair.numSensors) {
				newNode->startVal = node->startVal;
			} else {
				newNode->startVal = malloc(parentPair.numSensors * sizeof(*(newNode->startVal)));
				for (int i = 0; i < parentPair.numSensors; i++) {
					newNode->startVal[i] = 0;
				}
			}
			newNode->nodeId = destNode->nodeId + 1;
		}

		// printf("DEBUG: Depth - %d %d\n\r", depth(node), depth(finalNode));
		if (node->nodeId == endId) {
			// print("we're done");
			if (extraNode != NULL && depth(node) == depth(extraNode)) {
				// print("Added new sibling!");
				newNode->sibling = extraNode;
			}
			// return;
		}

		print("hi");
		if (node->child != NULL) {
			attachNewNode(node->child, endId, newNode, extraNode, false);
		}

		if (node->sibling != NULL) {
			attachNewNode(node->sibling, endId, newNode, extraNode, true);
		}
	}

	if (endNode->nodeId < startNode->nodeId) {
		MoveNode *temp = startNode;
		startNode = endNode;
		endNode = temp;
	}

	/*print("Starting to copy!");
	getchar();*/
	attachNewNode(startNode, endNode->nodeId, destNode, destNode->sibling, isSibling);
	// print("Done copying.");
}

void correctIDs() {
	void correctNodeID(MoveNode *node, int correctID) {
		// printf("%d\n\r", node->nPairId);
		// printf("Corrected %d to %d\n\r", node->nodeId, correctID);
		// getchar();
		if (node->nodeId != correctID) {
			node->nodeId = correctID;
		}

		if (node->child != NULL) {
			correctNodeID(node->child, correctID + 1);
		}

		if (node->sibling != NULL) {
			correctNodeID(node->sibling, correctID + 1 + numChildren(node));
		}
	}

	if (rootNode->child != NULL) {
		correctNodeID(rootNode->child, 1);
	}
}

void deleteNode(MoveNode *delNode) {
	free(delNode->startVal);			// free variable array of start values
	for (int i = 0; i < pairMap[delNode->nPairId].numPatterns; i++) {
		if (delNode->numRanges[i] != 0) {
			free(delNode->patternStartVals[i]);
			free(delNode->patternRangeLens[i]);
		}
	}
	free(delNode->numRanges);
	for (int i = 0; i < delNode->numPoints; i++) {
		free(delNode->points[i].motPower);
		free(delNode->points[i].endSensorVal);
	}
	free(delNode->points);
	free(delNode);					// free all stored memory related to the node
}

void handleInput(int *cursorX, int *cursorY) {
	static int pointX = -1, pointY = -1;
	static MoveNode *copyStartNode = NULL;
	static MoveNode *copyEndNode = NULL;
	static MoveNode *copyDestNode = NULL;
	static int moveMode = -1;
	static int startMode = -1;
	// static int storedX = -1;

	// print("waiting for key press");
	char c = getchar();
	switch (c) {
	case 'w':	// sibling traversal OR point traversal
		if (focused) {
			if (pointY == -1) {
				pointY = selectedNode->numPoints - 1;
				*cursorY += selectedNode->numPoints;
				int totalMove = 0;
				for (int i = 0; i < pointX; i++) {
					totalMove += getNumWidth(selectedNode->startVal[i]) + 2;
				}
				*cursorX -= 11 + totalMove;
			} else {
				int totalMove = 0;
				MotorSensorPair pair = pairMap[selectedNode->nPairId];
				if (notAPatternIndex(pointY, selectedNode)) {
					for (int i = 0; i < pointX; i++) {
						if (i >= pair.numPorts) {
							totalMove += getNumWidth(selectedNode->points[pointY].endSensorVal[i - pair.numPorts]) + 2;
						} else {
							totalMove += getNumWidth(selectedNode->points[pointY].motPower[i]) + 2;
						}
					}
				} else if (pointX > 0) {
					for (int i = 0; i < pointX - 1; i++) {
						totalMove += getNumWidth(selectedNode->points[pointY].endSensorVal[i]) + 2;
					}
					int patInd = getPattInd(0, selectedNode);
					totalMove += 3 + getNumWidth(selectedNode->points[pointY].motPower[0]);
					for (int i = 0; i < pair.numPorts; i++) {
						totalMove += getNumWidth(
								(short)(selectedNode->points[pointY].motPower[0] * pair.pattern[patInd][i])) + 2;
					}
				}
				pointY -= 1;
				*cursorY -= 1;
				if (pointY == -1) {
					*cursorX -= totalMove - 11;
				} else {
					*cursorX -= totalMove;
				}
			}
			pointX = 0;
			displayCurrPoint = pointY + 1;
		} else {
			if (selectedNode->nodeId > findParent(selectedNode)->child->nodeId) {
				selectedNode = findPrevSibling(selectedNode);
				*cursorY -= (1 + numChildren(selectedNode));
			}
		}
		chooseFirstNode(cursorX, cursorY);
		break;
	case 's':
		if (focused) {
			if (pointY == selectedNode->numPoints - 1) {
				pointY = -1;
				*cursorY -= selectedNode->numPoints;
				int totalMove = 0;
				MotorSensorPair pair = pairMap[selectedNode->nPairId];
				if (notAPatternIndex(selectedNode->numPoints - 1, selectedNode)) {
					for (int i = 0; i < pointX; i++) {
						if (i >= pair.numPorts) {
							totalMove += getNumWidth(selectedNode->points[selectedNode->numPoints - 1].endSensorVal[i - pair.numPorts]) + 2;
						} else {
							totalMove += getNumWidth(selectedNode->points[selectedNode->numPoints - 1].motPower[i]) + 2;
						}
					}
				} else if (pointX > 0) {
					for (int i = 0; i < pointX - 1; i++) {
						totalMove += getNumWidth(selectedNode->points[selectedNode->numPoints - 1].endSensorVal[i]) + 2;
					}
					int patInd = getPattInd(0, selectedNode);
					totalMove += 3 + getNumWidth(selectedNode->points[selectedNode->numPoints - 1].motPower[0]);
					for (int i = 0; i < pair.numPorts; i++) {
						totalMove += getNumWidth(
								(short)(selectedNode->points[selectedNode->numPoints - 1].motPower[0] * pair.pattern[patInd][i])) + 2;
					}
				}
				*cursorX += 11 - totalMove;
				pointX = 0;
			} else {
				int totalMove = 0;
				if (pointY == -1) {
					*cursorX -= 11;
					for (int i = 0; i < pointX; i++) {
						totalMove += getNumWidth(selectedNode->startVal[i]) + 2;
					}
				} else {
					MotorSensorPair pair = pairMap[selectedNode->nPairId];
					if (notAPatternIndex(pointY, selectedNode)) {
						for (int i = 0; i < pointX; i++) {
							if (i >= pair.numPorts) {
								totalMove += getNumWidth(selectedNode->points[pointY].endSensorVal[i - pair.numPorts]) + 2;
							} else {
								totalMove += getNumWidth(selectedNode->points[pointY].motPower[i]) + 2;
							}
						}
					} else if (pointX > 0) {
						for (int i = 0; i < pointX - 1; i++) {
							totalMove += getNumWidth(selectedNode->points[pointY].endSensorVal[i]) + 2;
						}
						int patInd = getPattInd(0, selectedNode);
						totalMove += 3 + getNumWidth(selectedNode->points[pointY].motPower[0]);
						for (int i = 0; i < pair.numPorts; i++) {
							totalMove += getNumWidth(
									(short)(selectedNode->points[pointY].motPower[0] * pair.pattern[patInd][i])) + 2;
						}
					}
				}
				pointY += 1;
				*cursorY += 1;
				pointX = 0;
				*cursorX -= totalMove;
			}
			displayCurrPoint = pointY + 1;
		} else {
			if (selectedNode->sibling != NULL) {
				int extra = numChildren(selectedNode);
				selectedNode = selectedNode->sibling;
				*cursorY += 1 + extra;
			}
		}
		chooseFirstNode(cursorX, cursorY);
		break;
	case 'a':  // child traversal OR point traversal
		if (focused) {
			if (pointY == -1) {
				if (pointX == 0) {
					focused = false;
					clrrect(*cursorY, HEIG - (*cursorY - Y_POS - 2), cursorX, cursorY);
					*cursorX -= 11;
					*cursorY -= 1;
					delay(20);
					redrawDisplay = true;
				} else {
					pointX--;
					*cursorX -= getNumWidth(selectedNode->startVal[pointX]) + 2;
				}
			} else {
				MotorSensorPair pair = pairMap[selectedNode->nPairId];
				if (notAPatternIndex(pointY, selectedNode)) {
					if (pointX == 0) {
						pointX = pair.numPorts + pair.numSensors - 1;
						int totalMove = 0;
						for (int i = 0; i < pair.numPorts; i++) {
							totalMove += getNumWidth(selectedNode->points[pointY].motPower[i]) + 2;
						}
						for (int i = 0; i < pair.numSensors - 1; i++) {
							totalMove += getNumWidth(selectedNode->points[pointY].endSensorVal[i]) + 2;
						}
						*cursorX += totalMove;
					} else if (pointX < pair.numPorts) {
						pointX--;
						*cursorX -= getNumWidth(selectedNode->points[pointY].motPower[pointX]) + 2;
					} else {
						pointX--;
						*cursorX -= getNumWidth(selectedNode->points[pointY].endSensorVal[pointX - pair.numPorts - 1]) + 2;
					}
				} else {
					int totalMove = 0;
					if (pointX == 0 || pointX == 1) {
						int patInd = getPattInd(pointY, selectedNode);
						totalMove = 3 + getNumWidth(selectedNode->points[pointY].motPower[0]);
						for (int i = 0; i < pair.numPorts; i++) {
							totalMove += getNumWidth(
									(short)(selectedNode->points[pointY].motPower[0] * pair.pattern[patInd][i])) + 2;
						}
						if (pointX == 0) {
							for (int i = 0; i < pair.numSensors - 1; i++) {
								totalMove += getNumWidth(selectedNode->points[pointY].endSensorVal[i]) + 2;
							}
						}
					} else {
						totalMove += getNumWidth(selectedNode->points[pointY].endSensorVal[pointX - 2]) + 2;
					}
					if (pointX != 0) {
						pointX--;
						*cursorX -= totalMove;
					} else {
						pointX = pair.numSensors;
						*cursorX += totalMove;
					}
				}
			}
		} else {
			if (findParent(selectedNode) != NULL && findParent(selectedNode) != rootNode) {
				int extra = selfIndex(selectedNode);
				selectedNode = findParent(selectedNode);
				*cursorY -= (1 + extra);
				*cursorX -= 1;
			}
		}
		chooseFirstNode(cursorX, cursorY);
		break;
	case 'd':
		if (focused) {
			if (pointY == -1) {
				if (pointX == pairMap[selectedNode->prevPairId].numSensors - 1) {
					int totalMove = 0;
					for (int i = 0; i < pairMap[selectedNode->prevPairId].numSensors - 1; i++) {
						totalMove += getNumWidth(selectedNode->startVal[i]) + 2;
					}
					*cursorX -= totalMove;
					pointX = 0;
				} else {
					*cursorX += getNumWidth(selectedNode->startVal[pointX]) + 2;
					pointX++;
				}
			} else {
				MotorSensorPair pair = pairMap[selectedNode->nPairId];
				if (notAPatternIndex(pointY, selectedNode)) {
					if (pointX == pair.numPorts + pair.numSensors - 1) {
						pointX = 0;
						int totalMove = 0;
						for (int i = 0; i < pair.numPorts; i++) {
							totalMove += getNumWidth(selectedNode->points[pointY].motPower[i]) + 2;
						}
						for (int i = 0; i < pair.numSensors - 1; i++) {
							totalMove += getNumWidth(selectedNode->points[pointY].endSensorVal[i]) + 2;
						}
						*cursorX -= totalMove;
					} else if (pointX < pair.numPorts) {
						*cursorX += getNumWidth(selectedNode->points[pointY].motPower[pointX]) + 2;
						pointX++;
					} else {
						*cursorX += getNumWidth(selectedNode->points[pointY].endSensorVal[pointX - pair.numPorts]) + 2;
						pointX++;
					}
				} else {
					int patInd = getPattInd(pointY, selectedNode);
					int totalMove = 0;
					if (pointX == 0 || pointX == pair.numSensors) {
						totalMove = 3 + getNumWidth(selectedNode->points[pointY].motPower[0]);
						for (int i = 0; i < pair.numPorts; i++) {
							totalMove += getNumWidth(
									(short)(selectedNode->points[pointY].motPower[0] * pair.pattern[patInd][i])) + 2;
						}
						if (pointX == pair.numSensors) {
							for (int i = 0; i < pair.numSensors - 1; i++) {
								totalMove += getNumWidth(selectedNode->points[pointY].endSensorVal[i]) + 2;
							}
						}
					} else {
						totalMove += getNumWidth(selectedNode->points[pointY].endSensorVal[pointX - 1]) + 2;
					}
					if (pointX != pair.numSensors) {
						pointX++;
						*cursorX += totalMove;
					} else {
						pointX = 0;
						*cursorX -= totalMove;
					}
				}
			}
		} else {
			if (selectedNode->child != NULL) {
				selectedNode = selectedNode->child;
				*cursorY += 1;
				*cursorX += 1;
			}
		}
		chooseFirstNode(cursorX, cursorY);
		break;
	case 13:  // Enter key, do something with it
		if (startMode >= 0) { // copying stuff
			switch (startMode) {
			case 0:
				startNode = selectedNode;
				gotoxy(0, 6);
				print("Select new end node.    ");
				gotoxy(*cursorX, *cursorY);
				startMode++;
				break;
			case 1:
				endNode = selectedNode;
				gotoxy(0, 6);
				print("                        ");
				gotoxy(*cursorX, *cursorY);
				clrrect(Y_POS + 1, HEIG + 2, cursorX, cursorY);
				redrawDisplay = true;
				startMode = -1;
				break;
			}
			break;
		}

		if (moveMode >= 0) { // copying stuff
			switch (moveMode) {
			case 0:
				copyStartNode = selectedNode;
				gotoxy(0, 6);
				print("Select end node to copy.    ");
				gotoxy(*cursorX, *cursorY);
				moveMode++;
				break;
			case 1:
				copyEndNode = selectedNode;
				gotoxy(0, 6);
				print("Select destination node.    ");
				gotoxy(*cursorX, *cursorY);
				moveMode++;
				break;
			case 2:
				copyDestNode = selectedNode;
				int oldX = *cursorX, oldY = *cursorY;
				*cursorX = 0;
				*cursorY = 6;
				gotoxy(*cursorX, *cursorY);
				print("Sibling of node (y/n)?   ");
				char currChar = 'a';
				while (currChar != 'y' && currChar != 'n') {
					currChar = getchar();
				}
				gotoxy(0, 6);
				print("                         ");
				*cursorX = oldX;
				*cursorY = oldY;
				gotoxy(*cursorX, *cursorY);
				if (currChar == 'y') {
					copyNodesToEnd(copyStartNode, copyEndNode, copyDestNode, true);
				} else {
					copyNodesToEnd(copyStartNode, copyEndNode, copyDestNode, false);
				}
				correctIDs();
				printSize();		// TODO: actually update size and correct IDs
				delay(50);
				clrrect(Y_POS + 2, HEIG, cursorX, cursorY);
				redrawDisplay = true;
				moveMode = -1;
				copyStartNode = NULL;
				copyEndNode = NULL;
				copyDestNode = NULL;
				break;
			}
			break;
		}

		if (!focused) {
			focused = true;
			displayStartPoint = 0;
			displayCurrPoint = displayStartPoint;
			pointX = 0;
			pointY = -1;
			*cursorX += 11;
			*cursorY += 1;
			clrrect(*cursorY, HEIG - *cursorY, cursorX, cursorY);
			chooseFirstNode(cursorX, cursorY);
		} else {
			// edit value
			if (pointY == -1) {
				selectedNode->startVal[pointX] = getNewVal(cursorX, cursorY);
			} else {
				if (notAPatternIndex(pointY, selectedNode)) {
					if (pointX >= pairMap[selectedNode->nPairId].numPorts) {
						selectedNode->points[pointY].endSensorVal[pointX - pairMap[selectedNode->nPairId].numPorts] = getNewVal(cursorX, cursorY);
					} else {
						selectedNode->points[pointY].motPower[pointX] = getNewVal(cursorX, cursorY);
					}
				} else {
					if (pointX == 0) {
						selectedNode->points[pointY].motPower[0] = getNewVal(cursorX, cursorY);
					} else {
						selectedNode->points[pointY].endSensorVal[pointX - 1] = getNewVal(cursorX, cursorY);
					}
				}
			}
			clrline(*cursorY, cursorX, cursorY);
		}
		redrawDisplay = true;
		break;
	case 8:  // backspace
		if (!focused) { // TODO: Buggy, bad algorithm (or i'm just high)
			// deleting a node
			MoveNode *newSelect = NULL;
			MotorSensorPair parentPair = pairMap[findParent(selectedNode)->nPairId];
			if (selectedNode->child != NULL) {
				newSelect = selectedNode->child;
				clrrect(*cursorY, HEIG - *cursorY, cursorX, cursorY);
				MotorSensorPair selectPair = pairMap[selectedNode->nPairId];
				MotorSensorPair childPair = pairMap[selectedNode->child->nPairId];
				if (findPrevSibling(selectedNode) != NULL) {
					findPrevSibling(selectedNode)->sibling = selectedNode->child;
				} else {
					findParent(selectedNode)->child = selectedNode->child;
				}
				if (parentPair.numSensors != selectPair.numSensors) {
					selectedNode->child->startVal = malloc(parentPair.numSensors * sizeof(*(selectedNode->child->startVal)));
				}
				for (int i = 0; i < parentPair.numSensors; i++) {
					selectedNode->child->startVal[i] = 0;
				}
				selectedNode->child->startPoint = 0;
				if (selectedNode->child->sibling != NULL) {
					if (selectedNode->sibling != NULL) {
						selectedNode->child->sibling = selectedNode->sibling;
						if (childPair.numSensors != parentPair.numSensors) {
							selectedNode->sibling->startVal = malloc(childPair.numSensors * sizeof(*(selectedNode->child->startVal)));
						}
						for (int i = 0; i < childPair.numSensors; i++) {
							selectedNode->sibling->startVal[i] = 0;
						}
						selectedNode->sibling->startPoint = 0;
					}
					deleteNode(selectedNode);
					selectedNode = NULL;
				}
			} else if (selectedNode->sibling != NULL) {
				newSelect = selectedNode->sibling;
				clrrect(*cursorY, HEIG - *cursorY, cursorX, cursorY);
				if (findPrevSibling(selectedNode) != NULL) {
					// findParent(selectedNode)->child = findPrevSibling(selectedNode);
					findPrevSibling(selectedNode)->sibling = selectedNode->sibling;
				} else {
					findParent(selectedNode)->child = selectedNode->sibling;
				}
				for (int i = 0; i < parentPair.numSensors; i++) {
					selectedNode->sibling->startVal[i] = 0;
				}
				selectedNode->sibling->startPoint = 0;
				deleteNode(selectedNode);
				selectedNode = NULL;
			} else if (findPrevSibling(selectedNode) != NULL) {
				newSelect = findPrevSibling(selectedNode);
				*cursorY -= 1 + numChildren(newSelect);
				clrrect(*cursorY, HEIG - *cursorY, cursorX, cursorY);
				findPrevSibling(selectedNode)->sibling = NULL;
				deleteNode(selectedNode);
				selectedNode = NULL;
			} else {
				newSelect = findParent(selectedNode);
				*cursorX -= 1;
				*cursorY -= 1 + selfIndex(selectedNode);
				clrrect(*cursorY, HEIG - *cursorY, cursorX, cursorY);
				findParent(selectedNode)->child = NULL;
				deleteNode(selectedNode);
				selectedNode = NULL;
			}
			correctIDs();
			selectedNode = newSelect;
		} else if (pointY >= 0) {
			// deleting a point
			// redrawDisplay = true;
		}
		redrawDisplay = true;
		break;
	case 'c': // start copy sequence
		if (moveMode == -1 && startMode == -1 && !focused) {
			gotoxy(0, 6);
			print("Select start node to copy.");
			gotoxy(*cursorX, *cursorY);
			moveMode = 0;
		}
		break;
	case 'p':
		if (moveMode == -1 && startMode == -1 && !focused) {
			gotoxy(0, 6);
			print("Select new start node.");
			gotoxy(*cursorX, *cursorY);
			startMode = 0;
		}
		break;
	case 'r': // run the path
		gotoxy(0, 6);
		print("Running current path.");
		// getchar();
		mutexTake(doNotDrive, -1);
		if (endNode != NULL) {
			runCurrentPath(startNode, endNode->nodeId);
		} else {
			runCurrentPath(startNode, -1);
		}
		mutexGive(doNotDrive);
		gotoxy(0, 6);
		print("                     ");
		gotoxy(*cursorX, *cursorY);
		break;
	case 'e':
		writeTreeToFile("fml.dat");
		/*getchar();
		createCodeFile();*/
		break;
	case 'l':
		// print("start load");
		// DEBUG = true;
		loadTreeFromFile("b");
		if (rootNode->child != NULL) {
			startNode = rootNode->child;
		}
		endNode = NULL;
		redrawDisplay = true;
		// DEBUG = false;
		break;
	case 'o':
		loadTreeFromStart("b");
		if (rootNode->child != NULL) {
			startNode = rootNode->child;
		}
		endNode = NULL;
		redrawDisplay = true;
		break;
	case 'b':
		createCodeFile();
		gotoxy(0, 6);
		print("Created code file.");
		delay(1000);
		gotoxy(0, 6);
		print("                  ");
		gotoxy(*cursorX, *cursorY);
		break;
	case 'f':
		fflush(stdout);
		break;
	case 'x':
		if (!focused) {
			clrrect(Y_POS + 1, HEIG + 5, cursorX, cursorY);
			restartTree();
			redrawDisplay = true;
		}
		break;
	default:
		break;
	}
}

int *currX;
int *currY;

void updater(void *ignore) {
	currX = malloc(sizeof(int));
	*currX = X_POS + 5;
	currY = malloc(sizeof(int));
	*currY = Y_POS + 3;

	startNode = NULL;
	endNode = NULL;

	while (true) {
		mutexTake(doNotMoveCursor, -1);
		gotoxy(*currX, *currY);
		mutexGive(doNotMoveCursor);
		if (rootNode->child == NULL || selectedNode == rootNode) {
			/*if (rootNode->child == NULL) {
				print("Press 'l' to load, or just drive.\n\r");
			}*/
			while (rootNode->child == NULL) {
				char c = getchar();
				if (c == 'l') {
					loadTreeFromStart("b");
				}
				delay(20);
			}
			// gotoxy(X_POS, Y_POS);
			// print("                        ");
			selectedNode = rootNode->child;
			startNode = rootNode->child;
			*currX = X_POS + 5;
			*currY = Y_POS + 3;
			gotoxy(*currX, *currY);
		}
		handleInput(currX, currY);
		delay(20);
	}
}

void runEditor(void *ignore) {
	static int lastSize = 0;

	redrawDisplay = false;

	doNotMoveCursor = mutexCreate();

	clrscr();
	gotoxy(0, 0);
	print("Auton Replayer Editor");
	printVoltage();
	printSize();
	maxId = rootNode->nodeId;
	firstNode = rootNode;
	selectedNode = rootNode;
	displayTree();
	taskCreate(updater, 3 * TASK_DEFAULT_STACK_SIZE / 2, NULL, TASK_PRIORITY_DEFAULT);
	int timer = 0;
	while (true) {
		maxId = lastUniqueId - 1;
		if (timer >= 2000) {
			if (totalTreeSize != lastSize) {
				printVoltage();
				delay(50);
				printSize();
				delay(50);
				fflush(stdout);
				lastSize = totalTreeSize;
			}
			timer = 0;
		}

		if (movedCursor) {
			mutexTake(doNotMoveCursor, -1);
			gotoxy(*currX, *currY);
			mutexGive(doNotMoveCursor);
			// movedCursor = false;
		}

		if (redrawDisplay) {
			mutexTake(doNotMoveCursor, -1);
			displayTree();
			redrawDisplay = false;
			mutexGive(doNotMoveCursor);
		}

		if (movedCursor) {
			mutexTake(doNotMoveCursor, -1);
			gotoxy(*currX, *currY);
			mutexGive(doNotMoveCursor);
			movedCursor = false;
		}

		timer += 20;
		delay(20);
	}
}
