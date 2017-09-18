#ifndef POINT_H
#define POINT_H

#include "../Util/Util.hpp"

class Point {
public:
  /* Public fields */
  float x, y, z;

  /* Constructor */
  Point (float x, float y, float z);

  void print();
};

#endif
