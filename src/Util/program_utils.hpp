#ifndef PROGRAM_UTILS_H
#define PROGRAM_UTILS_H

#include "Util.hpp"
#include "../Util/camera_utils.hpp"
#include "../Program/Program.hpp"
#include "../Component/Component.hpp"
#include "../Component/Line.hpp"
#include "../Component/Prism.hpp"
#include "../Component/Texture.hpp"

shared_ptr<Program> initProgram(const char* vs, const char* fs);

void attachUniforms(const char* name, GLfloat* uniform);

void drawAllPrograms();

void updateButtonData(uint8_t* data);
void updateCompanionCube(float);

void generateModels();

void updateButtons(shared_ptr<Component>, int);

#endif
