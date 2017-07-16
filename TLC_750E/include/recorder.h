#ifndef INCLUDE_RECODER_H
#define INCLUDE_RECORDER_H

#include main.h

#define MOVE_UP 0
#define MOVE_DOWN 1
#define MOVE_LEFT 2
#define MOVE_RIGHT 3
#define TURN_LEFT 4
#define TURN_RIGHT 5
#define LIFT_UP 6
#define LIFT_DOWN 7
#define CLAW_OPEN 8
#define CLAW_CLOSE 9
#define IDLE 10

typedef struct Action {
  unsigned char actionType;
  int durationMillis;
}Action;

void record();
void replay();
void pause();
void resume();
void stop();
