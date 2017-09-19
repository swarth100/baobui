#ifndef CAMERA_UTILS_H
#define CAMERA_UTILS_H

#include "Util.hpp"
#include "../Math/maths_funcs.h"
#include "../Geometry/Point.hpp"

bool update_camera(GLFWwindow* g_window, double elapsed_seconds);
void init_camera(float, float, float, float);

shared_ptr<Point> getCamera();

mat4 getTranslationMatrix();

/* Helper functions */
bool updateCameraPos(int index, bool increase, float elapsed_seconds);

#endif
