#ifndef ARDUINO_UTILS_H
#define ARDUINO_UTILS_H

#include "Util.hpp"
#include "../Geometry/Point.hpp"
#include "../Geometry/ReferencePoint.hpp"
#include "../Geometry/ArduinoPoint.hpp"
#include "../Geometry/DeltaObject.hpp"

/* Arduino utils contains a number of functions to setup and handle arduino
 * communication.
 * Firstly all arduino servo motors are virtualised with Geometry Components.
 * This means that changed wia GLFW to this virtual arduino state will reflect
 * real changes being applied to the actual board.
 * It is important to NOTE that custom tailored values have been introduced to
 * calibrate the program to the BAoBOS arm.
 * Serial communication can also be initialised and handled from this utility.
 * Serial packets are received and dispatched accordingly to the arduino. */

/* Arduino (C++ - sided) component initialisation */
void setupArduino();
shared_ptr<ReferencePoint> determineArduinoDeltas(shared_ptr<Point> point);

/* Arduino setup helper functions */
shared_ptr<DeltaObject> getDeltaZDiff(shared_ptr<DeltaObject>, shared_ptr<DeltaObject>, float, float);

/* Serial port initialisation */
void setupSerial(const char* portname);

/* Serial port initialisation helper funcions */
int set_interface_attribs (int fd, int speed, int parity);
void set_blocking (int fd, int should_block);

/* Serial port communication handlers */
void sendByteData(uint8_t*, double);
uint8_t* readByteData();

#endif
