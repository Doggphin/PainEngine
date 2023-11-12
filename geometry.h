#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "vector3.h"
#include "ecs.h"
#include "Quaternion.h"
#include <assert.h>

typedef enum PrimitiveShape {
	SHAPE_NONE, SHAPE_AABB, SHAPE_SPHERE, SHAPE_CUBE
} PrimitiveShape;

typedef struct AABB {
	Vector3 center;
	Vector3 extents;
	Vector3 max;
	Vector3 min;
} AABB;

typedef struct Sphere {
	Vector3 center;
	float radius;
} Sphere;

typedef struct Cuboid {
	Vector3 center;
	Vector3 scale;
	Quaternion rotation;
} Cuboid;

void Geometry_generateAABB(Vector3* center, Vector3* extents, AABB* out);
void Geometry_generateAABBShape(void* shape, PrimitiveShape shapeType, AABB* out);
Sphere* Geometry_createSphere(float radius);
Cuboid* Geometry_createCuboid(Vector3* scale, Quaternion* rotation);
AABB* Geometry_createAABB(Vector3* extents);
void* Geometry_applyTransform(void* shape, PrimitiveShape shapeType, Transform* t);

#endif