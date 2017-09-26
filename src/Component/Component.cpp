#include "Component.hpp"

/* --------------------------------- Public --------------------------------- */

/* Component constructor initialises vao field */
Component::Component() {
  glGenVertexArrays(1, &this->vao);
  initTranslationMatrix();
}

/* Getter for VAO */
GLuint Component::getVao() {
  return this->vao;
}

/* */
GLuint Component::getTexture() {
  return this->texture->getTexture(this->textureIndex);
}

/* Getter for size, inherited from VBO */
int Component::getSize() {
  return this->size;
}

/* Getter for type, inherited from VBO */
GLenum Component::getType() {
  return this->type;
}

GLfloat* Component::getDeltaPos() {
  return this->deltapos;
}

int Component::getTextureIndex() {
  return this->textureIndex;
}

/* Adds a type 2 Vbo */
void Component::addVbo2(GLfloat* array, int size, int vecNum) {
  this->addVbo(array, size, 2, vecNum);
}

/* Adds a type 3 Vbo */
void Component::addVbo3(GLfloat* array, int size, int vecNum) {
  this->addVbo(array, size, 3, vecNum);
}

/* */
void Component::addTexture(shared_ptr<Texture> texture) {
	this->texture = texture;
}

/* */
void Component::setTextureIndex(int newIndex) {
  this->textureIndex = newIndex;
}

/* */
void Component::setPos(shared_ptr<Point> newPos) {
  this->deltapos[12] = newPos->x;
  this->deltapos[13] = newPos->y;
  this->deltapos[14] = newPos->z;
}

/* */
void Component::addSubComponent(shared_ptr<Component> newSub) {
  subComponentList.push_back(newSub);
}

/* */
void Component::updateSubCompTexture(int newTextureIndex) {
  this->setTextureIndex(newTextureIndex);

  list<shared_ptr<Component>>::iterator it;
  for (it = subComponentList.begin(); it != subComponentList.end(); ++it){
    shared_ptr<Component> component = (*it);

    component->setTextureIndex(newTextureIndex);
  }
}

/* -------------------------------- Private --------------------------------- */

/* Initialises and adds a new VboElement */
void Component::addVbo(GLfloat* array, int size, int VboType, int vecNum) {
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
	glVertexAttribPointer(this->index, vecNum, GL_FLOAT, GL_FALSE, 0, NULL);

  /* The specific Vao index must be enabled after creation */
	glEnableVertexAttribArray(this->index);

  /* Increase the Vao index for the next Vbo */
  this->index = this->index + 1;
}

/* */
void Component::initTranslationMatrix() {
  GLfloat transMatrix[] = {
    1.0f, 0.0f, 0.0f, 0.0f, // first column
    0.0f, 1.0f, 0.0f, 0.0f, // second column
    0.0f, 0.0f, 1.0f, 0.0f, // third column
    0.0f, 0.0f, 0.0f, 1.0f	// fourth column
  };

  GLfloat* returnArr;

  returnArr = (GLfloat*) malloc(sizeof(transMatrix) * sizeof(GLfloat));
  memcpy(returnArr, transMatrix, sizeof(transMatrix));

  this->deltapos = returnArr;
}
