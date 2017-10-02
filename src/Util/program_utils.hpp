#ifndef PROGRAM_UTILS_H
#define PROGRAM_UTILS_H

#include <SFML/Audio.hpp>
#include "Util.hpp"
#include "../Util/camera_utils.hpp"
#include "../Program/Program.hpp"
#include "../Component/Component.hpp"
#include "../Component/Line.hpp"
#include "../Component/Prism.hpp"
#include "../Component/Texture.hpp"

/* Program_utils contains a number of helper functions to handle Programs,
 * graphics and audio.
 * Program_utils must be queried when initialising programs, attaching uniforms
 * to them or issuing draw requests. Program_utils stores the only list of
 * active Program*s.
 * Program_utils includes stb_image.h and is thus able to load textures and
 * render them onto the screen. Due to this it also keeps track of all Virtual
 * components of the scene, and must be queried when updating the companionCube
 * or during interactions with buttons.
 * Being the only utility to include SFML/Audio.h, audio files must be imported
 * and run fro within this utility.
 */

/* Program initialisation and utility */
shared_ptr<Program> initProgram(const char* vs, const char* fs);
void attachUniforms(const char* name, GLfloat* uniform);
void drawAllPrograms();

/* Button data initialisation and utility */

void updateButtonData(uint8_t* data);
void updateCompanionCube(float);
void updateButtons(shared_ptr<Component>, int);

/* Texturing and Virtual Modelling */
void generateModels();

/* SOund handling */
void initSound();
shared_ptr<sf::Sound> loadSound(const char*, int);

#endif
