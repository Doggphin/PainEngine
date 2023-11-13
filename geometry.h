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
	struct Vector3 center;
	struct Vector3 extents;
	struct Vector3 max;
	struct Vector3 min;
	float skin;
} AABB;

typedef struct Sphere {
	struct Vector3 center;
	float radius;
	float skin;
} Sphere;

typedef struct Cuboid {
	struct Vector3 center;
	struct Vector3 scale;
	struct Quaternion rotation;
	float skin;
} Cuboid;

void Geometry_generateAABB(Vector3* center, Vector3* extents, AABB* out);
void Geometry_generateAABBShape(Vector3* center, void* shape, PrimitiveShape shapeType, AABB* out);
Sphere* Geometry_createSphere(float radius);
Cuboid* Geometry_createCuboid(Vector3* scale, Quaternion* rotation);
AABB* Geometry_createAABB(Vector3* extents);
void* Geometry_applyTransform(void* shape, PrimitiveShape shapeType, Transform* t);

#endif