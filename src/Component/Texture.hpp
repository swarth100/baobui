#ifndef TEXTURE_H
#define TEXTURE_H

#include "../Util/Util.hpp"

class Texture {
public:
  Texture();

  void addTexture(const char* path);

  GLuint getTexture(int index);

private:
  list<GLuint> textureList;

};

#endif
