/*
 *  AutonReplayer.c
 *
 *  Created on: Feb 17, 2017
 *      Author: Mikel Matticoli and Kirsten Meidlinger
 */

 #include <main.h>

/*
 * This file abstracts all of the functionality of 750E's Auton Replayer.
 *   This file should be placed in the src directory. Note that it won't
 *   compile without AutonReplayer.h properly included in main.h.
 */

/* Global variable declarations */

  TaskHandle replayTask;
  Frame *currentFrame;
  char* fileName;
  bool isRunning = false;

  int activeSlot = 1;
/* Private function declarations */

  void replayerLoop();

/* Public function definitons */

  void startAuton() {

    //#if (DEBUG_MODE == 1)
      printf("Replaying auton %d...\n",activeSlot);
      delay(100);
    //#endif

    // Defines global variables and starts replayTask
    currentFrame = malloc(sizeof(Frame *));
    fileName = malloc(sizeof(char *));
    sprintf(fileName,"Rec%d.txt",activeSlot);
    isRunning = true;
    replayTask = taskCreate(replayerLoop,TASK_DEFAULT_STACK_SIZE,NULL,TASK_PRIORITY_DEFAULT);
  }

  void stopAuton() {
    // Stops the auton by suspending and deleting replayTask and stops all of the motors on the robot
    printf("Stopping auton...\n");
    isRunning = false;
    taskSuspend(replayTask);
    taskDelete(replayTask);
    motorStopAll();
    // Frees allocated memory from currentFrame and fileName
    // free(currentFrame);
    // free(fileName);
    printf("Auton stopped.\n");
  }

  bool isReplayerAuton(){
    return isRunning;
  }

  int inputGetAnalog(unsigned char joystick, unsigned char axis) {
    // If the robot is replaying auton, get values from currentFrame
    if(isAutonomous() || isReplayerAuton()) {
      if(joystick == 1) {
        return currentFrame->analog_main[axis - 1];
      } else if (joystick == 2) {
        return currentFrame->analog_partner[axis - 1];
      }
    }
    // Otherwise return the value of joystick for use in opcontrol
    return joystickGetAnalog(joystick, axis);
  }

  bool inputGetDigital(unsigned char joystick, unsigned char buttonGroup,
      unsigned char button) {
    // If the robot is replaying auton, get values from currentFrame
    if(isAutonomous() || isReplayerAuton()) {
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
      } else if (joystick == 2) {  //was joystick==2
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
    // Otherwise return the value of joystick for use in opcontrol
    return joystickGetDigital(joystick, buttonGroup, button);
  }

/* Private function definitions */

  void replayerLoop() {
    // If slot doesn't initialize properly, end replay immediately
    if (activeSlot == 0) {
      printf("DED");
      return;
    }
    #if (DEBUG_MODE == 1)
      printf("Opening recording %d from file %s...\n", activeSlot, fileName);
      delay(100);
    #endif

    // Initializes variables for iterating through file
    FILE* recording = fopen(fileName,"r");
    char* frameString = malloc(sizeof(char)*50);

    // Makes sure the file exists, otherwise ends replay
    if (recording == NULL) {
      for(int i = 0;i <3;i++){
        boop();
        delay(500);
        #if (DEBUG_MODE == 1)
          printf("Replayer Error: Selected slot is empty");
        #endif
      }
      stopAuton();
      return;
    }
    #if (DEBUG_MODE == 1)
      printf("Recording %d opened.\n",activeSlot);
      delay(100);
    #endif

    // Loop only runs in autonomous mode
    // fread assigns values to frameString from recording until end of file
    while((isAutonomous() || isReplayerAuton()) && fread(frameString,1,49,recording) != NULL){
      #if (DEBUG_MODE == 1)
        printf("Setting frame: %s",frameString);
        delay(75);
      #endif

      // Creates frame from string and assigns it to
      //   global variable currentFrame for use in inputGet functions
      *currentFrame = stringToFrame(frameString);

      // Sets values from currentFrame to motors on robot
      joystickMap();

      #if (DEBUG_MODE == 1)
        printf("LF %d",motorGet(LF));
        delay(50);
      #endif

      delay(170);
    }
    #if (DEBUG_MODE == 1)
      printf("Closing recording %d...\n",activeSlot);
      delay(100);
    #endif

    // Closes recording file
    fclose(recording);

    // Frees allocated memory from frameString
    // free(frameString);
    #if (DEBUG_MODE == 1)
      printf("Recording %d closed.\n",activeSlot);
      delay(100);
    #endif
    stopAuton();
  }

  void setActiveSlot(int slot) {
    activeSlot = slot;
  }
