#ifndef LINE_H
#define LINE_H

#include "Component.hpp"
#include "../Util/Util.hpp"
#include "../Geometry/Point.hpp"

/* A Line is a subClass of a Component */
class Line : public Component {
public:
  /* Constructor */
  Line(shared_ptr<Point> start, shared_ptr<Point> end);

private:
  /* Private fields */
  shared_ptr<Point> start, end;

  /* Helper generation method */
  void generateLine();
};

#endif
