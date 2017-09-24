#include "Point.hpp"

/* --------------------------------- Public --------------------------------- */

/* A point holds a reference to a triplet of coordinates */
Point::Point(float x, float y, float z) {
  this->x = x;
  this->y = y;
  this->z = z;
}

void Point::print() {
  printf("X: %f, Y: %f, Z: %f\n", x, y ,z);
}
