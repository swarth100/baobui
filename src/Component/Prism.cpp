#include "Prism.hpp"

/* --------------------------------- Public --------------------------------- */

/* Prism constructor.
   Each line is generated given a its three dimensions and a center */
Prism::Prism(float width, float height, float depth, shared_ptr<Point> center) : Component() {
  this->width = width;
  this->height = height;
  this->depth = depth;
  this->center = center;

  this->generatePrism(width, height, depth, center);
}

/* -------------------------------- Private --------------------------------- */

/* Generates a new Prism Component and adds it to the super class's Vao */
void Prism::generatePrism(float width, float height, float depth, shared_ptr<Point> center) {

  GLfloat points[] = {
    0.0f, 0.0f, 0.0f,    /* Trinagle 1 start */
    0.0f, 0.0f, depth,
    0.0f, height, depth, /* Triangle 1 end */
    width, height, 0.0f, /* Triangle 2 start */
    0.0f, 0.0f, 0.0f,
    0.0f, height, 0.0f,  /* Triangle 2 end */
    width, 0.0f, depth,  /* ... */
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

  /* */
  float dx = center->x - (width / 2);
  float dy = center->y - (height / 2);
  float dz = center->z - (depth / 2);

  int pointNum = sizeof(points) / sizeof(float);

  for (int i = 0; i < pointNum; i += 3) {
    points[i] += dx;
    points[i+1] += dy;
    points[i+2] += dz;
  }

  /* Ensure the new prism is added to the Component's Vao */
  this->Component::addVbo3(points, 36);
}
