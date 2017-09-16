#include "Line.hpp"

/* --------------------------------- Public --------------------------------- */

/* Line constructor.
   Each line is generated given a start and an ending point */
Line::Line(shared_ptr<Point> start, shared_ptr<Point> end) : Component() {
  this->start = start;
  this->end = end;

  this->generateLine();
}

/* -------------------------------- Private --------------------------------- */

/* Generates a new line Component and adds it to the super class's Vao */
void Line::generateLine() {
  GLfloat points[] = {
    start->x, start->y, start->z,
      end->x,   end->y,   end->z,
  };

  /* Ensure the new line is added to the Component's Vao */
  this->Component::addVbo2(points, 2);
}
