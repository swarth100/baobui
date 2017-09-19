#include "ArduinoPoint.hpp"

/* --------------------------------- Public --------------------------------- */

/* Arduino points are initialised with a given servo configuration and a
   Cardinal expressing the point's "position" */
ArduinoPoint::ArduinoPoint(int servo1, int servo2, int servo3, int servo4, ArduinoPoint::Cardinals cardinal) {
  this->servo1 = servo1;
  this->servo2 = servo2;
  this->servo3 = servo3;
  this->servo4 = servo4;

  this->cardinal = cardinal;
}

/* Computes and updates the given delta fields given a reference central point */
void ArduinoPoint::computeDeltas(shared_ptr<ArduinoPoint> centerPtr) {
  int delta1 = this->servo1 - centerPtr->servo1;
  int delta2 = this->servo2 - centerPtr->servo2;
  int delta3 = this->servo3 - centerPtr->servo3;
  int delta4 = this->servo4 - centerPtr->servo4;

  bool isHorizontal;

  /* Uses the cardinal fields to determine the orientation */
  switch (this->cardinal) {
    case RIGHT:
    case LEFT:
      isHorizontal = true;
      break;
    default:
      isHorizontal = false;
      break;
  }

  this->delta = make_shared<DeltaObject>(delta1, delta2, delta3, delta4, isHorizontal);
}

/* Debug print method */
void ArduinoPoint::print() {
  printf("Deltas, 1:%i, 2:%i, 3:%i, 4:%i\n", servo1 + delta->getDelta1(), servo2 + delta->getDelta2(), servo3 + delta->getDelta3(), servo4 + delta->getDelta4());
}

uint8_t* ArduinoPoint::createBuffer() {
  uint8_t* retBuffer = (uint8_t*) malloc(sizeof (uint8_t) * 5);
	retBuffer[0] = 20;
  retBuffer[1] = servo1 + delta->getDelta1();
  retBuffer[2] = servo2 + delta->getDelta2();
  retBuffer[3] = servo3 + delta->getDelta3();
  retBuffer[4] = servo4 + delta->getDelta4();

  return retBuffer;
}
