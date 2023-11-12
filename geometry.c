#include "geometry.h"
#include "macros.h"
#include "ecs.h"

void Geometry_calculateAABB(Vector3* center, Vector3* extents, AABB* out) {
	assert(out != NULL);

	Vector3_copy(center, &(out->center));
	Vector3_copy(extents, &(out->extents));
	Vector3_set(center->x + extents->x / 2, center->y + extents->y / 2, center->z + extents->z / 2, &(out->max));
	Vector3_set(center->x - extents->x / 2, center->y - extents->y / 2, center->z - extents->z / 2, &(out->min));
}

void Geometry_generateAABBShape(void* shape, PrimitiveShape shapeType, AABB* out) {
	switch (shapeType) {
	case SHAPE_NONE:
		return;
	case SHAPE_AABB:
		AABB* aabb = (AABB*)shape;
		Geometry_calculateAABB(&aabb->center, &aabb->extents, out);
		break;
	case SHAPE_SPHERE:
		Sphere* sphere = (Sphere*)shape;
		Vector3 sphereExtents = { sphere->radius, sphere->radius, sphere->radius };
		Geometry_calculateAABB(&sphere->center, &sphereExtents, out);
		break;
	}
	return;
}

Sphere* Geometry_createSphere(float radius) {
	QMALLOC(ret, Sphere)
	ret->radius = radius;
	return ret;
}

Cuboid* Geometry_createCuboid(Vector3* scale, Quaternion* rotation) {
	QMALLOC(ret, Cuboid)
	Vector3_copy(scale, &ret->scale);
	Quaternion_copy(rotation, &ret->rotation);
	return ret; 
}

AABB* Geometry_createAABB(Vector3* extents) {
	QMALLOC(ret, AABB)
	Vector3_copy(extents, &ret->extents);
	return ret;
}

void* Geometry_applyTransform(void* shape, PrimitiveShape shapeType, Transform* t) {
	switch(shapeType) {
		case SHAPE_NONE:
			printf("Cannot apply transform to SHAPE_NONE!");
			return NULL;

		case SHAPE_SPHERE:
			QMALLOC(sphere, Sphere)
			memcpy(sphere, shape, sizeof(Sphere));
			Vector3_copy(&t->position, &sphere->center);
			sphere->radius = fmax(fmax(t->scale.x, t->scale.y), t->scale.z);
			return sphere;

		case SHAPE_AABB:
			QMALLOC(newExtents, Vector3)
			//Quaternion_rotate(&t->rotation, &((AABB*)shape)->extents, newExtents); use with cuboid
			Vector3_multiplyEach(newExtents, &t->scale, newExtents);
			struct AABB* aabb = malloc(sizeof(aabb));
			Geometry_calculateAABB(&t->position, newExtents, aabb);
			free(newExtents);
			return aabb;
	}
	return NULL;
}