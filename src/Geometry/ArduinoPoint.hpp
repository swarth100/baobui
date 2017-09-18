#ifndef ARDUINO_POINT_H
#define ARDUINO_POINT_H

#include "../Util/Util.hpp"
#include "DeltaObject.hpp"

class ArduinoPoint {
public:
  enum Cardinals {
    CENTER,
    TOP,
    RIGHT,
    BOT,
    LEFT
  };

  int servo1, servo2, servo3, servo4;
  shared_ptr<DeltaObject> delta;

  ArduinoPoint::Cardinals cardinal;

  ArduinoPoint(int, int, int, int, ArduinoPoint::Cardinals);

  void computeDeltas(shared_ptr<ArduinoPoint>);
};

#endif
