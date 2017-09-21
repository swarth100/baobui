#ifndef PROGRAM_H
#define PROGRAM_H

#include "../Util/Util.hpp"

#include "../Component/Component.hpp"
#include "../Component/Prism.hpp"
#include "../Component/Line.hpp"

#define MAX_GRID_ELEMENTS 100

class Program {
public:
  /* Constructor */
  Program(const char* vertex_shader, const char* fragment_shader);

  /* Program Uniform handlers */
  void attachUniform(const char* name, const GLfloat* value);

  /* Program Component draw */
  void draw();

  /* Program Component generation */
  void generatePrism(float, float, float, shared_ptr<Point> center);
  void generateTexturedPrism(float, float, float, shared_ptr<Point>, GLuint);
  void generateLine(shared_ptr<Point> start, shared_ptr<Point> end);
  void generateGrid(float maxItem, float separator);

  GLuint shader_programme;

private:
  /* Private fields */
  const char* vertex_shader;
  const char* fragment_shader;

  list<shared_ptr<Component>> componentList;

  GLuint vs_Ref;
  GLuint fs_Ref;

  /* Shader handlers */
  void compileAllShaders();
  GLuint compileShader(const char* shader, GLenum shaderType);

  /* Compile checkers */
  void checkLinkStatus();
  void checkShaderCompilation(GLuint shaderInt);
};

#endif
