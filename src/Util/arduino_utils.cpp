#include "arduino_utils.hpp"

shared_ptr<ReferencePoint> lowerLevel;
shared_ptr<ReferencePoint> middleLevel;
shared_ptr<ReferencePoint> upperLevel;

void setupArduino() {
  /* Bottom Level */
	shared_ptr<ArduinoPoint> lowerCenter = make_shared<ArduinoPoint>(95, 80, 55, 45, ArduinoPoint::CENTER);
	lowerLevel = make_shared<ReferencePoint>(lowerCenter, ReferencePoint::LOWER);

	lowerLevel->attachPoint(make_shared<ArduinoPoint>(70, 10, 140, 55, ArduinoPoint::TOP));
	lowerLevel->attachPoint(make_shared<ArduinoPoint>(90, 65, 90, -10, ArduinoPoint::RIGHT));
	lowerLevel->attachPoint(make_shared<ArduinoPoint>(100, 140, 40, 45, ArduinoPoint::BOT));
	lowerLevel->attachPoint(make_shared<ArduinoPoint>(95, 70, 90, 100, ArduinoPoint::LEFT));

	/* Middle level */
	shared_ptr<ArduinoPoint> middleCenter = make_shared<ArduinoPoint>(70, 90, 20, 45, ArduinoPoint::CENTER);
	middleLevel = make_shared<ReferencePoint>(middleCenter, ReferencePoint::MIDDLE);

	middleLevel->attachPoint(make_shared<ArduinoPoint>(10, 10, 50, 55, ArduinoPoint::TOP));
	middleLevel->attachPoint(make_shared<ArduinoPoint>(75, 70, 35, -10, ArduinoPoint::RIGHT));
	middleLevel->attachPoint(make_shared<ArduinoPoint>(90, 140, 20, 45, ArduinoPoint::BOT));
	middleLevel->attachPoint(make_shared<ArduinoPoint>(75, 75, 35, 100, ArduinoPoint::LEFT));

	/* Upper level */
	shared_ptr<ArduinoPoint> upperCenter = make_shared<ArduinoPoint>(50, 100, 0, 45, ArduinoPoint::CENTER);
	upperLevel = make_shared<ReferencePoint>(upperCenter, ReferencePoint::HIGHER);

	upperLevel->attachPoint(make_shared<ArduinoPoint>(0, 10, 30, 55, ArduinoPoint::TOP));
	upperLevel->attachPoint(make_shared<ArduinoPoint>(60, 70, 20, -10, ArduinoPoint::RIGHT));
	upperLevel->attachPoint(make_shared<ArduinoPoint>(90, 140, -20, 45, ArduinoPoint::BOT));
	upperLevel->attachPoint(make_shared<ArduinoPoint>(60, 70, 20, 100, ArduinoPoint::LEFT));
}

/* */
shared_ptr<ReferencePoint> determineArduinoDeltas(shared_ptr<Point> point) {
  shared_ptr<DeltaObject> retValue;

  shared_ptr<ReferencePoint> lowerRef;
  shared_ptr<ReferencePoint> upperRef;

  /* */
  if (point->z <= 10) {
    lowerRef = lowerLevel;
    upperRef = middleLevel;
  } else {
    lowerRef = middleLevel;
    upperRef = upperLevel;
  }

  shared_ptr<DeltaObject> lowerDelta = lowerRef->getValues(point);
  shared_ptr<DeltaObject> upperDelta = upperRef->getValues(point);

  float ZRatio = fmod(point->z, 5);
  float reverseRatio = (5 - ZRatio) / 5;
  ZRatio /= 5;

  shared_ptr<DeltaObject> innerDelta = getDeltaZDiff(lowerDelta, upperDelta, ZRatio, reverseRatio);

  shared_ptr<ArduinoPoint> lowerCenter = lowerRef->getInnerArduinoData();
  shared_ptr<ArduinoPoint> upperCenter = upperRef->getInnerArduinoData();

  int newServo1 = lowerCenter->servo1 * reverseRatio + upperCenter->servo1 * ZRatio;
  int newServo2 = lowerCenter->servo2 * reverseRatio + upperCenter->servo2 * ZRatio;
  int newServo3 = lowerCenter->servo3 * reverseRatio + upperCenter->servo3 * ZRatio;
  int newServo4 = lowerCenter->servo4 * reverseRatio + upperCenter->servo4 * ZRatio;

  shared_ptr<ArduinoPoint> newPosition = make_shared<ArduinoPoint>(newServo1, newServo2, newServo3, newServo4, ArduinoPoint::CENTER);
  newPosition->delta = innerDelta;

  return make_shared<ReferencePoint>(newPosition, ReferencePoint::MIDDLE);
}

/* */
shared_ptr<DeltaObject> getDeltaZDiff(shared_ptr<DeltaObject> lowerDelta, shared_ptr<DeltaObject> upperDelta, float ZRatio, float reverseRatio) {
  int newDelta1 = lowerDelta->getDelta1() * reverseRatio + upperDelta->getDelta1() * ZRatio;
  int newDelta2 = lowerDelta->getDelta2() * reverseRatio + upperDelta->getDelta2() * ZRatio;
  int newDelta3 = lowerDelta->getDelta3() * reverseRatio + upperDelta->getDelta3() * ZRatio;
  int newDelta4 = lowerDelta->getDelta4() * reverseRatio + upperDelta->getDelta4() * ZRatio;

  return make_shared<DeltaObject>(newDelta1, newDelta2, newDelta3, newDelta4, true);
}
