#include "camera_utils.hpp"

float cam_speed;
float cam_yaw_speed;
float cam_pos[3];
float cam_yaw;

void init_camera(float x, float y, float z) {
  cam_speed = 1.5f;			 // 1 unit per second
	cam_yaw_speed = 10.0f; // 10 degrees per second

  /* */
	cam_pos[0] = x;
  cam_pos[1] = y;
  cam_pos[2] = z;

  /* */
  cam_yaw = 0.0f;				// y-rotation in degrees
}

/* */
bool update_camera(GLFWwindow* g_window, double elapsed_seconds) {
	bool cam_moved = false;
	if (glfwGetKey(g_window, GLFW_KEY_LEFT)) {
		cam_pos[0] -= cam_speed * elapsed_seconds;
		cam_moved = true;
	}
	if (glfwGetKey(g_window, GLFW_KEY_RIGHT)) {
		cam_pos[0] += cam_speed * elapsed_seconds;
		cam_moved = true;
	}
	if (glfwGetKey(g_window, GLFW_KEY_UP)) {
		cam_pos[1] += cam_speed * elapsed_seconds;
		cam_moved = true;
	}
	if (glfwGetKey(g_window, GLFW_KEY_DOWN)) {
		cam_pos[1] -= cam_speed * elapsed_seconds;
		cam_moved = true;
	}
	if (glfwGetKey(g_window, GLFW_KEY_W)) {
		cam_pos[2] -= cam_speed * elapsed_seconds;
		cam_moved = true;
	}
	if (glfwGetKey(g_window, GLFW_KEY_S)) {
		cam_pos[2] += cam_speed * elapsed_seconds;
		cam_moved = true;
	}
	if (glfwGetKey(g_window, GLFW_KEY_A)) {
		cam_yaw += cam_yaw_speed * elapsed_seconds;
		cam_moved = true;
	}
	if (glfwGetKey(g_window, GLFW_KEY_D)) {
		cam_yaw -= cam_yaw_speed * elapsed_seconds;
		cam_moved = true;
	}

	return cam_moved;
}

/* */
shared_ptr<Point> getCamera() {
  return make_shared<Point>(cam_pos[0], cam_pos[1], cam_pos[2]);
}

/* */
mat4 getTranslationMatrix() {
  mat4 T = translate( identity_mat4(), vec3(-cam_pos[0], -cam_pos[1], -cam_pos[2]));
	mat4 R = rotate_y_deg(identity_mat4(), -cam_yaw);
	mat4 view_mat = R * T;

  return view_mat;
}
