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
    bool digial_main[12]; //5U, 5D, 6U, 6D, 7U, 7D, 7L, 7R, 8U, 8D, 8L, 8R
    int analog_partner[4];
    bool digial_partner[12];
    Frame *previous;
    Frame *next;
  }Frame;

  void startRecorder(int slot);
  /*
   * Starts the task that runs the Auton Recorder for the file Recorder[slot].txt
   */

  void stopRecorder();
  /*
   */

  Frame loadRecording(int slot);
  /*
   * Gets file for recording at slot #, returns the root Frame of the recording
   */

#endif /* AUTONRECORDER_H_ */
