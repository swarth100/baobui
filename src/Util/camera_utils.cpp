#include "camera_utils.hpp"

/* Utility variables to keep track of camera movements */
float cam_speed;
float cam_yaw_speed;
float cam_yaw;
float cam_roll;

/* Camera position stored as: [xCoord, yCoord, zCoord].
   Camera points doen the Z-axis */
float cam_pos[3];

/* Arduino-related data to keep track of the extendable Magnet.
   Extension-angles above the threshold are considered to be ACTIVE dragging
   states. If below the threshold the cube is not being dragged. */
float extensionAngle;
float extensionSpeed;
#define EXTENSION_ANGLE_THRESHOLD 50

/* As mentioned above, stores the dragging status of the cube, necessary when
   updating its position dynamically */
bool draggingCube;

/* The free-fall speed of the cube.
   As no physics gets involved this is just a constant speed at which the cube
   will fall when it isn't being dragged and it's Y-Coord isn't 0 */
float cube_speed;

/* Camera lock to the bounds of the arduino-operational field.
   The lock area is a 45° tilted square with vertices in:
   (0; 10), (10; 0), (0; -10), (-10; 0),
   and extending upwards from z-Coord 5 to z-Coord 15.
   When lockCamera is deactivated, free-camming is possible */
bool lockCamera;

/* Stores the current Cube Position.
   Important not to set the Z-Coord below 0, as there will be undefined
   behaviour. */
shared_ptr<Point> cubePos;

/* init_camera initialises all camera_related components.
   Instead of assigning values upon declaration, they are assigned here and
   can be tweaked. */
void init_camera(float x, float y, float z, float speed) {
  /* Initialise the camera speed */
  cam_speed = speed;

  /* Speed at which the camera will rotate on any given axis.
     Current speed is 10° per second. */
	cam_yaw_speed = 10.0f;

  /* Initialises all camera_position components */
	cam_pos[0] = x;
  cam_pos[1] = y;
  cam_pos[2] = z;

  /* By default the camera isn't tilted on any axis and is facing downwards */
  cam_yaw = 0.0f;				// y-rotation in degrees
  cam_roll = 0.0f;

  /* Camera lock is initialised to true and the cube isn't being dragged */
  lockCamera = true;
  draggingCube = false;

  /* The companionCube is initialised over the topButton.
     This means it will have a y-offset of 10.
     Due to the intro Audio clip, the companion cube must reach the platform
     after a certain amount of time, due to this it is initialised at a given
     height and it will slowly make its way down. */
  cubePos = make_shared<Point>(0, 10, 50);

  /* Default speed of the freefalling cube */
  cube_speed = 2.0f;

  /* Default arduino magnet extension controllers */
  extensionAngle = 80;
  extensionSpeed = 25.0f;
}

/* Main camera update function.
   Camera position is updated based on the currently pressed keys. */
bool update_camera(GLFWwindow* g_window, double elapsed_seconds) {
	bool cam_moved = false;
	if (glfwGetKey(g_window, GLFW_KEY_LEFT)) {
    /* The LEFT key shifts the camera negatively on the x axis */
		updateCameraPos(0, false, elapsed_seconds);
		cam_moved = true;
	}
	if (glfwGetKey(g_window, GLFW_KEY_RIGHT)) {
    /* The RIGHT key shifts the camera positivley on the x axis */
		updateCameraPos(0, true, elapsed_seconds);
		cam_moved = true;
	}
	if (glfwGetKey(g_window, GLFW_KEY_UP)) {
    /* The UP key shifts the camera positively on the y axis */
		updateCameraPos(1, true, elapsed_seconds);
		cam_moved = true;
	}
	if (glfwGetKey(g_window, GLFW_KEY_DOWN)) {
    /* The DOWN key shifts the camera negatively on the y axis */
		updateCameraPos(1, false, elapsed_seconds);
		cam_moved = true;
	}
	if (glfwGetKey(g_window, GLFW_KEY_W)) {
    /* The W key shifts the camera negatively on the z axis */
		updateCameraPos(2, false, elapsed_seconds);
		cam_moved = true;
	}
	if (glfwGetKey(g_window, GLFW_KEY_S)) {
    /* The S key shifts the camera positively on the z axis */
		updateCameraPos(2, true, elapsed_seconds);
		cam_moved = true;
	}
  if (glfwGetKey(g_window, GLFW_KEY_A)) {
    /* The A key retracts the magnet from it being extended */
    moveExtension(false, elapsed_seconds);
    cam_moved = true;
	}
	if (glfwGetKey(g_window, GLFW_KEY_D)) {
    /* The D key extends the magnet so that it ccan grab pieces correctly */
    moveExtension(true, elapsed_seconds);
    cam_moved = true;
	}
	if (glfwGetKey(g_window, GLFW_KEY_R)) {
    /* The R key rotates the view on the x axis positively */
		cam_yaw += cam_yaw_speed * elapsed_seconds;
		cam_moved = true;
	}
	if (glfwGetKey(g_window, GLFW_KEY_T)) {
    /* The T key rotates the view on the x axis negatively */
		cam_yaw -= cam_yaw_speed * elapsed_seconds;
		cam_moved = true;
	}
  if (glfwGetKey(g_window, GLFW_KEY_G)) {
    /* The G key rotates the view on the z axis positively */
		cam_roll += cam_yaw_speed * elapsed_seconds;
		cam_moved = true;
	}
	if (glfwGetKey(g_window, GLFW_KEY_H)) {
    /* The G key rotates the view on the z axis negatievly */
		cam_roll -= cam_yaw_speed * elapsed_seconds;
		cam_moved = true;
	}
  if (glfwGetKey(g_window, GLFW_KEY_L)) {
    /* The L key locks the camera movement binding it to the max-mov rectangle */
		lockCamera = true;
	}
	if (glfwGetKey(g_window, GLFW_KEY_U)) {
    /* The U key unlocks camera movement allowing free-camming.
       NOTE: It is important NOT to enable this mode while connected to the
       arduino board as pistons might be extended beyond their maximum tolerance
       angle. */
		lockCamera = false;
	}

	return cam_moved;
}

