#include "ReferencePoint.hpp"
/* --------------------------------- Public --------------------------------- */

/* */
ReferencePoint::ReferencePoint(shared_ptr<ArduinoPoint> center, ReferencePoint::Position position) {
  this->center = center;
  this->position = position;
}

void ReferencePoint::attachPoint(shared_ptr<ArduinoPoint> arduinoPt) {
  this->arduinoPoints.push_back(arduinoPt);

  arduinoPt->computeDeltas(this->center);
}

void ReferencePoint::getValues(shared_ptr<Point>) {
  list<shared_ptr<ArduinoPoint>>::iterator it;
  for (it = this->arduinoPoints.begin(); it != this->arduinoPoints.end(); ++it){
		shared_ptr<ArduinoPoint> arduinoPt = (*it);

		printf("1: %i, 2: %i, 3: %i, 4: %i\n", arduinoPt->delta1, arduinoPt->delta2, arduinoPt->delta3, arduinoPt->delta4);
	}
}
