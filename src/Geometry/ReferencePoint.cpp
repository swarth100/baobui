#include "ReferencePoint.hpp"
/* --------------------------------- Public --------------------------------- */

/* Constructor initialises arduino point and layer fields */
ReferencePoint::ReferencePoint(shared_ptr<ArduinoPoint> center, ReferencePoint::Position position) {
  this->center = center;
  this->position = position;
}

/* Multiple other arduino points can be attached to the main reference one.
   This enables to attach up to one point per cardinal angle */
void ReferencePoint::attachPoint(shared_ptr<ArduinoPoint> arduinoPt) {
  this->arduinoPoints.push_back(arduinoPt);

  arduinoPt->computeDeltas(this->center);
}

/* Returns the given central point upon which deltas are based */
shared_ptr<ArduinoPoint> ReferencePoint::getInnerArduinoData() {
  return this->center;
}

/* Returns a new deltaObject which expresses the nearest ArduinoPoint Deltas.
   The object is used when calculating the deltas to send over the COM port */
shared_ptr<DeltaObject> ReferencePoint::getValues(shared_ptr<Point> point) {
  int tmpDelta1 = 0;
  int tmpDelta2 = 0;
  int tmpDelta3 = 0;
  int tmpDelta4 = 0;

  /* We iterate through the list of held arduinoPoints to find the one which
     suits best the given point */
  list<shared_ptr<ArduinoPoint>>::iterator it;
  for (it = this->arduinoPoints.begin(); it != this->arduinoPoints.end(); ++it){
		shared_ptr<ArduinoPoint> arduinoPt = (*it);

    /* Switch through all cases which would make a given point viable for
       selection at this stage */
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

    /* If we select the given point we will retrieve the delta data */
    if (increaseDelta) {
      shared_ptr<DeltaObject> delta = arduinoPt->delta;

      /* We set the given point as reference to calculate the deltas from */
      delta->setRefPoint(point);

      tmpDelta1 += delta->getDelta1();
      tmpDelta2 += delta->getDelta2();
      tmpDelta3 += delta->getDelta3();
      tmpDelta4 += delta->getDelta4();

      /* Remove the delta reference point restoring the delta object's state */
  		delta->removePoint();
    }
  }

  return make_shared<DeltaObject>(tmpDelta1, tmpDelta2, tmpDelta3, tmpDelta4, true);
}
