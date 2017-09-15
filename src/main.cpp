/******************************************************************************\
| OpenGL 4 Example Code.                                                       |
| Accompanies written series "Anton's OpenGL 4 Tutorials"                      |
| Email: anton at antongerdelan dot net                                        |
| First version 27 Jan 2014                                                    |
| Copyright Dr Anton Gerdelan, Trinity College Dublin, Ireland.                |
| See individual libraries for separate legal notices                          |
|******************************************************************************|
| Virtual Camera - create and modify VIEW and PROJECTION matrices              |
| keyboard controls: W,S,A,D,left and right arrows                             |
\******************************************************************************/
#define _USE_MATH_DEFINES
#include <math.h>

#include "Util/Util.hpp"
#include "Math/maths_funcs.h"
#include "Component/Component.hpp"
#include "Component/Grid.hpp"
#include "Component/Prism.hpp"
#include "Geometry/Point.hpp"
#include "Program/Program.hpp"

#define GL_LOG_FILE "gl.log"

using namespace std;

// keep track of window size for things like the viewport and the mouse cursor
int g_gl_width = 640;
int g_gl_height = 480;
GLFWwindow *g_window = NULL;

/* List of initialised components to be rendered */
list<shared_ptr<Component>> components;

int main() {
	restart_gl_log();
	/*------------------------------start GL
	 * context------------------------------*/
	start_gl();

	/*------------------------------create
	 * geometry-------------------------------*/
	GLfloat points[] = {
    -1.0f,-1.0f,-1.0f, // triangle 1 : begin
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f, // triangle 1 : end
    1.0f, 1.0f,-1.0f, // triangle 2 : begin
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f, // triangle 2 : end
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f
};

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

	GLuint points_vbo;
	glGenBuffers( 1, &points_vbo );
	glBindBuffer( GL_ARRAY_BUFFER, points_vbo );
	glBufferData( GL_ARRAY_BUFFER, 12*3*3 * sizeof( GLfloat ), points, GL_STATIC_DRAW );

	GLuint colours_vbo;
	glGenBuffers( 1, &colours_vbo );
	glBindBuffer( GL_ARRAY_BUFFER, colours_vbo );
	glBufferData( GL_ARRAY_BUFFER, 12*3*3 * sizeof( GLfloat ), colours, GL_STATIC_DRAW );

	GLuint vao;
	glGenVertexArrays( 1, &vao );
	glBindVertexArray( vao );
	glBindBuffer( GL_ARRAY_BUFFER, points_vbo );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, NULL );
	glBindBuffer( GL_ARRAY_BUFFER, colours_vbo );
	glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, NULL );
	glEnableVertexAttribArray( 0 );
	glEnableVertexAttribArray( 1 );

	GLfloat points2[] = {
    -3.0f,-3.0f,-3.0f, // triangle 1 : begin
    -3.0f,-3.0f, 3.0f,
    -3.0f, 3.0f, 3.0f, // triangle 1 : end
	};

	/*
	GLuint points2_vbo;
	glGenBuffers( 1, &points2_vbo );
	glBindBuffer( GL_ARRAY_BUFFER, points2_vbo );
	glBufferData( GL_ARRAY_BUFFER, 3*3 * sizeof( GLfloat ), points2, GL_STATIC_DRAW );

	GLuint vao2;
	glGenVertexArrays( 1, &vao2 );
	glBindVertexArray( vao2 );
	glBindBuffer( GL_ARRAY_BUFFER, points2_vbo );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, NULL );
	glEnableVertexAttribArray( 0 ); */

	shared_ptr<Component> component = make_shared<Component>();
	component->addVbo(points2, 3);

	/*------------------------------create
	 * shaders--------------------------------*/
	shared_ptr<Program> program1 = make_shared<Program>("assets/test_vs.glsl",  "assets/test_fs.glsl", vao);

	shared_ptr<Program> program2 = make_shared<Program>("assets/test2_vs.glsl",  "assets/test_fs.glsl", component->getVao());

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

	program1->attachUniform("view", view_mat.m);
	program1->attachUniform("proj", proj_mat);

	program2->attachUniform("view", view_mat.m);
	program2->attachUniform("proj", proj_mat);

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

		//glUseProgram( shader_programme );
		//glBindVertexArray( vao );
		// draw points 0-3 from the currently bound VAO with current in-use shader
		//glDrawArrays( GL_TRIANGLES, 0, 12*3 );

		program1->draw();

		program2->draw();

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

			program1->attachUniform("view", view_mat.m);
			program2->attachUniform("view", view_mat.m);
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
