#include <PPMReader.h>
#include "pins.h"

int maxSpeed = 250;
//baseline lowest 5 speed is 50
byte interruptPin = 3;
byte channelAmount = 8;
PPMReader ppm(interruptPin, channelAmount);

struct Motor {
  const int PWM;
  const int FWD;
  const int BWD;
  const bool reversed;
  const int maxSpeed;
};

int readChannel (int channelNumber, PPMReader ppm) {
  unsigned value = ppm.latestValidChannelValue(channelNumber, 0);
  return value;
}

int controllerMap(int value) {
  //map value from -255 to 255
  if (value >= 1000 && value < 1450) {
    value = map(value,1000,1450,-255,-50);
  } else if (value <= 2000 && value > 1550) {
    value = map(value,1550,2000,50,255);
  } else {
    //assume stick is stationary
    value = 0;
  }
  return value;
}

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

void setup() {
  Serial.begin(115200);
  Serial.print("Exiting Setup");
}

void loop() {
  Motor FL = {PWM_FL,FWD_FL,BWD_FL,true,maxSpeed}; 
  Motor FR = {PWM_FR,FWD_FR,BWD_FR,false,maxSpeed};
  Motor ML = {PWM_ML,FWD_ML,BWD_ML,true,maxSpeed};
  Motor MR = {PWM_MR,FWD_MR,BWD_MR,false,maxSpeed};
  Motor BR = {PWM_BR,FWD_BR,BWD_BR,false,maxSpeed};
  Motor BL = {PWM_BL,FWD_BL,BWD_BL,false,maxSpeed};
  Motor RollerL = {PWM_ROLLER_L, FWD_ROLLER_L, BWD_ROLLER_L, false, maxSpeed};
  Motor RollerR = {PWM_ROLLER_R, FWD_ROLLER_R, BWD_ROLLER_R, true, maxSpeed};
  Motor Intake = {PWM_INTAKE,FWD_INTAKE,BWD_INTAKE,true,maxSpeed};
  Motor Index = {PWM_INDEX, FWD_INDEX, BWD_INDEX, true, maxSpeed};
  
  setupMotor(FR);
  setupMotor(FL);
  setupMotor(BR);
  setupMotor(BL);
  setupMotor(ML);
  setupMotor(MR);
  setupMotor(RollerL);
  setupMotor(RollerR);
  setupMotor(Intake);
  setupMotor(Index);
  moveMotor(Intake,250);
  moveMotor(Index,250);
  moveMotor(RollerL,250);
  moveMotor(RollerR,250);
  moveMotor(FL,250);
  moveMotor(FR,250);
  moveMotor(ML,250);
  moveMotor(MR,250); 
  moveMotor(BL,250);
  moveMotor(BR,250);
  delay(5);  
}
