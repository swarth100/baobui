#ifndef PRISM_H
#define PRISM_H

#include "Component.hpp"
#include "../Util/Util.hpp"
#include "../Geometry/Point.hpp"

/* A Prism is a subClass of a Component */
class Prism : public Component {
public:
  /* Constructor */
  Prism(float width, float height, float depth, shared_ptr<Point> center);

private:
  /* Private fields */
  float width, height, depth;

  shared_ptr<Point> center;

  /* Private generation helper function */
  void generatePrism(float, float, float, shared_ptr<Point>);
};

GLfloat* getExtendedPrismTexture(float, float, float);

#endif
