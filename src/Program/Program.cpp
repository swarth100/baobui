#include "Program.hpp"

/* Public */

/* */
Program::Program(const char* vertex_shader, const char* fragment_shader) {
  this->vertex_shader = vertex_shader;
  this->fragment_shader = fragment_shader;
  this->shader_programme = glCreateProgram();

  this->compileAllShaders();

  this->checkLinkStatus();
}

/* */
void Program::attachUniform(const char* name, const GLfloat* value) {
  GLint uniformLoc = glGetUniformLocation(this->shader_programme, name);

  glUseProgram(this->shader_programme);
  glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, value);
}

/* */
void Program::draw() {
  glUseProgram(this->shader_programme);

  /* Iterate through all components within the componentList */
  list<shared_ptr<Component>>::iterator it;
  for (it = componentList.begin(); it != componentList.end(); ++it){
    shared_ptr<Component> component = (*it);

    /* For each component bind the relevant VAO and draw */
    glBindVertexArray(component->getVao());
    // glDrawArrays(component->getType(), 0, component->getSize());
    glDrawArrays(component->getType(), 0, component->getSize());
  }
}

/* */
void Program::generatePrism() {
  shared_ptr<Component> component = make_shared<Prism>(2, 2, 2, make_shared<Point>(0, 0, 0));

  componentList.push_back(component);
}

void Program::generateLine() {
  for (int i = -100; i <= 100; i ++) {
    shared_ptr<Component> component = make_shared<Line>(make_shared<Point>(-100, i, 0), make_shared<Point>(100, i, 0));

    componentList.push_back(component);

    shared_ptr<Component> component2 = make_shared<Line>(make_shared<Point>(i, -100, 0), make_shared<Point>(i, 100, 0));

    componentList.push_back(component2);
  }
}

/* */
void Program::generateColouredPrism() {

  GLfloat colours[] = { 1.0f, 0.0f, 0.0f,
		                    0.0f, 1.0f, 0.0f,
												0.0f, 0.0f, 1.0f,
												1.0f, 0.0f, 0.0f,
												0.0f, 1.0f, 0.0f,
												0.0f, 0.0f, 1.0f,
												1.0f, 0.0f, 0.0f,
												0.0f, 1.0f, 0.0f,
												0.0f, 0.0f, 1.0f,
												1.0f, 0.0f, 0.0f,
												0.0f, 1.0f, 0.0f,
												0.0f, 0.0f, 1.0f,
												1.0f, 0.0f, 0.0f,
												0.0f, 1.0f, 0.0f,
												0.0f, 0.0f, 1.0f,
												1.0f, 0.0f, 0.0f,
												0.0f, 1.0f, 0.0f,
												0.0f, 0.0f, 1.0f,
												1.0f, 0.0f, 0.0f,
		                    0.0f, 1.0f, 0.0f,
												0.0f, 0.0f, 1.0f,
												1.0f, 0.0f, 0.0f,
												0.0f, 1.0f, 0.0f,
												0.0f, 0.0f, 1.0f,
												1.0f, 0.0f, 0.0f,
												0.0f, 1.0f, 0.0f,
												0.0f, 0.0f, 1.0f,
												1.0f, 0.0f, 0.0f,
												0.0f, 1.0f, 0.0f,
												0.0f, 0.0f, 1.0f,
												1.0f, 0.0f, 0.0f,
												0.0f, 1.0f, 0.0f,
												0.0f, 0.0f, 1.0f,
												1.0f, 0.0f, 0.0f,
												0.0f, 1.0f, 0.0f,
												0.0f, 0.0f, 1.0f };

  shared_ptr<Component> component = make_shared<Prism>(2, 2, 2, make_shared<Point>(0, 0, 0));
  component->addVbo3(colours, 36);

  componentList.push_back(component);
}

/* Private */

/* */
void Program::compileAllShaders() {
  this->vs_Ref = this->compileShader(this->vertex_shader, GL_VERTEX_SHADER);
  this->checkShaderCompilation(vs_Ref);

  this->fs_Ref = this->compileShader(this->fragment_shader, GL_FRAGMENT_SHADER);
  this->checkShaderCompilation(fs_Ref);
}

/* */
GLuint Program::compileShader(const char* shader, GLenum shaderType) {
  char shaderBuff[1024 * 256];
	parse_file_into_str(shader, shaderBuff, 1024 * 256 );

  GLuint shaderInt = glCreateShader(shaderType);
	const GLchar *shaderChar = (const GLchar *)shaderBuff;
	glShaderSource( shaderInt, 1, &shaderChar, NULL );
	glCompileShader( shaderInt );

  return shaderInt;
}

/* */
void Program::checkLinkStatus() {
  glAttachShader(this->shader_programme, this->fs_Ref);
  glAttachShader(this->shader_programme, this->vs_Ref);

  glLinkProgram(this->shader_programme);

  int debugParams = -1;
  glGetProgramiv( this->shader_programme, GL_LINK_STATUS, &debugParams );
	if ( GL_TRUE != debugParams ) {
		fprintf( stderr, "ERROR: could not link shader programme GL index %i\n",
						 this->shader_programme );
		print_programme_info_log( this->shader_programme );
		exit(-1);
	}
}

/* */
void Program::checkShaderCompilation(GLuint shaderInt) {
  int debugParams = -1;
	glGetShaderiv( shaderInt, GL_COMPILE_STATUS, &debugParams );
	if ( GL_TRUE != debugParams ) {
		fprintf( stderr, "ERROR: GL shader index %i did not compile\n", shaderInt );
		print_shader_info_log( shaderInt );
		exit(-1); // or exit or something
	}
}
