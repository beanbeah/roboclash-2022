#include "Arduino.h"
#include <PPMReader.h>
#ifndef _CONTROLLER_HPP_
#define _CONTROLLER_HPP_

int readChannel (int channelNumber,PPMReader ppm);
bool readButton (int channelNumber, PPMReader ppm);
int controllerMap(int value);

#endif
