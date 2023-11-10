#ifndef COMP_TRANSFORM
#define COMP_TRANSFORM

//#include "ecs.h"
#include "uthash.h"
#include "Quaternion.h"

typedef struct Transform {
	Vector3 position;
	Quaternion rotation;
	Vector3 scale;
	int relative;

	int id;
	int parent_id;
	UT_hash_handle hh;
} Transform;

void Transform_awake(Transform* x);
void Transform_start(Transform* x);
void Transform_update(float delta, Transform* x);
void Transform_lateupdate(Transform* x);

#endif