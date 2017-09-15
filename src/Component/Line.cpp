#include "Line.hpp"

/* Public */

/* */
Line::Line(shared_ptr<Point> start, shared_ptr<Point> end) {
  this->start = start;
  this->end = end;
}

/* Private */

/* */
void Line::getLine() {
  GLfloat points[] = {
    start->x, start->y, start->z,
      end->x,   end->y,   end->z,
  };

  this->Component::addVbo2(points, 1);
}
