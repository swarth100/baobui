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

GLenum Component::getType() {
  return this->type;
}

/* */
void Component::addVbo2(GLfloat* array, int size) {
  this->addVbo(array, size, 2);
}

/* */
void Component::addVbo3(GLfloat* array, int size) {
  this->addVbo(array, size, 3);
}

/* Private */

/* */
void Component::addVbo(GLfloat* array, int size, int VboType) {
  shared_ptr<VboElement> vboElem = make_shared<VboElement>(array, size);

  /* */
  switch (VboType) {
    case 2:
      this->type = GL_LINES;
      break;
    case 3:
      this->type = GL_TRIANGLES;
      break;
    default:
      printf("Invalid VboType of %i\n", VboType);
      exit(-1);
      return;
  }

  this->size = max(this->size, size);

	glBindVertexArray(this->vao);
	glBindBuffer(GL_ARRAY_BUFFER, vboElem->getVbo());
	glVertexAttribPointer(this->index, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(this->index);

  this->index = this->index + 1;
}
