#ifndef PRISM_H
#define PRISM_H

#include "Component.hpp"
#include "../Util/Util.hpp"
#include "../Geometry/Point.hpp"

/* A Prism is a subClass of a Component */
class Prism : public Component {
public:
  Prism();
  Prism(float width, float height, float depth, shared_ptr<Point> center);

private:
  /* Every Prism must be defined according to its 3D dimensions */
  float width, height, depth;

  /* A Point object indicates the Prism's center */
  shared_ptr<Point> center;

  void getPrism(float, float, float, shared_ptr<Point>);
};

#endif
