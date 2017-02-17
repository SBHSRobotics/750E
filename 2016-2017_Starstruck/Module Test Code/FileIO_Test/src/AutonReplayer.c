/*
 *  AutonReplayer.c
 *
 *  Created on: Feb 17, 2017
 *      Author: Mikel Matticoli
 */

 #include <main.h>

/*
 * This file abstracts all of the functionality of 750E's Auton Recorder.
 *   This file should be placed in the src directory. Note that it won't
 *   compile without AutonRecorder.h properly included in main.h.
 */

TaskHandle recordTask;
Frame *currentFrame;
int recordingSlot = 0;

 void replayerLoop();

void replayAuton(int slot) {
  recordingSlot = slot;
  currentFrame = malloc(sizeof(Frame *));
  recordTask = taskCreate(replayerLoop,TASK_DEFAULT_STACK_SIZE,NULL,TASK_PRIORITY_DEFAULT);
}

void stopAuton() {
  taskSuspend(recordTask);
  taskDelete(recordTask);
  motorStopAll();
}

void replayerLoop() {
  if (recordingSlot == 0) {
    return;
  }
  Frame *root = loadRecording(recordingSlot);
  currentFrame = root;
  while(currentFrame->next != root) {
    currentFrame = currentFrame->next;
    delay(200);
  }

}

int inputGetAnalog(unsigned char joystick, unsigned char axis) {
  if(isAutonomous()) {
    if(joystick == 1) {
      return currentFrame->analog_main[axis - 1];
    } else if (joystick == 2) {
      return currentFrame->analog_partner[axis - 1];
    }
  } else {
    return joystickGetAnalog(joystick, axis);
  }
}

bool inputGetDigital(unsigned char joystick, unsigned char buttonGroup,
    unsigned char button) {
  if(isAutonomous()) {
    if(joystick == 1) {
      // main
    } else if (joystick == 2) {
      // partner
    }
  } else {
    return joystickGetDigital(joystick, buttonGroup, button);
  }
}
