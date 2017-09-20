#include "DeltaObject.hpp"

/* --------------------------------- Public --------------------------------- */

/* A delta object is initialised with given maximum deltas and a horizontal
   status */
DeltaObject::DeltaObject(int delta1, int delta2, int delta3, int delta4, bool isHor) {
  this->delta1 = delta1;
  this->delta2 = delta2;
  this->delta3 = delta3;
  this->delta4 = delta4;

  this->isHorizontal = isHor;
}

/* When a reference point is set, future measurements of the reference obj's
   delta fields will take into account the point's ratio */
void DeltaObject::setRefPoint(shared_ptr<Point> refPt) {
  if (isHorizontal) {
    this->refDelta = refPt->x / 10;
  } else {
    this->refDelta = refPt->y / 10;
  }

  /* Absolute value is ambiguous with floats */
  if (this->refDelta < 0) {
    this->refDelta = this->refDelta * -1;
  }

  /* Set the reference point flag */
  this->usePoint = true;
}

/* Clears the refference point flag */
void DeltaObject::removePoint() {
  this->usePoint = false;
}

/* Retrieves the delta1 field */
int DeltaObject::getDelta0() {
  return this->delta0;
}

/* Retrieves the delta1 field */
int DeltaObject::getDelta1() {
  return getWeightedDelta(this->delta1);
}

/* Retrieves the delta2 field */
int DeltaObject::getDelta2() {
  return getWeightedDelta(this->delta2);
}

/* Retrieves the delta3 field */
int DeltaObject::getDelta3() {
  return getWeightedDelta(this->delta3);
}

/* Retrieves the delta4 field */
int DeltaObject::getDelta4() {
  return getWeightedDelta(this->delta4);
}

void DeltaObject::setDelta0(int newDelta) {
  this->delta0 = newDelta;
}

/* Prints out the given delta object in readable form */
void DeltaObject::print() {
  printf("Deltas, 0: %i, 1:%i, 2:%i, 3:%i, 4:%i\n", delta0, delta1, delta2, delta3, delta4);
}

/* -------------------------------- Private --------------------------------- */

/* Helper method to take into account of refference points when retrieving
   delta measures */
int DeltaObject::getWeightedDelta (int deltaVal) {
  if (this->usePoint) {
    return deltaVal * this->refDelta;
  }
  return deltaVal;
}
