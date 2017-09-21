#ifndef COMPONENT_H
#define COMPONENT_H

#include "../Util/Util.hpp"
#include "../Geometry/VboElement.hpp"

/* The Component class is a wrapper around Vaos.
   It handles creation of new Vbos ensuring they're loaded correctly into the
   vao for rendering */
class Component {
public:
  /* Constructor */
  Component();

  /* Getters for protected fields */
  GLuint getVao();
  GLuint getTexture();
  int getSize();
  GLenum getType();

  /* Methods to add a new Vbo to the system */
  void addVbo2(GLfloat* array, int size, int vecNum);
  void addVbo3(GLfloat* array, int size, int vecNum);

  void addTexture(GLuint);

protected:
  /* Protected fields */
  int index = 0;
  int size = 0;

  GLuint vao;
  GLuint texture;
  GLenum type;

private:
  /* Private Vbo helper method */
  void addVbo(GLfloat* array, int size, int type, int vecNum);
};

#endif