/* updateCameraPos is a helper function that accordingly updates a specific field
   of the camera position array, given a certain value. */
bool updateCameraPos(int index, bool increase, float elapsed_seconds) {
  /* Temporarily stores the old value of the camera at the given index.
     Should the new position not be in a valid state, we must revert back to the
     old, previously stored position */
  float old_value = cam_pos[index];

  /* Switched based on wether or not the new camera state will increase or
     decrease the current value of said variable */
  if (increase) {
    cam_pos[index] += cam_speed * elapsed_seconds;
  } else {
    cam_pos[index] -= cam_speed * elapsed_seconds;
  }

  /* Should camera lock be set to false, all new camera positions will be
     considered valid. */
  if (lockCamera) {
    /* Revert the camera position back to the old one should the sum of the X
       and Y coordinates exceed 10. */
    if (float_abs(cam_pos[0]) + float_abs(cam_pos[1]) > 10) {
      cam_pos[index] = old_value;
    }
    /* Revert the camera position back to the old one should the Z axis
       coordinate not be in range 5 to 15 */
    if (cam_pos[2] < 5 || cam_pos[2] > 15) {
      cam_pos[index] = old_value;
    }
  }

  /* During cube draggins, the cube position must be updated as well as the
     camera. For optimisatio reasons, the three axis will be updated separately */
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

  /* Retuns true, meaning that a successful camera update has occured */
  return true;
}

/* Returns the extension angle of the magnet servoMotor.
   Extension angle can be asserted to be in range [0; 100] */
int getExtensionAngle() {
  return (int) extensionAngle;
}

/* Packages and returns the camera position as a shared_ptr<Point> */
shared_ptr<Point> getCamera() {
  return make_shared<Point>(cam_pos[0], cam_pos[1], cam_pos[2]);
}

/* Returns a shared_ptr<Point> holding the current companionCube's position.
   Should the cube not be dragged while it's z-axis offset is != 0, it is
   slowly lowered towards the ground at a constant speed.
   getCubePos MUST be called at every draw loop iteration with the given elapsed
   seconds */
shared_ptr<Point> getCubePos(float elapsed_seconds) {
  if (!draggingCube && cubePos->z >= 0) {
    cubePos->z -= cube_speed * elapsed_seconds;
  }
  return cubePos;
}

/* Sets the companionCube's position to a given Point value. */
void setCubePos(shared_ptr<Point> newPos) {
  cubePos = newPos;
}

/* Calculates and retrueved the full translation matrix.
   T rapresents the original matrix determined by the given positon the camera
   has in the virtual space.
   Rz rapresents the rotation on the z-axis.
   Rx rapresents the rotation on the x-axis.
   Byy multiplying the three matrices together we can determine the current view
   at which the given scene has to be offset and rotated. */
mat4 getTranslationMatrix() {
  mat4 T = translate( identity_mat4(), vec3(-cam_pos[0], -cam_pos[1], -cam_pos[2]));
	mat4 Rz = rotate_z_deg(identity_mat4(), -cam_yaw);
  mat4 Rx = rotate_x_deg(identity_mat4(), -cam_roll);
	mat4 view_mat = Rx * Ry * T;

  return view_mat;
}

/* The projection matrix projects all the elements of the scene onto the camera's
   view plane.
   The following calculations are those explained by anton in his book and I
   will take no credit for them. */
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

/* The moveExtension helper function determines the angle at which the
   extension servoMotor should be.
   It handles boundary checking for the servoMotor limiting moovement between
   0 and 100 degrees.
   Based on the EXTENSION_ANGLE_THRESHOLD, it determines after which angle the
   extension is to be considered dragging the companionCube. */
void moveExtension(bool isExtending, float elapsed_seconds) {
  int oldAngle = extensionAngle;

  /* According to the extension movement increases or decreases the servo's angle */
  if (isExtending) {
    extensionAngle += extensionSpeed * elapsed_seconds;
  } else {
    extensionAngle -= extensionSpeed * elapsed_seconds;
  }

  /* Limitations of movement */
  if (extensionAngle < 0 || extensionAngle > 100) {
    extensionAngle = oldAngle;
  }

  /* Threshold checking for cube dragging in the scene */
  if (extensionAngle < EXTENSION_ANGLE_THRESHOLD) {
    draggingCube = true;
  } else {
    draggingCube = false;
  }
}
