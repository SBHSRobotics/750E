/*
 *  AutonRecorder.h
 *
 *  Created on: Feb 8, 2017
 *      Author: Kirsten Meidlinger
 */

#ifndef AUTONRECORDER_H_
#define AUTONRECORDER_H_

/*
 * This is the library for 750E's Auton Recorder. It reads joystick values and writes them to
 *   a file in the cortex's memory, and is able to read the file again to replay it as an
 *   autonomous program. It can work for any game.
 *
 * This file should be placed in include and included in main.h
 */


  typedef struct Frame {
      int analog_main[4]; //ch1, ch2, ch3, ch4
      bool digital_main[12]; //5U, 5D, 6U, 6D, 7U, 7D, 7L, 7R, 8U, 8D, 8L, 8R
      int analog_partner[4];
      bool digital_partner[12];
      struct Frame *next;
      struct Frame *previous;
    }Frame;


  static const Frame NULLFRAME = {
    {-1,-1,-1,-1},
    {false,false,false,false,false,false,false,false,false,false,false,false},
    {-1,-1,-1,-1},
    {false,false,false,false,false,false,false,false,false,false,false,false},
    (Frame*)0,(Frame*)0
  };

  void startRecording(int slot);
  /*
   * Starts the task that runs the Auton Recorder for the file Recorder[slot].txt
   */

  void stopRecording();
  /*
   * Stops the Recorder task and uses the root Frame to write to the file. Does not require a slot
   *   because only one Auton Recorder should run at a time.
   */

  Frame loadRecording(int slot);
  /*
   * Gets file for recording at slot #, returns the root Frame of the recording
   */

  void recordingLoop();
  /*
   * Manages the linked list of Frames
   */

  Frame getCurrentFrame();
  /*
   * When recording files, this reads the joysticks and puts their values in a Frame structure.
   *   The returned Frame's next and previous values are NULL, and should be set in the circular
   *   list of Frames in
   */

  void addFrame(Frame toAdd);
  /*
   * Adds toAdd to the end of the linked list of Frames
   *   Frames are structured circularly
   */

  char* frameToString(Frame frame);
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
