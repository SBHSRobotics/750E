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
  char *fileName;
  bool endTask;

  void startRecording(int s){
    printf("Starting recording %d...\n",s);
    delay(200);

    slot = s;
    recordTask = taskCreate(recordingLoop,TASK_DEFAULT_STACK_SIZE,NULL,TASK_PRIORITY_DEFAULT);
    root = NULLFRAME;
    fileName = malloc(sizeof(char)*14);
    endTask = false;

    printf("Recording %d started.\n",s);
    delay(200);
  }

  void stopRecording(){
    printf("Stopping recording %d...\n",slot);
    delay(200);
    Frame currentFrame = root;

    printf("Titling file...\n");
    delay(200);
    sprintf(fileName,"Auton%d.txt",slot);
    printf("File titled: %s\n",fileName);
    delay(200);

    printf("Opening %s...\n",fileName);
    delay(200);
    FILE* recording = fopen(fileName,"w");
    printf("%s opened.\n",fileName);
    delay(200);

    do{
      char* frameVal = frameToString(currentFrame);
      fprintf(recording,"%s",frameVal);
      delay(100);
      printFrame(currentFrame);
      currentFrame = *(currentFrame.next);
    } while (currentFrame.next == NULL);
    fclose(recording);

    printf("File %s written and closed.\nrecordTask suspended.\nRecording %d stopped.\n",fileName,slot);
  	delay(200);

  }

  Frame loadRecording(int s){
    printf("Loading recording %d...\n",s);
    delay(200);
    sprintf(fileName,"Auton%d.txt",s);
    FILE* f = fopen(fileName,"r");
    char *frame = malloc(50); //TODO memory leaks
    root = NULLFRAME;
    while(fgets(frame,51,f) != NULL){
      delay(200);
      if(root.analog_main[CH1] == 255){
        printf("Adding root...\n");
        delay(200);
        root = stringToFrame(frame); // returned Frame from stringToFrame might only be in local scope
        root.previous = &root;
        root.next = &root;
        printf("Root added.\n");
        delay(200);
      } else {
        printf("Adding frame %s...\n",frame);
        delay(200);
        addFrame(stringToFrame(frame));
        printf("Frame %s added.\n",frame);
        delay(200);
      }
    }

    free(frame);
    fclose(f);

    printf("Recording %d loaded.\n",s);
    delay(200);
    return root;
  }

  void recordingLoop(){
    while(!endTask){
      printf("Running recordingLoop...\n");
      delay(200);
      if(root.analog_main[CH1] == 255){ //checks if it's equal to NULLFRAME
        root = getCurrentFrame();
        addFrame(root);
      } else {
        printf("reached\n");
        Frame currentFrame = getCurrentFrame();
        addFrame(currentFrame);
        //printFrame(currentFrame);
        if(currentFrame.digital_main[BTN5D] == true){
          endTask=true;
          printf("5 down pressed.\n");
          delay(200);
        }
      }
      delay(200);
      if(endTask){ //TODO when this works, make one unused button on joystick end the task
        break;
      }
    }
    printf("Suspending recordTask...\n");
    delay(200);
    printAllFrames(&root);
    stopRecording();
    delay(200);
    taskSuspend(recordTask);
  }

  Frame getCurrentFrame(){
    int analog_main[4] = {joystickGetAnalog(1,1),joystickGetAnalog(1,2),joystickGetAnalog(1,3),joystickGetAnalog(1,4)};
    int digital_main[12] = {joystickGetDigital(1,5,JOY_UP),joystickGetDigital(1,5,JOY_DOWN),joystickGetDigital(1,6,JOY_UP),joystickGetDigital(1,6,JOY_DOWN),
                            joystickGetDigital(1,7,JOY_UP),joystickGetDigital(1,7,JOY_DOWN),joystickGetDigital(1,7,JOY_LEFT),joystickGetDigital(1,7,JOY_RIGHT),
                            joystickGetDigital(1,8,JOY_UP),joystickGetDigital(1,8,JOY_DOWN),joystickGetDigital(1,8,JOY_LEFT),joystickGetDigital(1,8,JOY_RIGHT),
                          };
    int analog_partner[4] = {joystickGetAnalog(2,1),joystickGetAnalog(2,2),joystickGetAnalog(2,3),joystickGetAnalog(2,4)};
    int digital_partner[12] = {joystickGetDigital(2,5,JOY_UP),joystickGetDigital(2,5,JOY_DOWN),joystickGetDigital(2,6,JOY_UP),joystickGetDigital(2,6,JOY_DOWN),
                            joystickGetDigital(2,7,JOY_UP),joystickGetDigital(2,7,JOY_DOWN),joystickGetDigital(2,7,JOY_LEFT),joystickGetDigital(2,7,JOY_RIGHT),
                            joystickGetDigital(2,8,JOY_UP),joystickGetDigital(2,8,JOY_DOWN),joystickGetDigital(2,8,JOY_LEFT),joystickGetDigital(2,8,JOY_RIGHT),
                          };

    Frame frame = {
      .analog_main = {analog_main[CH1],analog_main[CH2],analog_main[CH3],analog_main[CH4]},
      .digital_main = {digital_main[BTN5U],digital_main[BTN5D],digital_main[BTN6U],digital_main[BTN6D],
                       digital_main[BTN7U],digital_main[BTN7D],digital_main[BTN7L],digital_main[BTN7R],
                       digital_main[BTN8U],digital_main[BTN8D],digital_main[BTN8L],digital_main[BTN8R]
                      },
      .analog_partner = {analog_partner[CH1],analog_partner[CH2],analog_partner[CH3],analog_partner[CH4]},
      .digital_partner = {digital_partner[BTN5U],digital_partner[BTN5D],digital_partner[BTN6U],digital_partner[BTN6D],
                          digital_partner[BTN7U],digital_partner[BTN7D],digital_partner[BTN7L],digital_partner[BTN7R],
                          digital_partner[BTN8U],digital_partner[BTN8D],digital_partner[BTN8L],digital_partner[BTN8R]
                      },
      .next = NULL,
      .previous = NULL
    };
    frame.next=&frame;
    frame.previous=&frame;
    printFrame(frame);
    return frame;
  }

  void printAllFrames(Frame *rootPtr){
    printf("Printing all frames...\n");
    delay(200);
    Frame currentFrame = *rootPtr;
    while(currentFrame.next != rootPtr){
      printf("%s\n",frameToString(currentFrame));
    	delay(200);
      currentFrame = *(currentFrame.next);
    }
    printf("All frames printed.\n");
    delay(200);
  }

  void addFrame(Frame toAdd){
    toAdd.previous = root.previous;
    toAdd.next = &root;
    (*(root.previous)).next = &toAdd;
    root.previous = &toAdd;
    printf("%s added.\n",frameToString(toAdd));
    delay(100);
    printf("Root previous: %d",*root.previous);
    delay(100);
  }

  char * frameToString(Frame frame){
    char * string = malloc(50);
    sprintf(string,"%03d%03d%03d%03d%d%d%d%d%d%d%d%d%d%d%d%d%03d%03d%03d%03d%d%d%d%d%d%d%d%d%d%d%d%d",
      (frame.analog_main[CH1]+127 < 0 || frame.analog_main[CH1]+127 > 255) ? 0 : frame.analog_main[CH1]+127,
      (frame.analog_main[CH2]+127 < 0 || frame.analog_main[CH2]+127 > 255) ? 0 : frame.analog_main[CH2]+127,
      (frame.analog_main[CH3]+127 < 0 || frame.analog_main[CH3]+127 > 255) ? 0 : frame.analog_main[CH3]+127,
      (frame.analog_main[CH4]+127 < 0 || frame.analog_main[CH4]+127 > 255) ? 0 : frame.analog_main[CH4]+127,
      (frame.digital_main[BTN5U] == 0 || frame.digital_main[BTN5U] == 1) ? frame.digital_main[BTN5U] : 0,
      (frame.digital_main[BTN5D] == 0 || frame.digital_main[BTN5D] == 1) ? frame.digital_main[BTN5D] : 0,
      (frame.digital_main[BTN6U] == 0 || frame.digital_main[BTN6U] == 1) ? frame.digital_main[BTN6U] : 0,
      (frame.digital_main[BTN6D] == 0 || frame.digital_main[BTN6D] == 1) ? frame.digital_main[BTN6D] : 0,
      (frame.digital_main[BTN7U] == 0 || frame.digital_main[BTN7U] == 1) ? frame.digital_main[BTN7U] : 0,
      (frame.digital_main[BTN7D] == 0 || frame.digital_main[BTN7D] == 1) ? frame.digital_main[BTN7D] : 0,
      (frame.digital_main[BTN7L] == 0 || frame.digital_main[BTN7L] == 1) ? frame.digital_main[BTN7L] : 0,
      (frame.digital_main[BTN7R] == 0 || frame.digital_main[BTN7R] == 1) ? frame.digital_main[BTN7R] : 0,
      (frame.digital_main[BTN8U] == 0 || frame.digital_main[BTN8U] == 1) ? frame.digital_main[BTN8U] : 0,
      (frame.digital_main[BTN8D] == 0 || frame.digital_main[BTN8D] == 1) ? frame.digital_main[BTN8D] : 0,
      (frame.digital_main[BTN8L] == 0 || frame.digital_main[BTN8L] == 1) ? frame.digital_main[BTN8L] : 0,
      (frame.digital_main[BTN8R] == 0 || frame.digital_main[BTN8R] == 1) ? frame.digital_main[BTN8R] : 0,
      (frame.analog_partner[CH1]+127 < 0 || frame.analog_partner[CH1]+127 > 255) ? 0 : frame.analog_partner[CH1]+127,
      (frame.analog_partner[CH2]+127 < 0 || frame.analog_partner[CH2]+127 > 255) ? 0 : frame.analog_partner[CH2]+127,
      (frame.analog_partner[CH3]+127 < 0 || frame.analog_partner[CH3]+127 > 255) ? 0 : frame.analog_partner[CH3]+127,
      (frame.analog_partner[CH4]+127 < 0 || frame.analog_partner[CH4]+127 > 255) ? 0 : frame.analog_partner[CH4]+127,
      (frame.digital_partner[BTN5U] == 0 || frame.digital_partner[BTN5U] == 1) ? frame.digital_partner[BTN5U] : 0,
      (frame.digital_partner[BTN5D] == 0 || frame.digital_partner[BTN5D] == 1) ? frame.digital_partner[BTN5D] : 0,
      (frame.digital_partner[BTN6U] == 0 || frame.digital_partner[BTN6U] == 1) ? frame.digital_partner[BTN6U] : 0,
      (frame.digital_partner[BTN6D] == 0 || frame.digital_partner[BTN6D] == 1) ? frame.digital_partner[BTN6D] : 0,
      (frame.digital_partner[BTN7U] == 0 || frame.digital_partner[BTN7U] == 1) ? frame.digital_partner[BTN7U] : 0,
      (frame.digital_partner[BTN7D] == 0 || frame.digital_partner[BTN7D] == 1) ? frame.digital_partner[BTN7D] : 0,
      (frame.digital_partner[BTN7L] == 0 || frame.digital_partner[BTN7L] == 1) ? frame.digital_partner[BTN7L] : 0,
      (frame.digital_partner[BTN7R] == 0 || frame.digital_partner[BTN7R] == 1) ? frame.digital_partner[BTN7R] : 0,
      (frame.digital_partner[BTN8U] == 0 || frame.digital_partner[BTN8U] == 1) ? frame.digital_partner[BTN8U] : 0,
      (frame.digital_partner[BTN8D] == 0 || frame.digital_partner[BTN8D] == 1) ? frame.digital_partner[BTN8D] : 0,
      (frame.digital_partner[BTN8L] == 0 || frame.digital_partner[BTN8L] == 1) ? frame.digital_partner[BTN8L] : 0,
      (frame.digital_partner[BTN8R] == 0 || frame.digital_partner[BTN8R] == 1) ? frame.digital_partner[BTN8R] : 0
    );
    // TODO: code above is really ugly and inefficient lol
    return string;
  }

  Frame stringToFrame(char* string){
    Frame frame = NULLFRAME;
    char* analog_main = substring(string,1,12);
    char* digital_main = substring(string,13,12);
    char* analog_partner = substring(string,25,12);
    char* digital_partner = substring(string,37,12);

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

    /*char *substring;// = malloc(length+1);
    strncpy(substring, str+start, length-start);
    return substring;

    */ // Code from Mikel, returns 712 instead of 127

    char *substring = malloc(length+1);
    int subPos;

    if(substring == NULL){
      printf("str in substring method is NULL.\n");
     	delay(200);
      exit(1);
    }

    for(subPos = 0 ; subPos < length ; subPos++){
       *(substring+subPos) = *(str+start-1);
       str++;
    }

    *(substring+subPos) = '\0';

    return substring;
  }

  void printFrame(Frame frame){
    printf("printFrame:\n");
    delay(100);
    printf("Analog Main: %d\t%d\t%d\t%d\t\n",frame.analog_main[CH1],frame.analog_main[CH2],frame.analog_main[CH3],frame.analog_main[CH4]);
    delay(200);

    printf("Digital Main\t: %d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t\n",
      frame.digital_main[BTN5U],frame.digital_main[BTN5D],frame.digital_main[BTN6U],frame.digital_main[BTN6D],
      frame.digital_main[BTN7U],frame.digital_main[BTN7D],frame.digital_main[BTN7L],frame.digital_main[BTN7R],
      frame.digital_main[BTN8U],frame.digital_main[BTN8D],frame.digital_main[BTN8L],frame.digital_main[BTN8R]
    );
    delay(200);

    printf("Analog Partner: %d\t%d\t%d\t%d\t\n",frame.analog_partner[CH1],frame.analog_partner[CH2],frame.analog_partner[CH3],frame.analog_partner[CH4]);
    delay(200);

    printf("Digital Partner\t: %d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t\n",
      frame.digital_partner[BTN5U],frame.digital_partner[BTN5D],frame.digital_partner[BTN6U],frame.digital_partner[BTN6D],
      frame.digital_partner[BTN7U],frame.digital_partner[BTN7D],frame.digital_partner[BTN7L],frame.digital_partner[BTN7R],
      frame.digital_partner[BTN8U],frame.digital_partner[BTN8D],frame.digital_partner[BTN8L],frame.digital_partner[BTN8R]
    );
    delay(200);

  }
