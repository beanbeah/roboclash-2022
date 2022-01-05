#include "Arduino.h"
#ifndef _BASE_LIB_HPP_
#define _MECH_LIB_HPP_

struct Motor {
  const int PWM;
  const int FWD;
  const int BWD;
  const bool reversed;
  const int maxSpeed;
};

extern Motor FL,FR,BL,BR,ML,MR;

void setupMotor (Motor motor);
void moveMotor (Motor motor, int speed);
void powerBase(int l, int r);
void timerBase(int l, int r, double t);
#endif
