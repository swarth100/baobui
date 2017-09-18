#ifndef DELTA_OBJECT_H
#define DELTA_OBJECT_H

#include "Point.hpp"
#include "../Util/Util.hpp"

class DeltaObject{
public:
  DeltaObject(int, int, int, int, bool);

  void setRefPoint(shared_ptr<Point>);
  void removePoint();
  void print();

  int getDelta1();
  int getDelta2();
  int getDelta3();
  int getDelta4();

private:
  int delta1, delta2, delta3, delta4;

  float refDelta;
  bool isHorizontal;
  bool usePoint = false;

  int getWeightedDelta (int deltaVal);
};

#endif
