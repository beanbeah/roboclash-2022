#include <Servo.h>
#include <IBusBM.h>
#include "pins.h"

////////////////////////////////////////////MOTORS/////////////////////////////////////
class Motor {
  private:
    int PWM, FWD, BWD;
    bool reversed;
    int maxSpeed;

  public:
    Motor(int PWM, int FWD, int BWD, bool reversed, int maxSpeed){
      this->PWM = PWM;
      this->FWD = FWD;
      this->BWD = BWD;
      this->reversed = reversed;
      this->maxSpeed = maxSpeed;
    }

    void init(){
      pinMode(this->PWM,OUTPUT);
      pinMode(this->FWD,OUTPUT);
      pinMode(this->BWD,OUTPUT);
    }

    void move(int speed){
      if (speed >= this->maxSpeed) speed = this->maxSpeed;
      if (speed <= -this->maxSpeed) speed = -this->maxSpeed;
      analogWrite(this->PWM, abs(speed)); 
      if (this->reversed) {
        digitalWrite(this->FWD, speed < 0);
        digitalWrite(this->BWD, speed > 0);
      } else {
        digitalWrite(this->FWD, speed > 0);
        digitalWrite(this->BWD, speed < 0);
      }
    }
};

////////////////////////////////////////////CONTROLS/////////////////////////////////////
IBusBM ibusRc;
HardwareSerial& ibusRcSerial = Serial1;

void setup_ibus() {
  ibusRc.begin(ibusRcSerial);
}

int readChannel(int channelNumber) {
  return ibusRc.readChannel(channelNumber);
}

int readSwitch(int channelNumber) {
  int value = ibusRc.readChannel(channelNumber);
  if (value == 1000) return 1;
  else if (value == 2000) return 0;  
  return -1; //refers to err
}

int controllerMap(int value, int maxSpeed) {
  if (value >= 1000 && value < 1450) {
    value = map(value,1000,1450,-maxSpeed,-50);
  } else if (value <= 2000 && value > 1550) {
    value = map(value,1550,2000,50,maxSpeed);
  } else {
    //assume stick is stationary
    value = 0;
  }
  return value;
}

////////////////////////////////////////////BASE LIB/////////////////////////////////////
int motorMaxSpeed = 250; //motor Max Speed
Motor FL (PWM_FL, FWD_FL, BWD_FL, false, motorMaxSpeed);
Motor FR (PWM_FR, FWD_FR, BWD_FR, true, motorMaxSpeed);
Motor BL (PWM_BL, FWD_BL, BWD_BL, true, motorMaxSpeed);
Motor BR (PWM_BR, FWD_BR, BWD_BR, true, motorMaxSpeed);
Motor Roller_L (PWM_ROLLER_L, FWD_ROLLER_L, BWD_ROLLER_L, false, motorMaxSpeed);
Motor Roller_R (PWM_ROLLER_R, FWD_ROLLER_R, BWD_ROLLER_R, true, motorMaxSpeed);

void setup_motors()
{
  FL.init();
  BL.init();
  FR.init();
  BR.init();
  Roller_R.init();
  Roller_L.init();
}

void powerBase(double l, double r) {
  FL.move(l);
  FR.move(r);
  BL.move(l);
  BR.move(r);
}

void roller(double power) {
  Roller_L.move(power);
  Roller_R.move(power);
}

////////////////////////////////////////////MAIN/////////////////////////////////////
Servo indexer;
bool state = false;

void setup() {
  Serial.begin(115200);
  setup_motors();
  setup_ibus();
  indexer.attach(27);
  indexer.write(180);
}

void loop() {
  //note Ibus Lib reads switches from 0 - 9. Thus offset.
  int y = controllerMap(readChannel(1),250);
  int x = controllerMap(readChannel(3),250);
  int servo = readSwitch(4);
  int auton = readSwitch(6);
  int intake_dir = readSwitch(7);
  int intake = readSwitch(5);
  bool rollerSwitch = readChannel(8) == 2000 ? true : false;

  if (auton == 0) { 
    if (!state) {
      roller(-250);
      delay(1000);
      roller(0);
      powerBase(0,0);
      state = true;
    }
    
    //end auton
    FL.move(0);
    FR.move(0);
    BL.move(0);
    BR.move(0);
    roller(0);
    
  } else if (auton == 1) {
   //driver control  
   FL.move(y+x);
   FR.move(y-x);
   BL.move(y+x);
   BR.move(y-x);

   if (intake == 1){
    if (intake_dir == 1) roller(250);
    else if (intake_dir == 0) roller(-180);
    else if (intake_dir == -1) roller(0);
   } else {
    roller(0);
   }
   
   if (servo) {
    indexer.write(0);
   } else {
    indexer.write(180);
   }
   
  } 


  delay(2); 
}
