#include "autonRep.h"
#include "variables.h"
#include "main.h"

void autonRep(){
  FILE * fp;
  if(fileNumber == 1)
    fp = fopen("auton1.txt", "r");
  if(fileNumber == 2)
    fp = fopen("auton2.txt", "r");
  if(fileNumber == 3)
    fp = fopen("auton3.txt", "r");

  int timeInMs = 0;
  bool repAuton = true;
  while (timeInMs <= 15000 && repAuton) {
    
  }
}

//fscanf(fileptr, "%d %d %d %d",mt.analog[0], mt.analog[1], );
