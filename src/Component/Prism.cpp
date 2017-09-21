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
    width, 0.0f, depth,  /* 3 */
    0.0f, 0.0f, 0.0f,
    width, 0.0f, 0.0f,
    width, height, 0.0f, /* 4 */
    width, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, /* 5 */
    0.0f, height, depth,
    0.0f, height, 0.0f,
    width, 0.0f, depth, /* 6 */
    0.0f, 0.0f, depth,
    0.0f, 0.0f, 0.0f,
    0.0f, height, depth, /* 7 */
    0.0f, 0.0f, depth,
    width, 0.0f, depth,
    width, height, depth, /* 8 */
    width, 0.0f, 0.0f,
    width, height, 0.0f,
    width, 0.0f, 0.0f, /* 9 */
    width, height, depth,
    width, 0.0f, depth,
    width, height, depth, /* 10 */
    width, height, 0.0f,
    0.0f, height, 0.0f,
    width, height, depth, /* 11 */
    0.0f, height, 0.0f,
    0.0f, height, depth,
    width, height, depth, /* 12 */
    0.0f, height, depth,
    width, 0.0f, depth
  };

  /* Add delta calculation based on the three dimensions of the cube */
  float dx = center->x - (width / 2);
  float dy = center->y - (height / 2);
  float dz = center->z - (depth / 2);

  /* Retrieve the number of elements stored into the array of points */
  int pointNum = sizeof(points) / sizeof(float);

  /* For each point scale the dimensions accordingly to the deltas */
  for (int i = 0; i < pointNum; i += 3) {
    points[i] += dx;
    points[i+1] += dy;
    points[i+2] += dz;
  }

  /* Ensure the new prism is added to the Component's Vao */
  this->Component::addVbo3(points, 36, 3);
}

GLfloat* getExtendedPrismTexture(float width, float height, float depth) {
  /* */
  GLfloat texcoords[]  = {
		0.0f, 0.0f,
		depth, 0.0f,
		depth, height, // ok 1
	  0.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 1.0f, // ok 2
		1.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f, // ok 3
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f, // ok 4
		0.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f, // ok 5
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f, // ok 6
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f, // ok 7
		1.0f, 1.0f,
		0.0f, 0.0f,
		0.0f, 1.0f, // ok 8
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 0.0f, // ok 9
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f, // ok 10
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 0.0f, // ok 11
		1.0f, 1.0f,
		0.0f, 1.0f,
		1.0f, 0.0f, // ok 12
	};

  GLfloat* returnArr;

  returnArr = (GLfloat*) malloc(36 * 2 * sizeof(GLfloat));
  memcpy(returnArr, texcoords, sizeof(texcoords));

  return returnArr;
}
