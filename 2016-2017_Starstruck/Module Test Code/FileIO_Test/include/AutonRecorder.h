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

//Frame array macros, makes getting info from Frames more intuitive

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


  static const Frame NULLFRAME = {
    .analog_main = {255,255,255,255},
    .digital_main = {false,false,false,false,false,false,false,false,false,false,false,false},
    .analog_partner = {255,255,255,255},
    .digital_partner = {false,false,false,false,false,false,false,false,false,false,false,false},
  };

  // TODO fix all of the descriptions lol

  void startRecording(int slot);
  /*
   * Starts the task that runs the Auton Recorder for the file Recorder[slot].txt
   */

  void stopRecording();
  /*
   *
   */

  Frame* getCurrentFrame();
  /*
   * When recording files, this reads the joysticks and puts their values in a Frame structure.
   *   The returned Frame's next and previous values are NULL, and should be set in the circular
   *   list of Frames in
   */

  void printAllFrames();
  /*
   * Reads from file and prints all frames
   */

  void addFrame(Frame *toAdd);
  /*
   * Adds toAdd to the end of the linked list of Frames
   *   Frames are structured circularly
   */

  char* frameToString(Frame *frame);
  /*
   * Converts frame values into a single string, analog values offset from (-127,127) to (0,254)
   */

  Frame stringToFrame(char* str);
  /*
   * Converts single string into a Frame with NULL next and previous
   */

  char* substring(char* str,int start,int length);
  /*
   * Returns substring of str beginning at start of length length (first position in str is 1 not 0)
   */

#endif /* AUTONRECORDER_H_ */
