/* Main.cpp */

#include "Util/Util.hpp"
#include "Util/program_utils.hpp"
#include "Util/arduino_utils.hpp"
#include "Util/camera_utils.hpp"
#include "Math/maths_funcs.h"
#include "Component/Component.hpp"
#include "Component/Line.hpp"
#include "Component/Prism.hpp"
#include "Component/Texture.hpp"
#include "Geometry/Point.hpp"
#include "Geometry/ArduinoPoint.hpp"
#include "Geometry/ReferencePoint.hpp"
#include "Program/Program.hpp"

/* Logfile */
#define GL_LOG_FILE "gl.log"

using namespace std;

/* Keeps track of window size for things like the viewport and the mouse cursor */
int g_gl_width = 1280;
int g_gl_height = 960;

/* Window instance */
GLFWwindow *g_window = NULL;

int main() {
	restart_gl_log();

	/* Start GL context */
	start_gl();

	/* Setup arduino */
	setupArduino();

	/* Setup Serial port communication */
	setupSerial("/dev/ttyACM0");

	/* Set up the models in the virtual layout */
	generateModels();

	/* Initialise the camera instance.
	   Holds an instance of the view matrix. */
	init_camera(0.0f, 0.0f, 10.0f, 2.0f);

	/* Retrieve the translation matrix from the initialised camera */
	mat4 view_mat = getTranslationMatrix();

	/* Initialise and retrieve the projection matrix */
	mat4 proj_mat = getProjectionMatrix();

	/* Attach the newly created uniforms to all programs */
	attachUniforms("view", view_mat.m);
	attachUniforms("proj", proj_mat.m);
	attachUniforms("bob", view_mat.m);

	//glEnable( GL_CULL_FACE ); // cull face
	//glCullFace( GL_BACK );		// cull back face
	//glFrontFace( GL_CW );			// GL_CCW for counter clock-wise

	/* Enable depth test */
	glEnable(GL_DEPTH_TEST);
	/* Accept fragment if it closer to the camera than the former one */
	glDepthFunc(GL_LESS);

	sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
  sf::CircleShape shape(100.f);
  shape.setFillColor(sf::Color::Green);

  while (window.isOpen())
  {
      sf::Event event;
      while (window.pollEvent(event))
      {
          if (event.type == sf::Event::Closed)
              window.close();
      }

      window.clear();
      window.draw(shape);
      window.display();
  }

	/* Rendering loop: while window isn't closed */
	while (!glfwWindowShouldClose(g_window)) {

		/* Update time fields */
		static double previous_seconds = glfwGetTime();
		double current_seconds = glfwGetTime();
		double elapsed_seconds = current_seconds - previous_seconds;
		previous_seconds = current_seconds;

		_update_fps_counter(g_window);

		/* Wipe the drawing surface clear */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, g_gl_width, g_gl_height);

		/* Iterate through all programs calling draw() on their components */
		drawAllPrograms();

		// update other events like input handling
		glfwPollEvents();

		/* Update the camera based on Key Press events */
		bool cam_moved = update_camera(g_window, elapsed_seconds);
		updateCompanionCube(elapsed_seconds);

		/* Update view matrix */
		if (cam_moved) {
			/* Retrieve the new translation Matrix given the new Camera position */
			mat4 view_mat = getTranslationMatrix();

			/* Update the uniforms attached to the various programs */
			attachUniforms("view", view_mat.m);

			/* Retrieve the CameraPosition's Point */
			shared_ptr<Point> cameraPosition = getCamera();
			cameraPosition->print();

			/* Use the Camera position to compute the servo motor deltas */
			shared_ptr<ReferencePoint> refPos = determineArduinoDeltas(cameraPosition);
			shared_ptr<ArduinoPoint> refArduino = refPos->getInnerArduinoData();

			/* Set the extension angle (i.e. servo motor 0).
			   Must be set separately as it's independent from the camera position */
			refArduino->setExtensionAngle(getExtensionAngle());

			/* Send the current buffer data information to arduino */
			sendByteData(refArduino->createBuffer(), current_seconds);
		}

		/* Trap ESC Key press */
		if ( GLFW_PRESS == glfwGetKey(g_window, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(g_window, 1);
		}

		/* Show the drawn Components onto the Display */
		glfwSwapBuffers(g_window);

		/* Read the data sent over by arduino */
		uint8_t* readData = readByteData();
		updateButtonData(readData);
	}

	/* Close GL context and any other GLFW resources */
	glfwTerminate();
	return 0;
}
