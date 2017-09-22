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
int g_gl_width = 640;
int g_gl_height = 480;

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

	/* Initialise Program for textured Objects */
	shared_ptr<Program> texturedProgram = initProgram("assets/test_vs.glsl", "assets/test2_fs.glsl");

	shared_ptr<Texture> cubeTexture = make_shared<Texture>();
	cubeTexture->addTexture("assets/img/companionCube.png");

	// GLuint cubeTex;
	// load_texture("assets/img/companionCube.png", &cubeTex);
	//
	// GLuint wallTex0;
	// load_texture("assets/img/grid0.png", &wallTex0);
	//
	// GLuint wallTex0b;
	// load_texture("assets/img/grid0Gray.png", &wallTex0b);
	//
	// GLuint wallTex0Blue;
	// load_texture("assets/img/grid0Blue.png", &wallTex0Blue);
	//
	// GLuint wallTex0Orange;
	// load_texture("assets/img/grid0Orange.png", &wallTex0Orange);
	//
	// GLuint wallTex3;
	// load_texture("assets/img/grid3.png", &wallTex3);
	//
	// GLuint wallTex3b;
	// load_texture("assets/img/grid3b.png", &wallTex3b);
	//
	// GLuint signalLightBlue;
	// load_texture("assets/img/portalSignalLightBlue.png", &signalLightBlue);
	//
	// GLuint signalLightOrange;
	// load_texture("assets/img/portalSignalLightOrange.png", &signalLightOrange);
	//
	// GLuint companionCubeHug;
	// load_texture("assets/img/Companion-Cube-portal-2.jpg", &companionCubeHug);
	//
	// GLuint cakeIcon;
	// load_texture("assets/img/Chamber_icon_cake_on.png", &cakeIcon);
	//
	//
	// /* */
	// texturedProgram->generateTexturedPrism(10, 10, 50, make_shared<Point>(10, 0, -25), wallTex3, 5);
	// texturedProgram->generateTexturedPrism(5, 5, 2, make_shared<Point>(10, 0, -0.25f), wallTex0b, 1.25f);
	//
	// /* */
	// texturedProgram->generateTexturedPrism(10, 10, 50, make_shared<Point>(0, 10, -25), wallTex3, 5);
	// texturedProgram->generateTexturedPrism(5, 5, 2, make_shared<Point>(0, 10, -0.25f), wallTex0b, 1.25f);
	//
	// texturedProgram->generateTexturedPrism(1.25f, 1.25f, 0, make_shared<Point>(-3.13f, 6.85f, 0.01f), companionCubeHug, 0);
	// texturedProgram->generateTexturedPrism(1.25f, 1.25f, 0, make_shared<Point>(3.13f, 6.85f, 0.01f), cakeIcon, 0);
	//
	// texturedProgram->generateTexturedPrism(0.5f, 0.5f, 0, make_shared<Point>(-0.6f, 6.875f, 0.01f), signalLightBlue, 0);
	// texturedProgram->generateTexturedPrism(0.5f, 0.5f, 0, make_shared<Point>(-0.6f, 5.625f, 0.01f), signalLightBlue, 0);
	//
	// for (float i = 0; i >= -15; i-= 1.25f) {
	// 	texturedProgram->generateTexturedPrism(0.5f, 0, 0.5f, make_shared<Point>(-0.6f, 4.99f,  i - (1.25f/2)), signalLightBlue, 0);
	// 	}
	//
	// /* */
	// texturedProgram->generateTexturedPrism(10, 10, 50, make_shared<Point>(-10, 0, -25), wallTex3, 5);
	// texturedProgram->generateTexturedPrism(5, 5, 2, make_shared<Point>(-10, 0, -0.25f), wallTex0b, 1.25f);
	//
	// /* */
	// texturedProgram->generateTexturedPrism(10, 10, 50, make_shared<Point>(0, -10, -25), wallTex3, 5);
	// texturedProgram->generateTexturedPrism(5, 5, 2, make_shared<Point>(0, -10, -0.25f), wallTex0b, 1.25f);
	//
	// /* */
	// texturedProgram->generateTexturedPrism(200, 200, 0, make_shared<Point>(0, 0, -15), wallTex3b, 1);

	texturedProgram->generateTexturedPrism(2, 2, 2, make_shared<Point>(0, 10, 2), cubeTexture, 0);

	/* Initialise Program for blank objects. Templates and/or Grid */
	shared_ptr<Program> untexturedProgram = initProgram("assets/test2_vs.glsl", "assets/test_fs.glsl");

	//untexturedProgram->generateGrid(100, 1.0f);

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

	//glEnable( GL_CULL_FACE ); // cull face
	//glCullFace( GL_BACK );		// cull back face
	//glFrontFace( GL_CW );			// GL_CCW for counter clock-wise

	/* Enable depth test */
	glEnable(GL_DEPTH_TEST);
	/* Accept fragment if it closer to the camera than the former one */
	glDepthFunc(GL_LESS);

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
		if (readData != NULL)
			free (readData);
	}

	/* Close GL context and any other GLFW resources */
	glfwTerminate();
	return 0;
}
