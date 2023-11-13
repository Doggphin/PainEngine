#ifndef COMP_COLLIDER
#define COMP_COLLIDER

#include "ecs.h"
#include "vector3.h"
#include "Quaternion.h"
#include "geometry.h"

typedef struct TransformValues {
	Vector3 position;
	Quaternion rotation;
	Vector3 scale;
} TransformValues;

typedef struct Collider {
	PrimitiveShape shape;
	void* shapeStruct;
	
	AABB* AABB;

	Vector3 _AABBpositioncache;
	Quaternion _AABBrotationcache;
	Vector3 _AABBscalecache;
	PrimitiveShape _AABBshapecache;
	void* _transformedShape;
	MACRO_COMPONENTFIELDS
	int id2;
	UT_hash_handle collisionhh;
} Collider;


void Collider_awake(Collider* x);
void Collider_start(Collider* x);
void Collider_update(float delta, Collider* x);
void Collider_lateupdate(Collider* x);
void Collider_updateWorldShape(Collider* x);

#endif