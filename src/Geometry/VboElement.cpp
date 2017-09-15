#include "VboElement.hpp"

VboElement::VboElement(GLfloat* array, int size) {
  glGenBuffers(1, &this->vbo);
  glBindBuffer(GL_ARRAY_BUFFER, this->vbo);

  glBufferData(GL_ARRAY_BUFFER, size * 3 * sizeof(GLfloat), array, GL_STATIC_DRAW);

  this->vboSize = size;
}

GLuint VboElement::getVbo() {
  return this->vbo;
}

int VboElement::getVboSize() {
  return this->vboSize;
}
