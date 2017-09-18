#include "ReferencePoint.hpp"
/* --------------------------------- Public --------------------------------- */

/* */
ReferencePoint::ReferencePoint(shared_ptr<ArduinoPoint> center) {
  this->center = center;
}

void ReferencePoint::attachPoint(shared_ptr<ArduinoPoint> arduinoPt) {
  this->arduinoPoints.push_back(arduinoPt);
}

void ReferencePoint::getValues(shared_ptr<Point>) {
  list<shared_ptr<ArduinoPoint>>::iterator it;
  for (it = this->arduinoPoints.begin(); it != this->arduinoPoints.end(); ++it){
		shared_ptr<ArduinoPoint> arduinoPt = (*it);

		printf("1: %i, 2: %i, 3: %i, 4: %i\n", arduinoPt->servo1, arduinoPt->servo2, arduinoPt->servo3, arduinoPt->servo4);
	}
}
