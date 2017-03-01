/*
 *  AutonRecorder.h
 *
 *  Created on: Feb 8, 2017
 *      Author: Kirsten Meidlinger
 */

#ifndef AUTONRECORDER_H_
#define AUTONRECORDER_H_

#include <stdlib.h>
#include <string.h>
/*
 * This is the library for 750E's Auton Recorder. It reads joystick values and writes them to
 *   a file in the cortex's memory, and is able to read the file again to replay it as an
 *   autonomous program. It can work for any game.
 *
 * This file should be placed in include and included in main.h
 */

  //Frame array macros, makes getting info from arrays within Frames more intuitive
  #define CH1 0
  #define CH2 1
  #define CH3 2
  #define CH4 3
  #define BTN5U 0
  #define BTN5D 1
  #define BTN6U 2
  #define BTN6D 3
  #define BTN7U 4
  #define BTN7D 5
  #define BTN7L 6
  #define BTN7R 7
  #define BTN8U 8
  #define BTN8D 9
  #define BTN8L 10
  #define BTN8R 11

  typedef struct Frame {
    int analog_main[4]; //ch1, ch2, ch3, ch4
    bool digital_main[12]; //5U, 5D, 6U, 6D, 7U, 7D, 7L, 7R, 8U, 8D, 8L, 8R
    int analog_partner[4];
    bool digital_partner[12];
  }Frame;
  /*
   * Structure that holds the analog and digital values of the current state of both joysticks
   */

  static const Frame NULLFRAME = {
    .analog_main = {255,255,255,255},
    .digital_main = {false,false,false,false,false,false,false,false,false,false,false,false},
    .analog_partner = {255,255,255,255},
    .digital_partner = {false,false,false,false,false,false,false,false,false,false,false,false}
  };
  /*
   * Global Frame that acts as a NULL operator for algorithmic purposes
   */

  int slot = 0;
  /*
   * Current recording slot, for use in recorder and replayer
   */

/* Public function declarations */

  /*
   * Starts the task that runs the AutonRecorder for the file Rec[s].txt
   *
   * @param s is the slot that the recording will be saved to
   */
  void startRecording(int s);

  /*
   * Sets the bool endTask to true, which will in turn suspend the recordingLoop task
   */
  void stopRecording();

  /*
   * When recording files, this reads the joysticks and puts their values in a Frame structure.
   *   The function returns a pointer to the current frame.
   */
  Frame getCurrentFrame();

  /*
   * Reads from file and prints all frames in order
   */
  void printAllFrames();

  /*
   * Converts frame values into a single string, analog values offset from (-127,127) to (0,254)
   *   Ex: 127127127127000000000000127127127127000000000000 <- all buttons and analog sticks are at 0
   */
  char* frameToString(Frame *frame);

  /*
   * Converts single string into a Frame struct
   */
  Frame stringToFrame(char* str);

  /*
   * Returns substring of str beginning at start of length length (first position in str is 1 not 0)
   *   Code taken from online at http://www.programmingsimplified.com/c/source-code/c-substring
   */
  char* substring(char* str,int start,int length);

  /*
   * Returns the recording slot
   */
  int getSlot();

  /*
   * Sets the current slot for replaying or recording to s
   */
  void setSlot(int s);

#endif /* AUTONRECORDER_H_ */
