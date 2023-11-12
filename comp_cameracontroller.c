#include "comp_cameracontroller.h"
#include "ecs.h"
#include "vector2.h"
#include "input.h"

void CameraController_awake(CameraController* c) {
	c->camera = NULL;
	Vector2_set(0, 0, &c->totalMouseDelta);
}

void CameraController_start(CameraController* c) {
	c->camera = ECS_getComponent(c->entity, CTYPE_CAMERA);
	if (c == NULL) { printf("No camera found!"); }
}

void CameraController_update(float delta, CameraController* c) {

}

void CameraController_lateupdate(CameraController* c) {

}

void CameraController_getLookVector(Vector3* out, CameraController* x) {
	Vector3_set(cos(x->totalMouseDelta.x), -tan(x->totalMouseDelta.y), sin(x->totalMouseDelta.x), out);
}

void CameraController_apply(CameraController* x) {
	struct Vector2 mouseDelta;
	Input_getMouseDelta(&mouseDelta);
	Vector2_multiply(&mouseDelta, 0.001, &mouseDelta);
	Vector2_addDirect(&mouseDelta, &x->totalMouseDelta);
	Vector2_set(fmodf(x->totalMouseDelta.x, 6.2831), x->totalMouseDelta.y, &x->totalMouseDelta);
	if(x->totalMouseDelta.y > 1.5707) { Vector2_set(x->totalMouseDelta.x, 1.5707, &x->totalMouseDelta); }
	else if(x->totalMouseDelta.y < -1.5707) { Vector2_set(x->totalMouseDelta.x, -1.5707, &x->totalMouseDelta); }
	struct Vector3 lookVector;
	CameraController_getLookVector(&lookVector, x);
	Vector3_normalize(&lookVector, &lookVector);
	struct Quaternion quat;
	struct Vector3 forward;
	Vector3_preset(VECTOR3_FORWARD, &forward);
	Quaternion_fromToRotation(&forward, &lookVector, &quat);
	Quaternion_copy(&quat, &x->entity->transform->rotation);
}