#ifndef COMP_COLLIDER
#define COMP_COLLIDER

#include "macros.h"
#include "vector3.h"

typedef enum ColliderShape {
	COLL_NONE, COLL_PLANE, COLL_SPHERE, COLL_BOX
} ColliderShape;

typedef struct Collider {
	ColliderShape shape;
	Vector3 scale;

	MACRO_COMPONENTFIELDS
} Collider;

void Collider_awake(Collider* x);
void Collider_start(Collider* x);
void Collider_update(float delta, Collider* x);
void Collider_lateupdate(Collider* x);

#endif