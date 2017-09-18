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

shared_ptr<ArduinoPoint> ReferencePoint::getInnerArduinoData() {
  return this->center;
}

shared_ptr<DeltaObject> ReferencePoint::getValues(shared_ptr<Point> point) {
  int tmpDelta1 = 0;
  int tmpDelta2 = 0;
  int tmpDelta3 = 0;
  int tmpDelta4 = 0;

  list<shared_ptr<ArduinoPoint>>::iterator it;
  for (it = this->arduinoPoints.begin(); it != this->arduinoPoints.end(); ++it){
		shared_ptr<ArduinoPoint> arduinoPt = (*it);

    bool increaseDelta = false;

    if (point-> x >= 0 && arduinoPt->cardinal == ArduinoPoint::RIGHT) {
      increaseDelta = true;
    } else if (point-> x < 0 && arduinoPt->cardinal == ArduinoPoint::LEFT) {
      increaseDelta = true;
    } else if (point-> y >= 0 && arduinoPt->cardinal == ArduinoPoint::TOP) {
      increaseDelta = true;
    } else if (point-> y < 0 && arduinoPt->cardinal == ArduinoPoint::BOT) {
      increaseDelta = true;
    }

    if (increaseDelta) {
      shared_ptr<DeltaObject> delta = arduinoPt->delta;

      delta->setRefPoint(point);

      // printf("%i\n", arduinoPt->cardinal);

      tmpDelta1 += delta->getDelta1();
      tmpDelta2 += delta->getDelta2();
      tmpDelta3 += delta->getDelta3();
      tmpDelta4 += delta->getDelta4();

  		// printf("1: %i, 2: %i, 3: %i, 4: %i\n", delta->getDelta1(), delta->getDelta2(), delta->getDelta3(), delta->getDelta4());

      delta->removePoint();
    }
  }

  return make_shared<DeltaObject>(tmpDelta1, tmpDelta2, tmpDelta3, tmpDelta4, true);
}
