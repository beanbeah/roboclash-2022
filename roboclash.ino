#include "pins.hpp"
#include "baseLib.hpp"
#include "controller.hpp"

/**
 * TODO:
 * Add in FreeRTOS
 * Create sensor Task
 * Create Base Movement Task
 * Create Mech Control Task
 * 
 * Under Loop: 
 * when auton turned off, Suspend sensor and base movement task
 * when auton triggered, check for Task Status, start if not running
 */

/**
 * Initialisation
 */
#define maxSpeed 250
byte interruptPin = 3;
byte channelAmount = 8;
#define rightStickChannel 1
#define leftStickChannel 2
PPMReader ppm(interruptPin, channelAmount);

/**
 * Motor Config
 * PWM, FWD, BWD, reversed, maxSpeed
 */
Motor FR = {PWM_FR,FWD_FR,BWD_FR,false,maxSpeed};
Motor FL = {PWM_FL,FWD_FL,BWD_FL,false,maxSpeed};
Motor ML = {PWM_ML,FWD_ML,BWD_ML,false,maxSpeed};
Motor MR = {PWM_MR,FWD_MR,BWD_MR,false,maxSpeed};
Motor BR = {PWM_BR,FWD_BR,BWD_BR,false,maxSpeed};
Motor BL = {PWM_BL,FWD_BL,BWD_BL,false,maxSpeed};

void setup() {
  Serial.begin(115200);
  setupMotor(FR);
  setupMotor(FL);
  setupMotor(BR);
  setupMotor(BL);
  setupMotor(ML);
  setupMotor(MR);
  Serial.print("Motors Initialised");
  Serial.print("Testing Auton");
  timerBase(100,100,2500);
  Serial.print("Exiting Setup");
}

void loop() {
  //quick driver control test
  int x = controllerMap(readChannel(rightStickChannel,ppm));
  int y = controllerMap(readChannel(leftStickChannel,ppm));
  moveMotor(FL, y+x);
  moveMotor(ML, y+x);
  moveMotor(BL, y+x);
  moveMotor(FR, y-x);
  moveMotor(MR, y-x);
  moveMotor(BR, y-x);
  delay(5);  
}
