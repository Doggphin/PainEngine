#ifndef COMP_CAMERA_H
#define COMP_CAMERA_H

#include "comp_camera.h"

void Camera_awake(Camera* x) {
	x->fov = 100;
	x->dim = 1;
}

void Camera_start(Camera* x) {

}

void Camera_update(float delta, Camera* c) {

}

void Camera_lateupdate(Camera* c) {

}

#endif