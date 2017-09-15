#ifndef VBOELEMENT_H
#define VBOELEMENT_H

#include "../Util/Util.hpp"

class VboElement {

public:
  VboElement(GLfloat* array, int size);

  GLuint getVbo();
  int getVboSize();

private:
  GLuint vbo;
  int vboSize;



};

#endif
