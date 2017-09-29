#ifndef CAMERA_UTILS_H
#define CAMERA_UTILS_H

#include "Util.hpp"
#include "../Math/maths_funcs.h"
#include "../Geometry/Point.hpp"

/* Camera utils contains a number of helper functions to handle the position of
 * objects dependant on the camera.
 * The camera itself can be initialised and updated via KEY_PRESS events.
 * Bound to the camera's position is the companionCube. Based on the dragging of
 * the extension, it's position is accordingly updated. Minor physics is also
 * virtualised, making the companion cube glide down to its position should it
 * be hovering over the ground.
 * Ultimately camera_utils can export translation and projection matrices to
 * correctly render the various components onto the screen's view. */

/* Camera update functions */
bool update_camera(GLFWwindow* g_window, double elapsed_seconds);
void init_camera(float, float, float, float);
bool updateCameraPos(int index, bool increase, float elapsed_seconds);

/* Camera getters and setters */
shared_ptr<Point> getCamera();
shared_ptr<Point> getCubePos(float);

/* CompanionCube helper functions */
void setCubePos(shared_ptr<Point>);

/* Matrix calculations */
mat4 getTranslationMatrix();
mat4 getProjectionMatrix();

/* Extension servoMotors handling and helper functions */
int getExtensionAngle();
void moveExtension(bool isExtending, float elapsed_seconds);

#endif
