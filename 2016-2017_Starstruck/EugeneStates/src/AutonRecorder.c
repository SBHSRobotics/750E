/*
 *  AutonRecorder.c
 *
 *  Created on: Feb 8, 2017
 *      Author: Kirsten Meidlinger
 */

 #include <main.h>

/*
 * This file abstracts all of the functionality of 750E's Auton Recorder.
 *   This file should be placed in the src directory. Note that it won't
 *   compile without AutonRecorder.h properly included in main.h.
 */

  int slot;
  TaskHandle recordTask;
  Frame root;
  char* fileName;
  bool endTask;

/* Private function declarations */

  void recordingLoop();
  /*
   * Function contains a while loop that reads the joysticks and prints the values to a file
   */

/* Public function definitons */

  void startRecording(int s){
    printf("Starting recording %d...\n",s);
    delay(100);

    // Initializes global variables and starts recordTask
    slot = s;
    root = NULLFRAME;
    fileName = malloc(sizeof(char *));
    sprintf(fileName,"Rec%d.txt",slot);
    endTask = false;
    recordTask = taskCreate(recordingLoop,TASK_DEFAULT_STACK_SIZE,NULL,TASK_PRIORITY_DEFAULT);

    printf("Recording %d started.\n\n",s);
    delay(100);
  }

  void stopRecording(){
    // Sets endTask to true in order to suspend recordTask
    endTask = true;
  }

  Frame getCurrentFrame(){
    // Creates Frame struct of current joystick state and returns it
    Frame frame = {
      .analog_main = {joystickGetAnalog(1,1)+127,joystickGetAnalog(1,2)+127,joystickGetAnalog(1,3)+127,joystickGetAnalog(1,4)+127},
      .digital_main = {joystickGetDigital(1,5,JOY_UP),joystickGetDigital(1,5,JOY_DOWN),joystickGetDigital(1,6,JOY_UP),joystickGetDigital(1,6,JOY_DOWN),
                        joystickGetDigital(1,7,JOY_UP),joystickGetDigital(1,7,JOY_DOWN),joystickGetDigital(1,7,JOY_LEFT),joystickGetDigital(1,7,JOY_RIGHT),
                        joystickGetDigital(1,8,JOY_UP),joystickGetDigital(1,8,JOY_DOWN),joystickGetDigital(1,8,JOY_LEFT),joystickGetDigital(1,8,JOY_RIGHT),
                      },
      .analog_partner = {joystickGetAnalog(2,1)+127,joystickGetAnalog(2,2)+127,joystickGetAnalog(2,3)+127,joystickGetAnalog(2,4)+127},
      .digital_partner = {joystickGetDigital(2,5,JOY_UP),joystickGetDigital(2,5,JOY_DOWN),joystickGetDigital(2,6,JOY_UP),joystickGetDigital(2,6,JOY_DOWN),
                           joystickGetDigital(2,7,JOY_UP),joystickGetDigital(2,7,JOY_DOWN),joystickGetDigital(2,7,JOY_LEFT),joystickGetDigital(2,7,JOY_RIGHT),
                           joystickGetDigital(2,8,JOY_UP),joystickGetDigital(2,8,JOY_DOWN),joystickGetDigital(2,8,JOY_LEFT),joystickGetDigital(2,8,JOY_RIGHT),
                          }
    };
    return frame;
  }

  void printAllFrames(){
    printf("Printing all frames...\n");
    delay(100);

    // Initializes variables for iterating through file
    char* frame = malloc(sizeof(char)*50);
    FILE* recording = fopen(fileName,"r");

    // fgets saves the next 50 bytes from recording to string frame
    while(fgets(frame,50,recording) != NULL){
      // Prints current frame
      printf("%s",frame);
      delay(100);
    }
    // Closes recording
    fclose(recording);
    printf("All frames printed.\n");
    delay(100);
  }

  char* frameToString(Frame *frame){
    //Initializes variables for saving to a string
      // Saves these certain values to int variables, cortex issue
    int ch3 = frame->analog_main[CH3];
    int ch4 = frame->analog_main[CH4];
    char* string = malloc(sizeof(char *));
    sprintf(string,"%03d%03d%03d%03d%d%d%d%d%d%d%d%d%d%d%d%d%03d%03d%03d%03d%d%d%d%d%d%d%d%d%d%d%d%d\n",
      ((frame->analog_main[CH1] < 0 || frame->analog_main[CH1] > 255) ? 0 : frame->analog_main[CH1]),
      ((frame->analog_main[CH2] < 0 || frame->analog_main[CH2] > 255) ? 0 : frame->analog_main[CH2]),
      ((ch3 < 0 || ch3 > 255) ? 0 : ch3),
      ((ch4 < 0 || ch4 > 255) ? 0 : ch4),
      ((frame->digital_main[BTN5U] == false) ? 0 : 1),
      ((frame->digital_main[BTN5D] == false) ? 0 : 1),
      ((frame->digital_main[BTN6U] == false) ? 0 : 1),
      ((frame->digital_main[BTN6D] == false) ? 0 : 1),
      ((frame->digital_main[BTN7U] == false) ? 0 : 1),
      ((frame->digital_main[BTN7D] == false) ? 0 : 1),
      ((frame->digital_main[BTN7L] == false) ? 0 : 1),
      ((frame->digital_main[BTN7R] == false) ? 0 : 1),
      ((frame->digital_main[BTN8U] == false) ? 0 : 1),
      ((frame->digital_main[BTN8D] == false) ? 0 : 1),
      ((frame->digital_main[BTN8L] == false) ? 0 : 1),
      ((frame->digital_main[BTN8R] == false) ? 0 : 1),
      ((frame->analog_partner[CH1] < 0 || frame->analog_partner[CH1] > 255) ? 0 : frame->analog_partner[CH1]),
      ((frame->analog_partner[CH2] < 0 || frame->analog_partner[CH2] > 255) ? 0 : frame->analog_partner[CH2]),
      ((frame->analog_partner[CH3] < 0 || frame->analog_partner[CH3] > 255) ? 0 : frame->analog_partner[CH3]),
      ((frame->analog_partner[CH4] < 0 || frame->analog_partner[CH4] > 255) ? 0 : frame->analog_partner[CH4]),
      ((frame->digital_partner[BTN5U] == false) ? 0 : 1),
      ((frame->digital_partner[BTN5D] == false) ? 0 : 1),
      ((frame->digital_partner[BTN6U] == false) ? 0 : 1),
      ((frame->digital_partner[BTN6D] == false) ? 0 : 1),
      ((frame->digital_partner[BTN7U] == false) ? 0 : 1),
      ((frame->digital_partner[BTN7D] == false) ? 0 : 1),
      ((frame->digital_partner[BTN7L] == false) ? 0 : 1),
      ((frame->digital_partner[BTN7R] == false) ? 0 : 1),
      ((frame->digital_partner[BTN8U] == false) ? 0 : 1),
      ((frame->digital_partner[BTN8D] == false) ? 0 : 1),
      ((frame->digital_partner[BTN8L] == false) ? 0 : 1),
      ((frame->digital_partner[BTN8R] == false) ? 0 : 1)
    );

    return string;
  }

  Frame stringToFrame(char* string){
    // Initializes variables for splitting string into Frame values
    Frame frame = NULLFRAME;
    // Splits string into analog and digital values from main and partner joysticks
    char* analog_main = substring(string,1,12);
    char* digital_main = substring(string,13,12);
    char* analog_partner = substring(string,25,12);
    char* digital_partner = substring(string,37,12);

    // For loops split analog and digital strings into individual
    //  values to be saved in the frame
    int x;
    for(x=0;x<4;x++){
      frame.analog_main[x] = atoi(substring(analog_main,(x*3)+1,3))-127;
      frame.analog_partner[x] = atoi(substring(analog_partner,(x*3)+1,3))-127;
    }
    for(x=0;x<12;x++){
      frame.digital_main[x] = atoi(substring(digital_main,x+1,1));
      frame.digital_partner[x] = atoi(substring(digital_partner,x+1,1));
    }

    return frame;
  }

  char* substring(char* str,int start,int length){
    // Splits a string into a substring from index start (first index is 1, not 0) of length length
    // Code taken from online, link in header file
    char *substring = malloc(length+1);
    int subPos;

    if(substring == NULL){
      printf("str in substring method is NULL.\n");
     	delay(100);
      exit(1);
    }

    for(subPos = 0 ; subPos < length ; subPos++){
       *(substring+subPos) = *(str+start-1);
       str++;
    }

    *(substring+subPos) = '\0';

    return substring;
  }

/* Private function declarations */

  void recordingLoop(){
    // Opens file Rec[slot].txt in write mode
    printf("Opening %s...\n",fileName);
    delay(100);
    FILE* recording = fopen(fileName,"w");
    printf("%s opened.\n\n",fileName);
    delay(100);

    // Initializes variables for saving joystick values
    Frame currentFrame;
    char* frameVal = malloc(sizeof(char)*50);
    while(!endTask){
      // Gets current joystick state
      currentFrame = getCurrentFrame();
      frameVal = frameToString(&currentFrame);
      printf("%s",frameVal);
      delay(100);

      // Prints frameVal to recording
      fprintf(recording,frameVal);

      // If endTask is true, break out of the loop and suspend the task
      if(endTask){
        break;
      }
      delay(200);
    }
    printf("Closing recording %d...\n",slot);
    delay(100);

    // Closes recording
    fclose(recording);

    printf("Recording %d closed.\n\n",slot);
    delay(100);

    // Suspends recordTask
    taskSuspend(recordTask);
  }
