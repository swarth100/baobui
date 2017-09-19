#ifndef REFERENCE_POINT_H
#define REFERENCE_POINT_H

#include "../Util/Util.hpp"
#include "Point.hpp"
#include "ArduinoPoint.hpp"
#include "DeltaObject.hpp"

/* A ReferencePoint holds a reference to an arduino configuration given a
   central point and a layer.
   It mainly stores ArduinoPoints */
class ReferencePoint {
public:
  /* Enum to store the ReferencePoint's position layer */
  enum Position {
    HIGHER,
    MIDDLE,
    LOWER
  };

  /* Public fields */
  ReferencePoint::Position position;

  /* Constructor */
  ReferencePoint(shared_ptr<ArduinoPoint> center, ReferencePoint::Position pos);

  /* Public methods */
  void attachPoint(shared_ptr<ArduinoPoint> arduinoPt);

  shared_ptr<DeltaObject> getValues(shared_ptr<Point>);

  shared_ptr<ArduinoPoint> getInnerArduinoData();

private:
  /* Private fields */
  shared_ptr<ArduinoPoint> center;

  list<shared_ptr<ArduinoPoint>> arduinoPoints;
};

#endif
