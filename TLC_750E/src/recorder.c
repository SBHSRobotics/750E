#include "main.h"
#include <math.h>

FILE slot;

void record() {
  fopen(slot, "w");
  while (true) {
    if (ch3 > thresh) {
      fprint("0", slot);
    }
    else if (ch3 < -thresh) {
      fprint("1", slot);
    }
    if (ch4 < -thresh) {
      fprint("2", slot);
    }
    if (ch4 > thresh) {
      fprint("3", slot);
    }
    delay(200);
  }
}

void replay() {

}

void pause() {

}

void resume() {

}

void stop() {

}

void initialize() {
  TaskHandle taskHandle = taskCreate(record, TASK_DEFAULT_STACK_SIZE,
     NULL, TASK_PRIORITY_DEFAULT);
}
