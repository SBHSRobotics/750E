/*
 *  AutonReplayer.h
 *
 *  Created on: Feb 17, 2017
 *      Author: Mikel Matticoli and Kirsten Meidlinger
 */

#ifndef AUTONREPLAYER_H_

/*
 * This is the library for 750E's Auton Replayer. It reads from a file in the cortex's
 *   file system and converts its contents into values to send to the robot's motors.
 *   It can work for any game.
 *
 * This file should be placed in include and included in main.h
 */

/* Public function declarations */

  void startAuton(int slot);
  /*
   * Starts the task that replays the autonomous program in Rec[slot].txt
   */

  void stopAuton();
  /*
   * Suspends and deletes the replayer task which stops the auton
   */

  int inputGetAnalog(unsigned char joystick, unsigned char axis);
  /*
   * Returns the value of the specified axis on the specified joystick for
   *   replaying auton, otherwise just returns the value from the joystick
   *   in opcontrol.
   */

  bool inputGetDigital(unsigned char joystick, unsigned char buttonGroup,
    unsigned char button);
  /*
   * Returns the value of the specified button from the specified buttonGroup
   *   on the specified joystick for replaying auton, otherwise just returns
   *   the value from the joystick in opcontrol.
   */

  #define AUTON_REPLAYER_H_

#endif /* AUTONREPLAYER_H_ */
