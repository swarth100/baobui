#ifndef PROGRAM_H
#define PROGRAM_H

#include "../Util/Util.hpp"
#include "../Component/Component.hpp"
#include "../Component/Prism.hpp"
#include "../Component/Line.hpp"

class Program {
public:
  Program(const char* vertex_shader, const char* fragment_shader);

  void attachUniform(const char* name, const GLfloat* value);

  void draw();

  void generatePrism();
  void generateColouredPrism();

  void generateLine();

protected:

private:
  const char* vertex_shader;
  const char* fragment_shader;

  list<shared_ptr<Component>> componentList;

  GLuint shader_programme;
  GLuint vs_Ref;
  GLuint fs_Ref;

  void compileAllShaders();
  GLuint compileShader(const char* shader, GLenum shaderType);

  void checkLinkStatus();
  void checkShaderCompilation(GLuint shaderInt);
};

#endif
