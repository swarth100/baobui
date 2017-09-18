#define _USE_MATH_DEFINES
#include <math.h>

#include "Util/Util.hpp"
#include "Util/program_utils.hpp"
#include "Math/maths_funcs.h"
#include "Component/Component.hpp"
#include "Component/Line.hpp"
#include "Component/Prism.hpp"
#include "Geometry/Point.hpp"
#include "Geometry/ArduinoPoint.hpp"
#include "Geometry/ReferencePoint.hpp"
#include "Program/Program.hpp"

#define GL_LOG_FILE "gl.log"

using namespace std;

// keep track of window size for things like the viewport and the mouse cursor
int g_gl_width = 640;
int g_gl_height = 480;
GLFWwindow *g_window = NULL;

list<shared_ptr<ReferencePoint>> arduinoReferenceList;

int main() {
	restart_gl_log();

	/* Start GL context */
	start_gl();

	/* Setup arduino */

	/* Bottom Level */
	shared_ptr<ArduinoPoint> lowerCenter = make_shared<ArduinoPoint>(95, 80, 55, 45, ArduinoPoint::CENTER);
	shared_ptr<ReferencePoint> lowerLevel = make_shared<ReferencePoint>(lowerCenter, ReferencePoint::LOWER);

	lowerLevel->attachPoint(make_shared<ArduinoPoint>(70, 10, 140, 55, ArduinoPoint::TOP));
	lowerLevel->attachPoint(make_shared<ArduinoPoint>(90, 65, 90, -10, ArduinoPoint::RIGHT));
	lowerLevel->attachPoint(make_shared<ArduinoPoint>(100, 140, 40, 45, ArduinoPoint::BOT));
	lowerLevel->attachPoint(make_shared<ArduinoPoint>(95, 70, 90, 100, ArduinoPoint::LEFT));

	arduinoReferenceList.push_back(lowerLevel);

	/* Middle level */
	shared_ptr<ArduinoPoint> middleCenter = make_shared<ArduinoPoint>(70, 90, 20, 45, ArduinoPoint::CENTER);
	shared_ptr<ReferencePoint> middleLevel = make_shared<ReferencePoint>(middleCenter, ReferencePoint::LOWER);

	middleLevel->attachPoint(make_shared<ArduinoPoint>(10, 10, 50, 55, ArduinoPoint::TOP));
	middleLevel->attachPoint(make_shared<ArduinoPoint>(75, 70, 35, -10, ArduinoPoint::RIGHT));
	middleLevel->attachPoint(make_shared<ArduinoPoint>(90, 140, 20, 45, ArduinoPoint::BOT));
	middleLevel->attachPoint(make_shared<ArduinoPoint>(75, 75, 35, 100, ArduinoPoint::LEFT));

	arduinoReferenceList.push_back(middleLevel);

	/* Upper level */
	shared_ptr<ArduinoPoint> upperCenter = make_shared<ArduinoPoint>(50, 100, 0, 45, ArduinoPoint::CENTER);
	shared_ptr<ReferencePoint> upperLevel = make_shared<ReferencePoint>(upperCenter, ReferencePoint::LOWER);

	upperLevel->attachPoint(make_shared<ArduinoPoint>(0, 10, 30, 55, ArduinoPoint::TOP));
	upperLevel->attachPoint(make_shared<ArduinoPoint>(60, 70, 20, -10, ArduinoPoint::RIGHT));
	upperLevel->attachPoint(make_shared<ArduinoPoint>(90, 140, -20, 45, ArduinoPoint::BOT));
	upperLevel->attachPoint(make_shared<ArduinoPoint>(60, 70, 20, 100, ArduinoPoint::LEFT));

	lowerLevel->getValues(make_shared<Point>(5, 5, 0));
	//middleLevel->getValues(make_shared<Point>(0, 0, 0));
	//upperLevel->getValues(make_shared<Point>(0, 0, 0));

	arduinoReferenceList.push_back(upperLevel);



	/* Initialise Program for textured Objects */
	shared_ptr<Program> texturedProgram = initProgram(
		"assets/test_vs.glsl",  "assets/test_fs.glsl");

	texturedProgram->generateColouredPrism(2, 2, 2, make_shared<Point>(0, 0, 0));

	/* Initialise Program for blank objects. Templates and/or Grid */
	shared_ptr<Program> untexturedProgram = initProgram(
		"assets/test2_vs.glsl",  "assets/test_fs.glsl");

	untexturedProgram->generateGrid(100, 1.0f);

	/*--------------------------create camera
	 * matrices----------------------------*/
	/* create PROJECTION MATRIX */
	// input variables
	float near = 0.1f;									// clipping plane
	float far = 100.0f;									// clipping plane
	float fov = 67.0f * ONE_DEG_IN_RAD; // convert 67 degrees to radians
	float aspect = (float)g_gl_width / (float)g_gl_height; // aspect ratio
	// matrix components
	float range = tan( fov * 0.5f ) * near;
	float Sx = ( 2.0f * near ) / ( range * aspect + range * aspect );
	float Sy = near / range;
	float Sz = -( far + near ) / ( far - near );
	float Pz = -( 2.0f * far * near ) / ( far - near );
	GLfloat proj_mat[] = {  Sx, 0.0f, 0.0f, 0.0f,
		                    0.0f,   Sy,	0.0f, 0.0f,
												0.0f, 0.0f,   Sz,-1.0f,
												0.0f, 0.0f,   Pz, 0.0f };

	/* create VIEW MATRIX */
	float cam_speed = 1.0f;			 // 1 unit per second
	float cam_yaw_speed = 10.0f; // 10 degrees per second
	float cam_pos[] = { 0.0f, 0.0f,
											4.0f }; // don't start at zero, or we will be too close
	float cam_yaw = 0.0f;				// y-rotation in degrees
	mat4 T =
		translate( identity_mat4(), vec3( -cam_pos[0], -cam_pos[1], -cam_pos[2] ) );
	mat4 R = rotate_y_deg( identity_mat4(), -cam_yaw );
	mat4 view_mat = R * T;

	/* Attach the newly created uniforms to all programs */
	attachUniforms("view", view_mat.m);
	attachUniforms("proj", proj_mat);

	/*------------------------------rendering
	 * loop--------------------------------*/
	/* some rendering defaults */
	//glEnable( GL_CULL_FACE ); // cull face
	//glCullFace( GL_BACK );		// cull back face
	//glFrontFace( GL_CW );			// GL_CCW for counter clock-wise

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	while ( !glfwWindowShouldClose( g_window ) ) {
		static double previous_seconds = glfwGetTime();
		double current_seconds = glfwGetTime();
		double elapsed_seconds = current_seconds - previous_seconds;
		previous_seconds = current_seconds;

		_update_fps_counter( g_window );
		// wipe the drawing surface clear
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glViewport( 0, 0, g_gl_width, g_gl_height );

		drawAllPrograms();

		// update other events like input handling
		glfwPollEvents();

		/*-----------------------------move camera
		 * here-------------------------------*/
		// control keys
		bool cam_moved = false;
		if ( glfwGetKey( g_window, GLFW_KEY_LEFT ) ) {
			cam_pos[0] -= cam_speed * elapsed_seconds;
			cam_moved = true;
		}
		if ( glfwGetKey( g_window, GLFW_KEY_RIGHT ) ) {
			cam_pos[0] += cam_speed * elapsed_seconds;
			cam_moved = true;
		}
		if ( glfwGetKey( g_window, GLFW_KEY_UP ) ) {
			cam_pos[1] += cam_speed * elapsed_seconds;
			cam_moved = true;
		}
		if ( glfwGetKey( g_window, GLFW_KEY_DOWN ) ) {
			cam_pos[1] -= cam_speed * elapsed_seconds;
			cam_moved = true;
		}
		if ( glfwGetKey( g_window, GLFW_KEY_W ) ) {
			cam_pos[2] -= cam_speed * elapsed_seconds;
			cam_moved = true;
		}
		if ( glfwGetKey( g_window, GLFW_KEY_S ) ) {
			cam_pos[2] += cam_speed * elapsed_seconds;
			cam_moved = true;
		}
		if ( glfwGetKey( g_window, GLFW_KEY_A ) ) {
			cam_yaw += cam_yaw_speed * elapsed_seconds;
			cam_moved = true;
		}
		if ( glfwGetKey( g_window, GLFW_KEY_D ) ) {
			cam_yaw -= cam_yaw_speed * elapsed_seconds;
			cam_moved = true;
		}
		/* update view matrix */
		if ( cam_moved ) {
			mat4 T = translate( identity_mat4(), vec3( -cam_pos[0], -cam_pos[1],
																								 -cam_pos[2] ) ); // cam translation
			mat4 R = rotate_y_deg( identity_mat4(), -cam_yaw );					//
			mat4 view_mat = R * T;

			/* Update the uniforms attached to the various programs */
			attachUniforms("view", view_mat.m);
		}

		if ( GLFW_PRESS == glfwGetKey( g_window, GLFW_KEY_ESCAPE ) ) {
			glfwSetWindowShouldClose( g_window, 1 );
		}
		// put the stuff we've been drawing onto the display
		glfwSwapBuffers( g_window );
	}

	// close GL context and any other GLFW resources
	glfwTerminate();
	return 0;
}
