#include "camera_utils.hpp"

float cam_speed;
float cam_yaw_speed;
float cam_pos[3];
float cam_yaw;
float cam_roll;

float extensionAngle;
float extensionSpeed;
float cube_speed;

bool lockCamera;
bool draggingCube;

shared_ptr<Point> cubePos;

void init_camera(float x, float y, float z, float speed) {
  cam_speed = speed;			 // 1 unit per second
	cam_yaw_speed = 10.0f; // 10 degrees per second

  /* */
	cam_pos[0] = x;
  cam_pos[1] = y;
  cam_pos[2] = z;

  /* */
  cam_yaw = 0.0f;				// y-rotation in degrees
  cam_roll = 0.0f;

  /* */
  lockCamera = true;
  draggingCube = false;

  cubePos = make_shared<Point>(0, 10, 50);

  extensionAngle = 80;
  extensionSpeed = 25.0f;
  cube_speed = 2.0f;
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
  if (glfwGetKey(g_window, GLFW_KEY_G)) {
		cam_roll += cam_yaw_speed * elapsed_seconds;
		cam_moved = true;
	}
	if (glfwGetKey(g_window, GLFW_KEY_H)) {
		cam_roll -= cam_yaw_speed * elapsed_seconds;
		cam_moved = true;
	}
  if (glfwGetKey(g_window, GLFW_KEY_L)) {
		lockCamera = true;
	}
	if (glfwGetKey(g_window, GLFW_KEY_U)) {
		lockCamera = false;
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

  if (lockCamera) {
    if (float_abs(cam_pos[0]) + float_abs(cam_pos[1]) > 10) {
      cam_pos[index] = old_value;
    }
    if (cam_pos[2] < 5 || cam_pos[2] > 15) {
      cam_pos[index] = old_value;
    }
  }

  if (draggingCube) {
    switch (index) {
      case 0:
        cubePos->x = cam_pos[index];
        break;
      case 1:
        cubePos->y = cam_pos[index];
        break;
      case 2:
        cubePos->z = cam_pos[index] - 5;
        break;
    }
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
shared_ptr<Point> getCubePos(float elapsed_seconds) {
  if (!draggingCube) {
    if (cubePos->z >= 0) {
      cubePos->z -= cube_speed * elapsed_seconds;
    }
  }
  return cubePos;
}

void setCubePos(shared_ptr<Point> newPos) {
  cubePos = newPos;
}

/* */
mat4 getTranslationMatrix() {
  mat4 T = translate( identity_mat4(), vec3(-cam_pos[0], -cam_pos[1], -cam_pos[2]));
	mat4 Ry = rotate_z_deg(identity_mat4(), -cam_yaw);
  mat4 Rx = rotate_x_deg(identity_mat4(), -cam_roll);
	mat4 view_mat = Rx * Ry * T;

  return view_mat;
}

/* */
mat4 getProjectionMatrix() {
  // input variables
	float near = 0.1f;									// clipping plane
	float far = 100.0f;									// clipping plane
	float fov = 67.0f * ONE_DEG_IN_RAD; // convert 67 degrees to radians
	float aspect = (float)g_gl_width / (float)g_gl_height; // aspect ratio
	// matrix components
	float range = tan(fov * 0.5f) * near;
	float Sx = (2.0f * near) / (range * aspect + range * aspect);
	float Sy = near / range;
	float Sz = -(far + near) / (far - near);
	float Pz = -(2.0f * far * near) / (far - near);
	mat4 proj_mat = mat4 (  Sx, 0.0f, 0.0f, 0.0f,
		                    0.0f,   Sy,	0.0f, 0.0f,
												0.0f, 0.0f,   Sz,-1.0f,
												0.0f, 0.0f,   Pz, 0.0f );

  return proj_mat;
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

  if (extensionAngle < 50) {
    draggingCube = true;
  } else {
    draggingCube = false;
  }
}
