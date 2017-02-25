/*
 *  AutonReplayer.h
 *
 *  Created on: Feb 17, 2017
 *      Author: Mikel Matticoli
 */

#ifndef AUTONREPLAYER_H_

// Port definitions
  static const unsigned char LF = 2;
  static const unsigned char LB = 3;
  static const unsigned char RF = 8;
  static const unsigned char RB = 9;

  static const unsigned char AB = 4;
  static const unsigned char CD = 5;
  static const unsigned char E = 7;
  static const unsigned char F = 10;

  static const unsigned char PINCE = 6;

  void startAuton(int slot);

  void stopAuton();

  int inputGetAnalog(unsigned char joystick, unsigned char axis);

  bool inputGetDigital(unsigned char joystick, unsigned char buttonGroup,
    unsigned char button);

  void joystickMap();

  void pince(int speed);

  void lift(int speed);

  #define AUTON_REPLAYER_H_

#endif /* AUTONREPLAYER_H_ */
