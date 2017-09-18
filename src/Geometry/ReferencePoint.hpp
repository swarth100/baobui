#ifndef REFERENCE_POINT_H
#define REFERENCE_POINT_H

#include "../Util/Util.hpp"
#include "Point.hpp"
#include "ArduinoPoint.hpp"
#include "DeltaObject.hpp"

class ReferencePoint {
public:
  enum Position {
    HIGHER,
    MIDDLE,
    LOWER
  };

  ReferencePoint::Position position;

  ReferencePoint(shared_ptr<ArduinoPoint> center, ReferencePoint::Position pos);

  void attachPoint(shared_ptr<ArduinoPoint> arduinoPt);

  void getValues(shared_ptr<Point>);

private:
  shared_ptr<ArduinoPoint> center;

  list<shared_ptr<ArduinoPoint>> arduinoPoints;
};

#endif
