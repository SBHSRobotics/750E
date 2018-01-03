#include "autonRec.h"
#include "variables.h"
#include "main.h"

void recordAuton(){

  //moveFunctions = taskCreate(moveFunctions, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT);
  printf("TEST");
  FILE * fp;
  if(fileNumber == 1)
    fp = fopen("auton1.txt", "w");
  if(fileNumber == 2)
    fp = fopen("auton2.txt", "w");
  if(fileNumber == 3)
    fp = fopen("auton3.txt", "w");

  int timeInMs = 0;
  bool recAuton = true;

  /*while(timeInMs <= 15000 && recAuton){
    if(joystickGetDigital(1, 7, JOY_LEFT))
      recAuton = false;

    int A2, A3, U5, D5, U6, D6;
    int THRESH = 20;

    if((abs)(joystickGetAnalog(1, 2)) > THRESH) {
      A2 = joystickGetAnalog(1, 2);
    }
    else A2 = 0;
    if((abs)(joystickGetAnalog(1, 3)) > THRESH) {
      A3 = joystickGetAnalog(1, 3);
    }
    else A3 = 0;
    D5 = joystickGetDigital(1, 5, JOY_DOWN);
    U5 = joystickGetDigital(1, 5, JOY_UP);
    D6 = joystickGetDigital(1, 6, JOY_DOWN);
    U6 = joystickGetDigital(1, 6, JOY_UP);

    printf("TEST1");
    drive(A2, A3);

    lift(U6, D6);

    claw(U5, D5);*/

    /*printf("TEST2");
    int temp = A2;
    while(temp < 100) {
      temp *= 10;
      fprintf(fp, "0");
      printf("0");
    }
    fprintf(fp, "%d", A2);
    printf("%d", A2);

    temp = A3;
    while(temp < 100) {
      temp *= 10;
      fprintf(fp, "0");
      printf("0");
    }
    fprintf(fp, "%d", A3);
    printf("%d", A3);

    fprintf(fp, "%d %d %d %d\n", U5, D5, U6, D6);
    printf("%d %d %d %d\n",  U5, D5, U6, D6);
    printf("TEST3");
    delay(1);
    printf("TEST4");*/
  //}

  //taskDelete(moveFunctions);
}


//fprintf(fileptr, "%d %d %d %d", analog[0]);
