#ifndef DELTA_OBJECT_H
#define DELTA_OBJECT_H

#include "Point.hpp"
#include "../Util/Util.hpp"

/* DeltaObject store the delta data for ArduinoPoints.
   Delta data must be retrieved and sent correctly over too arduino */
class DeltaObject{
public:
  /* Constructor */
  DeltaObject(int, int, int, int, bool);

  /* Public methods */
  void setRefPoint(shared_ptr<Point>);
  void removePoint();
  void print();

  /* Public getters */
  int getDelta0();
  int getDelta1();
  int getDelta2();
  int getDelta3();
  int getDelta4();

  /* Public setter */
  void setDelta0(int newDelta);

private:
  /* Private fields */
  int delta0, delta1, delta2, delta3, delta4;

  float refDelta;
  bool isHorizontal;
  bool usePoint = false;

  /* Private methods */
  int getWeightedDelta (int deltaVal);
};

#endif
