#ifndef PROGRAM_UTILS_H
#define PROGRAM_UTILS_H

#include "Util.hpp"
#include "../Program/Program.hpp"

shared_ptr<Program> initProgram(const char* vs, const char* fs);

void attachUniforms(const char* name, GLfloat* uniform);

void drawAllPrograms();

#endif
