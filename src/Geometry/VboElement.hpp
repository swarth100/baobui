#ifndef VBOELEMENT_H
#define VBOELEMENT_H

#include "../Util/Util.hpp"

/* A VboElement stores buffered VBO array data together with it size.
   VboElements should be accessed whenever they need to be linked into a Vao. */
class VboElement {
public:
  /* Constructor */
  VboElement(GLfloat* array, int size);

  /* Getters for private fields */
  GLuint getVbo();
  int getVboSize();

private:
  /* Private fields */
  GLuint vbo;
  int vboSize;
};

#endif
