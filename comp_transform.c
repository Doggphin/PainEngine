#include "comp_transform.h"
#include "vector3.h"

void Transform_awake(Transform* x) {
	Vector3_set(0, 0, 0, &(x->position));
	Vector3_set(1, 1, 1, &(x->scale));
	Quaternion_setIdentity(&(x->rotation));
}

void Transform_start(Transform* x) {

}

void Transform_update(float delta, Transform* x) {

}

void Transform_lateupdate(Transform* x) {

}