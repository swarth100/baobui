#include "Component.hpp"

/* --------------------------------- Public --------------------------------- */

/* Component constructor initialises vao field */
Component::Component() {
  glGenVertexArrays(1, &this->vao);
}

/* Getter for VAO */
GLuint Component::getVao() {
  return this->vao;
}

/* Getter for size, inherited from VBO */
int Component::getSize() {
  return this->size;
}

/* Getter for type, inherited from VBO */
GLenum Component::getType() {
  return this->type;
}

/* Adds a type 2 Vbo */
void Component::addVbo2(GLfloat* array, int size) {
  this->addVbo(array, size, 2);
}

/* Adds a type 3 Vbo */
void Component::addVbo3(GLfloat* array, int size) {
  this->addVbo(array, size, 3);
}

/* -------------------------------- Private --------------------------------- */

/* Initialises and adds a new VboElement */
void Component::addVbo(GLfloat* array, int size, int VboType) {
  /* VboElement is initialised */
  shared_ptr<VboElement> vboElem = make_shared<VboElement>(array, size);

  /* Vbo type is switched upon */
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

  /* Size is updated to always be the maximum size of the input Vbos */
  this->size = max(this->size, size);

  /* The new Vbo is added and bound to the Vao */
	glBindVertexArray(this->vao);
	glBindBuffer(GL_ARRAY_BUFFER, vboElem->getVbo());

  /* Note: 3 in the number of dimensions present in each point of the Vao */
	glVertexAttribPointer(this->index, 3, GL_FLOAT, GL_FALSE, 0, NULL);

  /* The specific Vao index must be enabled after creation */
	glEnableVertexAttribArray(this->index);

  /* Increase the Vao index for the next Vbo */
  this->index = this->index + 1;
}
