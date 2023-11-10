#include "comp_cameracontroller.h"
#include "ecs.h"

void CameraController_awake(CameraController* c) {

}

void CameraController_start(CameraController* c) {
	c->camera = ECS_getComponent(ECS_getEntity(c->parent_id), CTYPE_CAMERA);
	if (c == NULL) { printf("No camera found!"); }
}

void CameraController_update(float delta, CameraController* c) {

}

void CameraController_lateupdate(CameraController* c) {

}