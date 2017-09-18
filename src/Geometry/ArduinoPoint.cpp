#include "ArduinoPoint.hpp"

/* --------------------------------- Public --------------------------------- */

/* */
ArduinoPoint::ArduinoPoint(int servo1, int servo2, int servo3, int servo4, ArduinoPoint::Cardinals cardinal) {
  this->servo1 = servo1;
  this->servo2 = servo2;
  this->servo3 = servo3;
  this->servo4 = servo4;

  this->cardinal = cardinal;
}

/* */
void ArduinoPoint::computeDeltas(shared_ptr<ArduinoPoint> centerPtr) {
  this->delta1 = this->servo1 - centerPtr->servo1;
  this->delta2 = this->servo2 - centerPtr->servo2;
  this->delta3 = this->servo3 - centerPtr->servo3;
  this->delta4 = this->servo4 - centerPtr->servo4;
}
