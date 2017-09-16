#include "VboElement.hpp"

/* --------------------------------- Public --------------------------------- */

/* A VboElement is initialised giving it an array of GLfloats and its size. */
VboElement::VboElement(GLfloat* array, int size) {
  /* The vbo field is initialised and bound as a buffer */
  glGenBuffers(1, &this->vbo);
  glBindBuffer(GL_ARRAY_BUFFER, this->vbo);

  /* The input array is bound to the new vbo buffer for the given size */
  glBufferData(GL_ARRAY_BUFFER, size * 3 * sizeof(GLfloat), array, GL_STATIC_DRAW);

  /* The size is stored as a reference */
  this->vboSize = size;
}

/* VBO Buffer handler getter */
GLuint VboElement::getVbo() {
  return this->vbo;
}

/* Vbo size getter */
int VboElement::getVboSize() {
  return this->vboSize;
}
