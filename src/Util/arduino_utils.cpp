#include "arduino_utils.hpp"

list<shared_ptr<ReferencePoint>> arduinoReferenceList;

void setupArduino() {
  /* Bottom Level */
	shared_ptr<ArduinoPoint> lowerCenter = make_shared<ArduinoPoint>(95, 80, 55, 45, ArduinoPoint::CENTER);
	shared_ptr<ReferencePoint> lowerLevel = make_shared<ReferencePoint>(lowerCenter, ReferencePoint::LOWER);

	lowerLevel->attachPoint(make_shared<ArduinoPoint>(70, 10, 140, 55, ArduinoPoint::TOP));
	lowerLevel->attachPoint(make_shared<ArduinoPoint>(90, 65, 90, -10, ArduinoPoint::RIGHT));
	lowerLevel->attachPoint(make_shared<ArduinoPoint>(100, 140, 40, 45, ArduinoPoint::BOT));
	lowerLevel->attachPoint(make_shared<ArduinoPoint>(95, 70, 90, 100, ArduinoPoint::LEFT));

	arduinoReferenceList.push_back(lowerLevel);

	/* Middle level */
	shared_ptr<ArduinoPoint> middleCenter = make_shared<ArduinoPoint>(70, 90, 20, 45, ArduinoPoint::CENTER);
	shared_ptr<ReferencePoint> middleLevel = make_shared<ReferencePoint>(middleCenter, ReferencePoint::LOWER);

	middleLevel->attachPoint(make_shared<ArduinoPoint>(10, 10, 50, 55, ArduinoPoint::TOP));
	middleLevel->attachPoint(make_shared<ArduinoPoint>(75, 70, 35, -10, ArduinoPoint::RIGHT));
	middleLevel->attachPoint(make_shared<ArduinoPoint>(90, 140, 20, 45, ArduinoPoint::BOT));
	middleLevel->attachPoint(make_shared<ArduinoPoint>(75, 75, 35, 100, ArduinoPoint::LEFT));

	arduinoReferenceList.push_back(middleLevel);

	/* Upper level */
	shared_ptr<ArduinoPoint> upperCenter = make_shared<ArduinoPoint>(50, 100, 0, 45, ArduinoPoint::CENTER);
	shared_ptr<ReferencePoint> upperLevel = make_shared<ReferencePoint>(upperCenter, ReferencePoint::LOWER);

	upperLevel->attachPoint(make_shared<ArduinoPoint>(0, 10, 30, 55, ArduinoPoint::TOP));
	upperLevel->attachPoint(make_shared<ArduinoPoint>(60, 70, 20, -10, ArduinoPoint::RIGHT));
	upperLevel->attachPoint(make_shared<ArduinoPoint>(90, 140, -20, 45, ArduinoPoint::BOT));
	upperLevel->attachPoint(make_shared<ArduinoPoint>(60, 70, 20, 100, ArduinoPoint::LEFT));

	lowerLevel->getValues(make_shared<Point>(-5, -5, 0));
	//middleLevel->getValues(make_shared<Point>(0, 0, 0));
	//upperLevel->getValues(make_shared<Point>(0, 0, 0));

	arduinoReferenceList.push_back(upperLevel);
}
