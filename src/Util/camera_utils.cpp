#include "camera_utils.hpp"

float cam_speed;
float cam_yaw_speed;
float cam_pos[3];
float cam_yaw;

float extensionAngle;
float extensionSpeed;

void init_camera(float x, float y, float z, float speed) {
  cam_speed = speed;			 // 1 unit per second
	cam_yaw_speed = 10.0f; // 10 degrees per second

  /* */
	cam_pos[0] = x;
  cam_pos[1] = y;
  cam_pos[2] = z;

  /* */
  cam_yaw = 0.0f;				// y-rotation in degrees

  extensionAngle = 80;
  extensionSpeed = 25.0f;
}

/* */
bool update_camera(GLFWwindow* g_window, double elapsed_seconds) {
	bool cam_moved = false;
	if (glfwGetKey(g_window, GLFW_KEY_LEFT)) {
		updateCameraPos(0, false, elapsed_seconds);
		cam_moved = true;
	}
	if (glfwGetKey(g_window, GLFW_KEY_RIGHT)) {
		updateCameraPos(0, true, elapsed_seconds);
		cam_moved = true;
	}
	if (glfwGetKey(g_window, GLFW_KEY_UP)) {
		updateCameraPos(1, true, elapsed_seconds);
		cam_moved = true;
	}
	if (glfwGetKey(g_window, GLFW_KEY_DOWN)) {
		updateCameraPos(1, false, elapsed_seconds);
		cam_moved = true;
	}
	if (glfwGetKey(g_window, GLFW_KEY_W)) {
		updateCameraPos(2, false, elapsed_seconds);
		cam_moved = true;
	}
	if (glfwGetKey(g_window, GLFW_KEY_S)) {
		updateCameraPos(2, true, elapsed_seconds);
		cam_moved = true;
	}
  if (glfwGetKey(g_window, GLFW_KEY_A)) {
    moveExtension(false, elapsed_seconds);
    cam_moved = true;
	}
	if (glfwGetKey(g_window, GLFW_KEY_D)) {
    moveExtension(true, elapsed_seconds);
    cam_moved = true;
	}
	if (glfwGetKey(g_window, GLFW_KEY_R)) {
		cam_yaw += cam_yaw_speed * elapsed_seconds;
		cam_moved = true;
	}
	if (glfwGetKey(g_window, GLFW_KEY_T)) {
		cam_yaw -= cam_yaw_speed * elapsed_seconds;
		cam_moved = true;
	}

	return cam_moved;
}

bool updateCameraPos(int index, bool increase, float elapsed_seconds) {
  float old_value = cam_pos[index];

  if (increase) {
    cam_pos[index] += cam_speed * elapsed_seconds;
  } else {
    cam_pos[index] -= cam_speed * elapsed_seconds;
  }

  if (float_abs(cam_pos[0]) + float_abs(cam_pos[1]) > 10) {
    cam_pos[index] = old_value;
  }
  if (cam_pos[2] < 5 || cam_pos[2] > 15) {
    cam_pos[index] = old_value;
  }

  return true;
}

int getExtensionAngle() {
  return (int) extensionAngle;
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

/* */
void moveExtension(bool isExtending, float elapsed_seconds) {
  int oldAngle = extensionAngle;

  if (isExtending) {
    extensionAngle += extensionSpeed * elapsed_seconds;
  } else {
    extensionAngle -= extensionSpeed * elapsed_seconds;
  }

  if (extensionAngle < 0 || extensionAngle > 100) {
    extensionAngle = oldAngle;
  }
}
