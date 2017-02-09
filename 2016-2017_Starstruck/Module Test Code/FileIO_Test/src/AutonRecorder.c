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
 *   compile without AutonRecorder.h properly included.
 */

FILE* recording;

//Public functions
  Frame getCurrentFrame(){

  }
  /*
   * When recording files, this reads the joysticks and puts their values in a Frame structure.
   *   should be called every 200ms in the Recorder task
   */

  void addFrame(Frame root, Frame toAdd){

  }
  /*
   * Adds toAdd to the end of the linked list of Frames
   *   Frames are structured circularly
   */
