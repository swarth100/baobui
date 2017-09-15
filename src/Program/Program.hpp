#ifndef PROGRAM_H
#define PROGRAM_H

#include "../Util/Util.hpp"
#include "../Component/Component.hpp"

class Program {
public:
  Program(const char* vertex_shader, const char* fragment_shader, shared_ptr<Component> component);

  void attachUniform(const char* name, const GLfloat* value);

  void draw();

protected:

private:
  const char* vertex_shader;
  const char* fragment_shader;

  shared_ptr<Component> component;

  GLuint shader_programme;
  GLuint vs_Ref;
  GLuint fs_Ref;

  void compileAllShaders();
  GLuint compileShader(const char* shader, GLenum shaderType);

  void checkLinkStatus();
  void checkShaderCompilation(GLuint shaderInt);
};

#endif
