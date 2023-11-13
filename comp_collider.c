#include "comp_collider.h"
#include "macros.h"

void Collider_updateAABB(Collider* x, Transform* newT) {
	if(x->shape == SHAPE_NONE) {
		return;
	}

	int recalculateAABB = 0;

	if (!Vector3_equal(&newT->position, &x->_AABBpositioncache)) {
		Vector3_copy(&newT->position, &x->_AABBpositioncache);
		recalculateAABB = 1;
	}
	if (!Quaternion_equal(&newT->rotation, &x->_AABBrotationcache)) {
		Quaternion_copy(&newT->rotation, &x->_AABBrotationcache);
		// spheres aren't affected by rotations
		if(x->shape != SHAPE_SPHERE) {
				recalculateAABB = 1;
		}
	}
	if (!Vector3_equal(&newT->scale, &x->_AABBscalecache)) {
		Vector3_copy(&newT->scale, &x->_AABBscalecache);
		recalculateAABB = 1;
	}
	if (x->shape != x->_AABBshapecache) {
		x->_AABBshapecache = x->shape;
		recalculateAABB = 1;
	}
	if (recalculateAABB) {
		Geometry_generateAABBShape(&newT->position, x->shapeStruct, x->shape, x->AABB);
	}
}

void Collider_updateWorldShape(Collider* c) {
	if(c->shape == SHAPE_NONE) {
		return;
	}
	QMALLOC(newT, Transform)
	Transform_hierarchyTransform(c->entity->transform, newT);
	Collider_updateAABB(c, newT);
	switch(c->shape) {
		case SHAPE_AABB:
			struct AABB* aabb = (AABB*)c->shapeStruct;
			Vector3_copy(&newT->position, &aabb->center);
		case SHAPE_SPHERE:
			struct Sphere* sphere = (Sphere*)c->shapeStruct;
			Vector3_copy(&newT->position, &sphere->center);
		case SHAPE_CUBE:
			struct Cuboid* cuboid = (Cuboid*)c->shapeStruct;
			Vector3_copy(&newT->position, &cuboid->center);
		case SHAPE_NONE:
			break;
	}
	free(newT);
}

void Collider_awake(Collider* x) {
	x->shape = SHAPE_NONE;
	x->AABB = malloc(sizeof(AABB));
	Collider_updateWorldShape(x);
	x->id2 = rand();
	printf("%d\n", x->id2);
}

void Collider_start(Collider* x) {

}

void Collider_update(float delta, Collider* x) {

}

void Collider_lateupdate(Collider* x) {

}