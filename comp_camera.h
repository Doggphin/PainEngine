#ifndef COMP_CAMERA
#define COMP_CAMERA

#include "ecs.h"

typedef struct Camera {
	float dim;
	float fov;

	MACRO_COMPONENTFIELDS
} Camera;

void Camera_awake(Camera* x);
void Camera_start(Camera* x);
void Camera_update(float delta, Camera* x);
void Camera_lateupdate(Camera* x);

#endif