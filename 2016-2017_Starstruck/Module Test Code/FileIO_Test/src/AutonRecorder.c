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

  void startRecording(int s){
    slot = s;
    //recordTask = taskCreate(recordingLoop,TASK_DEFAULT_STACK_SIZE,NULL,TASK_PRIORITY_DEFAULT);
    root = NULLFRAME;
    fileName = malloc(sizeof(char)*14);

  }

  void stopRecording(){
    printf("Deleting recordTask...\n");
    delay(200);
    //taskDelete(recordTask);
    //Frame currentFrame = root;
    printf("recordTask deleted.\n");
    delay(200);

    printf("Titling file...\n");
    delay(200);
    sprintf(fileName,"Recording%d.txt",slot);
    printf("File titled: %s\n",fileName);
    delay(200);

    printf("Opening %s...\n",fileName);
    delay(200);
    FILE* recording = fopen(fileName,"w");
    printf("%s opened.\n",fileName);
    delay(200);

    fprintf(recording,"127000127254000000000000127000127254000000000000");
    fprintf(recording,"128127126125000000000000128127126125000000000000");
    fprintf(recording,"001252069042010100100101127252069042010100100101");
    fclose(recording);

    printf("File %s written and closed.\n",fileName);
  	delay(200);
  }

  Frame loadRecording(int s){
    sprintf(fileName,"Recording%d.txt",s);
    FILE* f = fopen(fileName,"r");
    char *frame = malloc(50); //TODO memory leaks
    root = NULLFRAME;
    while(fgets(frame,51,f) != NULL){
      delay(200);
      if(root.analog_main[CH1] == -1){
        root = stringToFrame(frame); // returned Frame from stringToFrame might only be in local scope
        root.previous = &root;
        root.next = &root;
      } else {
        addFrame(stringToFrame(frame));
      }
    }

    free(frame);
    fclose(f);
    return root;
  }

  void recordingLoop(){
    if(root.analog_main[CH1] == -1){ //checks if it's equal to NULLFRAME
      root = getCurrentFrame();
    } else {
      addFrame(getCurrentFrame());
    }
    delay(200);
  }

  Frame getCurrentFrame(){
    Frame frame = {
      {joystickGetAnalog(1,1),joystickGetAnalog(1,2),joystickGetAnalog(1,3),joystickGetAnalog(1,4)},
      {joystickGetDigital(1,5,JOY_UP),joystickGetDigital(1,5,JOY_DOWN),joystickGetDigital(1,6,JOY_UP),joystickGetDigital(1,6,JOY_DOWN),
       joystickGetDigital(1,7,JOY_UP),joystickGetDigital(1,7,JOY_DOWN),joystickGetDigital(1,7,JOY_LEFT),joystickGetDigital(1,7,JOY_RIGHT),
       joystickGetDigital(1,8,JOY_UP),joystickGetDigital(1,8,JOY_DOWN),joystickGetDigital(1,8,JOY_LEFT),joystickGetDigital(1,8,JOY_RIGHT)},
      {joystickGetAnalog(2,1),joystickGetAnalog(2,2),joystickGetAnalog(2,3),joystickGetAnalog(2,4)},
      {joystickGetDigital(2,5,JOY_UP),joystickGetDigital(2,5,JOY_DOWN),joystickGetDigital(2,6,JOY_UP),joystickGetDigital(2,6,JOY_DOWN),
       joystickGetDigital(42,7,JOY_UP),joystickGetDigital(2,7,JOY_DOWN),joystickGetDigital(2,7,JOY_LEFT),joystickGetDigital(2,7,JOY_RIGHT),
       joystickGetDigital(2,8,JOY_UP),joystickGetDigital(2,8,JOY_DOWN),joystickGetDigital(2,8,JOY_LEFT),joystickGetDigital(2,8,JOY_RIGHT)},
       NULL,NULL
    };
    frame.next=&frame;
    frame.previous=&frame;
    return frame;
  }

  void printFrames(Frame *rootPtr){
    Frame currentFrame = *rootPtr;
    printf("printFrames: currentFrame initialized");
  	delay(200);
    printf("%s",frameToString(currentFrame));
  	delay(200);
    currentFrame = *(currentFrame.next);
  }

  void addFrame(Frame toAdd){
    toAdd.previous = root.previous;
    toAdd.next = &root;
    (*(root.previous)).next = &toAdd;
    root.previous = &toAdd;
  }

  char * frameToString(Frame frame){
    char * string = malloc(50);
    sprintf(string,"%03d%03d%03d%03d%d%d%d%d%d%d%d%d%d%d%d%d%03d%03d%03d%03d%d%d%d%d%d%d%d%d%d%d%d%d",
      frame.analog_main[CH1]+127,
      frame.analog_main[CH2]+127,
      frame.analog_main[CH3]+127,
      frame.analog_main[CH4]+127,
      frame.digital_main[BTN5U],
      frame.digital_main[BTN5D],
      frame.digital_main[BTN6U],
      frame.digital_main[BTN6D],
      frame.digital_main[BTN7U],
      frame.digital_main[BTN7D],
      frame.digital_main[BTN7L],
      frame.digital_main[BTN7R],
      frame.digital_main[BTN8U],
      frame.digital_main[BTN8D],
      frame.digital_main[BTN8L],
      frame.digital_main[BTN8R],
      frame.analog_partner[CH1]+127,
      frame.analog_partner[CH2]+127,
      frame.analog_partner[CH3]+127,
      frame.analog_partner[CH4]+127,
      frame.digital_partner[BTN5U],
      frame.digital_partner[BTN5D],
      frame.digital_partner[BTN6U],
      frame.digital_partner[BTN6D],
      frame.digital_partner[BTN7U],
      frame.digital_partner[BTN7D],
      frame.digital_partner[BTN7L],
      frame.digital_partner[BTN7R],
      frame.digital_partner[BTN8U],
      frame.digital_partner[BTN8D],
      frame.digital_partner[BTN8L],
      frame.digital_partner[BTN8R]
    );
    // TODO: code above is really ugly and inefficient lol
    printf("%s\n",string);
  	delay(200);
    return string;
  }

  Frame stringToFrame(char* string){
    printf("original string: %s",(string));
    delay(200);
    Frame frame = NULLFRAME;
    char* analog_main = substring(string,1,12);
    char* digital_main = substring(string,13,12);
    char* analog_partner = substring(string,25,12);
    char* digital_partner = substring(string,37,12);

    int x;
    printf("analog_main: %s",analog_main);
    delay(200);
    for(x=0;x<4;x++){
      printf("Analog Main %d: %s\n",x,substring(substring(string,1,12),(x*3)+1,3));
      delay(200);
      printf("Analog Partner %d: %s\n",x,substring(analog_partner,(x*3)+1,3));
      delay(200);
      frame.analog_main[x] = atoi(substring(analog_main,(x*3)+1,3))-127;
      frame.analog_partner[x] = atoi(substring(analog_partner,(x*3)+1,3))-127;
    }
    for(x=0;x<12;x++){
      frame.digital_main[x] = atoi(substring(digital_main,x+1,1));
      frame.digital_partner[x] = atoi(substring(digital_partner,x+1,1));
    }

    printf("%d\t%d\t%d\t%d\n",frame.analog_main[CH1],frame.analog_main[CH2],frame.analog_main[CH3],frame.analog_main[CH4]);
   	delay(200);
    printf("%d\t%d\t%d\t%d\n",frame.analog_partner[CH1],frame.analog_partner[CH2],frame.analog_partner[CH3],frame.analog_partner[CH4]);
   	delay(200);

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
