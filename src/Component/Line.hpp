#ifndef LINE_H
#define LINE_H

#include "Component.hpp"
#include "../Util/Util.hpp"
#include "../Geometry/Point.hpp"

/* A Prism is a subClass of a Component */
class Line : public Component {
public:
  Line(shared_ptr<Point> start, shared_ptr<Point> end);

private:
  /* Every Prism must be defined according to its 3D dimensions */
  shared_ptr<Point> start, end;

  void getLine();
};

#endif
