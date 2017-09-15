#ifndef COMPONENT_H
#define COMPONENT_H

#include "../Util/Util.hpp"
#include "../Geometry/VboElement.hpp"

class Component {
public:
  Component();

  GLuint getVao ();
  int getSize();

  void addVbo(GLfloat* array, int size);

protected:
  int index = 0;
  int size;

  GLuint vao;

  // void addVbo(GLfloat* array, int size);
};

#endif
