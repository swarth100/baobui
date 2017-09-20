#ifndef ARDUINO_UTILS_H
#define ARDUINO_UTILS_H

#include "Util.hpp"
#include "../Geometry/Point.hpp"
#include "../Geometry/ReferencePoint.hpp"
#include "../Geometry/ArduinoPoint.hpp"
#include "../Geometry/DeltaObject.hpp"

void setupArduino();
void setupSerial(const char* portname);

shared_ptr<ReferencePoint> determineArduinoDeltas(shared_ptr<Point> point);

shared_ptr<DeltaObject> getDeltaZDiff(shared_ptr<DeltaObject>, shared_ptr<DeltaObject>, float, float);

int set_interface_attribs (int fd, int speed, int parity);
void set_blocking (int fd, int should_block);

void sendByteData(uint8_t*, double);
uint8_t* readByteData();

#endif
