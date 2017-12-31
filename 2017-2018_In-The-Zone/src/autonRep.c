#include "autonRep.h"
#include "variables.h"
#include "main.h"

void autonRep(){
  int A2, A3, U5, D5, U6, D6;
  FILE * fp;
  if(fileNumber == 1)
    fp = fopen("auton1.txt", "r");
  if(fileNumber == 2)
    fp = fopen("auton2.txt", "r");
  if(fileNumber == 3)
    fp = fopen("auton3.txt", "r");

  char line[15];

  int timeInMs = 0;
  bool repAuton = true;
  while (timeInMs <= 15000 && repAuton) {
    fread(line, sizeof(char), 15, fp);
    A2 = line[0]*100 + line[1]*10 + line[2];
    A3 = line[4]*100 + line[5]*10 + line[6];
    U5 = line[8];
    D5 = line[10];
    U6 = line[12];
    D6 = line[14];

    drive(A2, A3);
    lift(U6, D6);
    claw(U5, D5);

    delay(1);

    fseek(fp, 1, SEEK_CUR);
  }
}

//fscanf(fileptr, "%d %d %d %d",mt.analog[0], mt.analog[1], );
