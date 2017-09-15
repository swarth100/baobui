#include "program_utils.hpp"

/* List of active programs to be rendered */
list<shared_ptr<Program>> programs;

/* */
shared_ptr<Program> initProgram(const char* vs, const char* fs) {
	shared_ptr<Program> program = make_shared<Program>(vs, fs);

	programs.push_back(program);

	return program;
}

/* */
void attachUniforms(const char* name, GLfloat* uniform) {
	list<shared_ptr<Program>>::iterator it;
  for (it = programs.begin(); it != programs.end(); ++it){
		shared_ptr<Program> program = (*it);

		program->attachUniform(name, uniform);
	}
}

/* */
void drawAllPrograms() {
	list<shared_ptr<Program>>::iterator it;
  for (it = programs.begin(); it != programs.end(); ++it){
		shared_ptr<Program> program = (*it);

		program->draw();
	}
}
