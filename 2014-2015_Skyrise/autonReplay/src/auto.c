#include "main.h"
#include "RobotLib.h"

void autonomous() {
	// oldPath();9
	// initTimers();
	print("hey");
	// loadTreeFromFile("bn");
	runCurrentPath(rootNode->child, 0);
}

