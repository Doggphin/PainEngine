#ifndef PHYSICS
#define PHYSICS

#include "vector3.h"
#include "comp_collider.h"
#include "ecs.h"

typedef struct CollisionInfo {
	Vector3 point;
	float distance;
	float depth;
	Vector3 normal;
	Vector3 safePosition;
} CollisionInfo;

typedef struct RaycastResults {
	Collider* collider;
	CollisionInfo info;
} RaycastResults;



int Physics_pointIsInAABB(Vector3* point, AABB* aabb);
int Physics_pointIsInSphere(Vector3* point, Sphere* sphere);
void Physics_closestPointAABB(Vector3* point, AABB* aabb, Vector3* output);
void Physics_closestPointSphere(Vector3* point, Sphere* sphere, Vector3* output);
int Physics_raycast(Vector3* from, Vector3* to, int layerMask, RaycastResults* out);
int Physics_penetrationCircleInCircle(Sphere* sphere1, Sphere* sphere2, CollisionInfo* out);
//int Physics_penetrationCircleInAABB(Sphere* sphere, AABB* aabb, CollisionInfo* out);

#endif