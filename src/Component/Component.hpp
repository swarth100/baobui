#ifndef COMPONENT_H
#define COMPONENT_H

#include "../Util/Util.hpp"
#include "../Geometry/VboElement.hpp"

class Component {
public:
  Component();

  GLuint getVao ();
  int getSize();
  GLenum getType();

  void addVbo2(GLfloat* array, int size);
  void addVbo3(GLfloat* array, int size);

protected:
  int index = 0;
  int size = 0;

  GLuint vao;
  GLenum type;

private:
  void addVbo(GLfloat* array, int size, int type);
};

#endif
