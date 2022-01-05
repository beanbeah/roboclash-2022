#include "baseLib.hpp"

void setupMotor (Motor motor) {
  pinMode(motor.FWD, OUTPUT);
  pinMode(motor.BWD, OUTPUT);
  pinMode(motor.PWM, OUTPUT);
}

//note speed is from -255 to 255
void moveMotor (Motor motor, int speed) {
  if (speed >= motor.maxSpeed)speed=motor.maxSpeed;
  if (speed <= -motor.maxSpeed)speed=-motor.maxSpeed;
  if (motor.reversed) {
    digitalWrite(motor.FWD, speed < 0);
    digitalWrite(motor.BWD, speed > 0);
  } else {
    digitalWrite(motor.FWD, speed > 0);
    digitalWrite(motor.BWD, speed < 0);
  }
  analogWrite(motor.PWM, abs(speed));
}

void powerBase(int l, int r) {
  moveMotor(FL, l);
  moveMotor(ML, l);
  moveMotor(BL, l);
  moveMotor(FR, r);
  moveMotor(MR, r);
  moveMotor(BR, r);
}

void timerBase(int l, int r, double t) {
  powerBase(l,r);
  delay(t);
  powerBase(0,0);
}
