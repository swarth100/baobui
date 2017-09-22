#include "Texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "../Util/stb_image.hpp"	// Sean Barrett's image loader - http://nothings.org/


Texture::Texture() {

}

void Texture::addTexture(const char* path) {
  GLuint textureHandle;
  load_texture(path, &textureHandle);
  textureList.push_back(textureHandle);
}

GLuint Texture::getTexture(int index) {
  if (textureList.size() > (uint) index)
  {
      list<GLuint>::iterator it = textureList.begin();
      advance(it, index);
      // 'it' points to the element at index 'N'
      return (*it);
  }
  return 0;
}
