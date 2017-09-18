#ifndef ARDUINO_UTILS_H
#define ARDUINO_UTILS_H

#include "Util.hpp"
#include "../Geometry/Point.hpp"
#include "../Geometry/ReferencePoint.hpp"
#include "../Geometry/ArduinoPoint.hpp"
#include "../Geometry/DeltaObject.hpp"

void setupArduino();

shared_ptr<ReferencePoint> determineArduinoDeltas(shared_ptr<Point> point);

shared_ptr<DeltaObject> getDeltaZDiff(shared_ptr<DeltaObject>, shared_ptr<DeltaObject>, float, float);

#endif
