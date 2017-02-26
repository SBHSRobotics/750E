/*
 *  AutonReplayer.c
 *
 *  Created on: Feb 17, 2017
 *      Author: Mikel Matticoli and Kirsten Meidlinger
 */

 #include <main.h>

/*
 * This file abstracts all of the functionality of 750E's Auton Recorder.
 *   This file should be placed in the src directory. Note that it won't
 *   compile without AutonRecorder.h properly included in main.h.
 */

TaskHandle replayTask;
Frame *currentFrame;
int slot = 0;
char* fileName;

void replayerLoop();

void startAuton(int s) {
  printf("Replaying auton %d...\n",slot);
  delay(200);
  slot = s;
  currentFrame = malloc(sizeof(Frame *));
  fileName = malloc(sizeof(char)*7);
  sprintf(fileName,"Rec%d.txt",slot);
  replayTask = taskCreate(replayerLoop,TASK_DEFAULT_STACK_SIZE,NULL,TASK_PRIORITY_DEFAULT);
}

void stopAuton() {
  printf("Stopping auton...\n");
  taskSuspend(replayTask);
  taskDelete(replayTask);
  motorStopAll();
  printf("Auton stopped.\n\n");
}

void replayerLoop() {
  if (slot == 0) {
    return;
  }
  FILE* recording = fopen(fileName,"r");
  char* frameString = malloc(sizeof(char)*50);
  while(/*isAutonomous() && */fgets(frameString,50,recording) != NULL){
    joystickMap();
    printf("%s",frameString);
    delay(200);
  }
  printf("Closing file.\n");
  delay(100);

  fclose(recording);
}

int inputGetAnalog(unsigned char joystick, unsigned char axis) {
  if(isAutonomous()) {
    if(joystick == 1) {
      return currentFrame->analog_main[axis - 1];
    } else if (joystick == 2) {
      return currentFrame->analog_partner[axis - 1];
    }
  }
  return joystickGetAnalog(joystick, axis);
}

bool inputGetDigital(unsigned char joystick, unsigned char buttonGroup,
    unsigned char button) {
  if(isAutonomous()) {
    if(joystick == 1) { //main
      switch(buttonGroup){
        case 5:
          if(button==JOY_UP){
            return currentFrame->digital_main[BTN5U];
          } else if(button==JOY_DOWN){
            return currentFrame->digital_main[BTN5D];
          }
          break;
        case 6:
          if(button==JOY_UP){
            return currentFrame->digital_main[BTN6U];
          } else if(button==JOY_DOWN){
            return currentFrame->digital_main[BTN6D];
          }
          break;
        case 7:
          if(button==JOY_UP){
            return currentFrame->digital_main[BTN7U];
          } else if(button==JOY_DOWN){
            return currentFrame->digital_main[BTN7D];
          } else if(button==JOY_LEFT){
            return currentFrame->digital_main[BTN7L];
          } else if(button==JOY_RIGHT){
            return currentFrame->digital_main[BTN7R];
          }
          break;
        case 8:
          if(button==JOY_UP){
            return currentFrame->digital_main[BTN8U];
          } else if(button==JOY_DOWN){
            return currentFrame->digital_main[BTN8D];
          } else if(button==JOY_LEFT){
            return currentFrame->digital_main[BTN8L];
          } else if(button==JOY_RIGHT){
            return currentFrame->digital_main[BTN8R];
          }
          break;
      }
    } else if (joystick == 2) {
      switch(buttonGroup){
        case 5:
          if(button==JOY_UP){
            return currentFrame->digital_partner[BTN5U];
          } else if(button==JOY_DOWN){
            return currentFrame->digital_partner[BTN5D];
          }
          break;
        case 6:
          if(button==JOY_UP){
            return currentFrame->digital_partner[BTN6U];
          } else if(button==JOY_DOWN){
            return currentFrame->digital_partner[BTN6D];
          }
          break;
        case 7:
          if(button==JOY_UP){
            return currentFrame->digital_partner[BTN7U];
          } else if(button==JOY_DOWN){
            return currentFrame->digital_partner[BTN7D];
          } else if(button==JOY_LEFT){
            return currentFrame->digital_partner[BTN7L];
          } else if(button==JOY_RIGHT){
            return currentFrame->digital_partner[BTN7R];
          }
          break;
        case 8:
          if(button==JOY_UP){
            return currentFrame->digital_partner[BTN8U];
          } else if(button==JOY_DOWN){
            return currentFrame->digital_partner[BTN8D];
          } else if(button==JOY_LEFT){
            return currentFrame->digital_partner[BTN8L];
          } else if(button==JOY_RIGHT){
            return currentFrame->digital_partner[BTN8R];
          }
          break;
        }
    }
  }
  return joystickGetDigital(joystick, buttonGroup, button);
}
