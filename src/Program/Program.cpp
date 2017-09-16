#include "Program.hpp"

/* --------------------------------- Public --------------------------------- */

/* Program constructor
   Given a vertex_shader file path and a fragment_shader filepath it compiles
   and links the shaders to the new program */
Program::Program(const char* vertex_shader, const char* fragment_shader) {
  this->vertex_shader = vertex_shader;
  this->fragment_shader = fragment_shader;

  /* A new GLprogram instance is created and stored */
  this->shader_programme = glCreateProgram();

  /* Shaders are compiled individually */
  this->compileAllShaders();

  /* Shaders are linked to the program and link status is checked */
  this->checkLinkStatus();
}

/* Given a uniform's name and a pointer to its new value, this method attaches
   a new (or existent) uniform to the program.
   Must be called should the uniform be updated. */
void Program::attachUniform(const char* name, const GLfloat* value) {
  GLint uniformLoc = glGetUniformLocation(this->shader_programme, name);

  /* Must make sure the current prograam is in use when attaching a new unif */
  glUseProgram(this->shader_programme);
  glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, value);
}

/* Draw iterates through the list of all components held by a given program,
   drawing them one by one */
void Program::draw() {
  /* Must make sure the given GLprogram is in use */
  glUseProgram(this->shader_programme);

  /* Iterate through all components within the componentList */
  list<shared_ptr<Component>>::iterator it;
  for (it = componentList.begin(); it != componentList.end(); ++it){
    shared_ptr<Component> component = (*it);

    /* For each component bind the relevant VAO and draw */
    glBindVertexArray(component->getVao());
    glDrawArrays(component->getType(), 0, component->getSize());
  }
}

/* Generates a new prism of given dimensions and center */
void Program::generatePrism(float width, float height, float depth, shared_ptr<Point> center) {
  /* Generate a new Prism Component with the given arguments */
  shared_ptr<Component> component = make_shared<Prism>(width, height, depth, center);

  /* Add the newly created prism to the list of components held by the Program */
  componentList.push_back(component);
}

// NOTE: Will be deprecated in further versions
/* Generates a new prism which accepts a secondary uniform for colour */
void Program::generateColouredPrism(float width, float height, float depth, shared_ptr<Point> center) {

  // TODO: Refactor color generation
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

  shared_ptr<Component> component = make_shared<Prism>(width, height, depth, center);
  component->addVbo3(colours, 36);

  componentList.push_back(component);
}

/* Given a start and ending Point, it generates a new Line Component adding
   it to the componentList */
void Program::generateLine(shared_ptr<Point> start, shared_ptr<Point> end) {
  shared_ptr<Component> component = make_shared<Line>(start, end);

  componentList.push_back(component);
}

/* Generates a new grid of given max Size with each element separated by a given
   separator */
void Program::generateGrid(float maxItem, float separator) {
  for (float i = -maxItem; i <= maxItem; i += separator) {
    /* Generates the new Y-Axis line */
    this->generateLine(make_shared<Point>(-maxItem, i, 0), make_shared<Point>(maxItem, i, 0));

    /* Generates the new X-Axis line */
    this->generateLine(make_shared<Point>(i, -maxItem, 0), make_shared<Point>(i, maxItem, 0));
  }
}

/* -------------------------------- Private --------------------------------- */

/* Compiles the Vertex and Fragment shaders checking if it were successful */
void Program::compileAllShaders() {
  this->vs_Ref = this->compileShader(this->vertex_shader, GL_VERTEX_SHADER);
  this->checkShaderCompilation(vs_Ref);

  this->fs_Ref = this->compileShader(this->fragment_shader, GL_FRAGMENT_SHADER);
  this->checkShaderCompilation(fs_Ref);
}

/* Compiles shaders and returns a reference Int */
GLuint Program::compileShader(const char* shader, GLenum shaderType) {
  char shaderBuff[1024 * 256];
  /* Load shader from file into buffer */
	parse_file_into_str(shader, shaderBuff, 1024 * 256 );

  /* Creates a new shader object and attaches the source */
  GLuint shaderInt = glCreateShader(shaderType);
	const GLchar *shaderChar = (const GLchar *)shaderBuff;
	glShaderSource(shaderInt, 1, &shaderChar, NULL);

  /* Attempts to compile the shader and returns a handler for the object */
	glCompileShader(shaderInt);
  return shaderInt;
}

/* Attaches and Links a Program's shaders. Checks if process was successful */
void Program::checkLinkStatus() {
  glAttachShader(this->shader_programme, this->fs_Ref);
  glAttachShader(this->shader_programme, this->vs_Ref);

  glLinkProgram(this->shader_programme);

  /* Check if the linking was successful to the program.
     Should it be unsuccessful it prints to stout and log file, then exits */
  int debugParams = -1;
  glGetProgramiv( this->shader_programme, GL_LINK_STATUS, &debugParams );
	if ( GL_TRUE != debugParams ) {
		fprintf( stderr, "ERROR: could not link shader programme GL index %i\n",
						 this->shader_programme );
		print_programme_info_log( this->shader_programme );
		exit(-1);
	}
}

/* Checks for successful shader compilation */
void Program::checkShaderCompilation(GLuint shaderInt) {
  /* Check if the shader compilation was successful.
     Should it be unsuccessful it prints to stout and log file, then exits */
  int debugParams = -1;
	glGetShaderiv( shaderInt, GL_COMPILE_STATUS, &debugParams );
	if ( GL_TRUE != debugParams ) {
		fprintf( stderr, "ERROR: GL shader index %i did not compile\n", shaderInt );
		print_shader_info_log( shaderInt );
		exit(-1); // or exit or something
	}
}
