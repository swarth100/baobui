#ifndef COMPONENT_H
#define COMPONENT_H

#include "../Util/Util.hpp"
#include "../Geometry/VboElement.hpp"
#include "../Geometry/Point.hpp"
#include "Texture.hpp"

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
  GLfloat* getDeltaPos();

  /* Methods to add a new Vbo to the system */
  void addVbo2(GLfloat* array, int size, int vecNum);
  void addVbo3(GLfloat* array, int size, int vecNum);

  void addTexture(shared_ptr<Texture>);
  void setTextureIndex(int);
  void setPos(shared_ptr<Point>);

  void addSubComponent(shared_ptr<Component>);
  void updateSubCompTexture(int);

protected:
  /* Protected fields */
  int index = 0;
  int size = 0;

  int textureIndex = 0;

  GLuint vao;
  GLfloat* deltapos;
  shared_ptr<Texture> texture;
  GLenum type;

  list<shared_ptr<Component>> subComponentList;

private:
  /* Private Vbo helper method */
  void addVbo(GLfloat* array, int size, int type, int vecNum);

  void initTranslationMatrix();
};

#endif
