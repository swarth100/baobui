#include "Component.hpp"

/* Public */

/* */
Component::Component() {
  glGenVertexArrays(1, &this->vao);
}

GLuint Component::getVao() {
  return this->vao;
}

int Component::getSize() {
  return this->size;
}

/* Protected */

/* */
void Component::addVbo(GLfloat* array, int size) {
  shared_ptr<VboElement> vboElem = make_shared<VboElement>(array, size);

  this->size = max(this->size, size);

	glBindVertexArray(this->vao);
	glBindBuffer(GL_ARRAY_BUFFER, vboElem->getVbo());
	glVertexAttribPointer(this->index, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(this->index);

  this->index = this->index + 1;
}
