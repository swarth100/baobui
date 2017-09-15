#ifndef COMPONENT_H
#define COMPONENT_H

#include "../Util/Util.hpp"
#include "../Geometry/VboElement.hpp"

class Component {
public:
  GLuint getVao ();

  void addVbo(GLfloat* array, int size);

protected:
  int index = 0;

  GLuint vao;

  // void addVbo(GLfloat* array, int size);
};

#endif
