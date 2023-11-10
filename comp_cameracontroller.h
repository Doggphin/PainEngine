#ifndef COMP_CAMERACONTROLLER
#define COMP_CAMERACONTROLLER

#include "macros.h"
#include "comp_camera.h"

typedef struct CameraController {
	Camera* camera;

	MACRO_COMPONENTFIELDS
} CameraController;

void CameraController_awake(CameraController* c);
void CameraController_start(CameraController* c);
void CameraController_update(float t, CameraController* c);
void CameraController_lateupdate(CameraController* c);

#endif