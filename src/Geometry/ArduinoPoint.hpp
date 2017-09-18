#ifndef ARDUINO_POINT_H
#define ARDUINO_POINT_H

#include "../Util/Util.hpp"

class ArduinoPoint {
public:
  enum Cardinals {
    CENTER,
    NORTH,
    EAST,
    SOUTH,
    WEST
  };

  int servo1, servo2, servo3, servo4;
  int delta1, delta2, delta3, delta4;

  ArduinoPoint::Cardinals cardinal;

  ArduinoPoint(int, int, int, int, ArduinoPoint::Cardinals);

  void computeDeltas(shared_ptr<ArduinoPoint>);
};

#endif
