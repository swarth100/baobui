#include "Component.hpp"

/* Public */
GLuint Component::getVao() {
  return this->vao;
}

/* Protected */

/* */
void Component::addVbo(GLfloat* array, int size) {
  shared_ptr<VboElement> vboElem = make_shared<VboElement>(array, size);

  glGenVertexArrays(1, &this->vao);
	glBindVertexArray(this->vao);
	glBindBuffer(GL_ARRAY_BUFFER, vboElem->getVbo());
	glVertexAttribPointer(this->index, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(this->index);

  this->index = this->index + 1;
}
