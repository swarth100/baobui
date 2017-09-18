#include "DeltaObject.hpp"

DeltaObject::DeltaObject(int delta1, int delta2, int delta3, int delta4, bool isHor) {
  this->delta1 = delta1;
  this->delta2 = delta2;
  this->delta3 = delta3;
  this->delta4 = delta4;

  this->isHorizontal = isHor;
}

void DeltaObject::setRefPoint(shared_ptr<Point> refPt) {
  if (isHorizontal) {
    this->refDelta = refPt->x / 10;
  } else {
    this->refDelta = refPt->y / 10;
  }

  if (this->refDelta < 0) {
    this->refDelta = this->refDelta * -1;
  }

  this->usePoint = true;
}

void DeltaObject::removePoint() {
  this->usePoint = false;
}

int DeltaObject::getDelta1() {
  return getWeightedDelta(this->delta1);
}

int DeltaObject::getDelta2() {
  return getWeightedDelta(this->delta2);
}

int DeltaObject::getDelta3() {
  return getWeightedDelta(this->delta3);
}

int DeltaObject::getDelta4() {
  return getWeightedDelta(this->delta4);
}

int DeltaObject::getWeightedDelta (int deltaVal) {
  if (this->usePoint) {
    return deltaVal * this->refDelta;
  }
  return deltaVal;
}
