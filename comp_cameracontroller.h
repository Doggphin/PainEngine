#ifndef COMP_CAMERACONTROLLER
#define COMP_CAMERACONTROLLER

#include "macros.h"
#include "comp_camera.h"
#include "vector2.h"

typedef struct CameraController {
	struct Vector2 totalMouseDelta;

	Camera* camera;

	MACRO_COMPONENTFIELDS
} CameraController;

void CameraController_awake(CameraController* c);
void CameraController_start(CameraController* c);
void CameraController_update(float t, CameraController* c);
void CameraController_lateupdate(CameraController* c);
void CameraController_apply(CameraController* x);
void CameraController_getLookVector(Vector3* out, CameraController* x);

#endif