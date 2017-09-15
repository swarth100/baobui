#include "Prism.hpp"

/* Public Methods */

Prism::Prism() : Component() {

  this->getPrism(0, 0, 0, make_shared<Point>(0, 0, 0));
}

Prism::Prism(float width, float height, float depth, shared_ptr<Point> center) : Component() {
  this->width = width;
  this->height = height;
  this->depth = depth;
  this->center = center;

  this->getPrism(width, height, depth, center);
}

/* Protected Methods */

/* Private Methods */
void Prism::getPrism(float width, float height, float depth, shared_ptr<Point> center) {

  GLfloat points[] = {
    0.0f, 0.0f, 0.0f, // triangle 1 : begin
    0.0f, 0.0f, depth,
    0.0f, height, depth, // triangle 1 : end
    width, height, 0.0f, // triangle 2 : begin
    0.0f, 0.0f, 0.0f,
    0.0f, height, 0.0f, // triangle 2 : end
    width, 0.0f, depth,
    0.0f, 0.0f, 0.0f,
    width, 0.0f, 0.0f,
    width, height, 0.0f,
    width, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,
    0.0f, height, depth,
    0.0f, height, 0.0f,
    width, 0.0f, depth,
    0.0f, 0.0f, depth,
    0.0f, 0.0f, 0.0f,
    0.0f, height, depth,
    0.0f, 0.0f, depth,
    width, 0.0f, depth,
    width, height, depth,
    width, 0.0f, 0.0f,
    width, height, 0.0f,
    width, 0.0f, 0.0f,
    width, height, depth,
    width, 0.0f, depth,
    width, height, depth,
    width, height, 0.0f,
    0.0f, height, 0.0f,
    width, height, depth,
    0.0f, height, 0.0f,
    0.0f, height, depth,
    width, height, depth,
    0.0f, height, depth,
    width, 0.0f, depth
  };

  this->Component::addVbo3(points, 36);
}
