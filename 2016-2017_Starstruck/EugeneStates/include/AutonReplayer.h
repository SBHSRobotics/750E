/*
 *  AutonReplayer.h
 *
 *  Created on: Feb 17, 2017
 *      Author: Mikel Matticoli
 */

#ifndef AUTONREPLAYER_H_

  void startAuton(int s);

  void stopAuton();

  int inputGetAnalog(unsigned char joystick, unsigned char axis);

  bool inputGetDigital(unsigned char joystick, unsigned char buttonGroup,
    unsigned char button);

  #define AUTON_REPLAYER_H_

#endif /* AUTONREPLAYER_H_ */
