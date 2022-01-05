#include "controller.hpp"

int readChannel (int channelNumber, PPMReader ppm) {
  unsigned value = ppm.latestValidChannelValue(channelNumber, 0);
  return value;
}

bool readButton (int channelNumber, PPMReader ppm) {
  bool state = ppm.latestValidChannelValue(channelNumber, 0);
  return state;
}

int controllerMap(int value) {
  //map value from -255 to 255
  if (value >= 1000 && value < 1450) {
    value = map(value,1000,1450,-255,0);
  } else if (value <= 2000 && value > 1550) {
    value = map(value,1550,2000,0,255);
  } else {
    //assume stick is stationary
    value = 0;
  }
  return value;
}
