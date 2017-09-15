#ifndef PROGRAM_H
#define PROGRAM_H

#include "../Util/Util.hpp"

class Program {
public:
  Program(const char* vertex_shader, const char* fragment_shader);

  void attachUniform(const char* name, const GLfloat* value);

protected:

private:
  const char* vertex_shader;
  const char* fragment_shader;

  GLuint shader_programme;
  GLuint vs_Ref;
  GLuint fs_Ref;

  void compileAllShaders();
  GLuint compileShader(const char* shader, GLenum shaderType);

  void checkLinkStatus();
  void checkShaderCompilation(GLuint shaderInt);
};

#endif
