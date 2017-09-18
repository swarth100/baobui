#ifndef REFERENCE_POINT_H
#define REFERENCE_POINT_H

#include "../Util/Util.hpp"
#include "Point.hpp"
#include "ArduinoPoint.hpp"

class ReferencePoint {
public:
  ReferencePoint(shared_ptr<ArduinoPoint> center);

  void attachPoint(shared_ptr<ArduinoPoint> arduinoPt);

  void getValues(shared_ptr<Point>);

private:
  shared_ptr<ArduinoPoint> center;

  list<shared_ptr<ArduinoPoint>> arduinoPoints;
};

#endif
