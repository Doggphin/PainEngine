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
int Physics_AABBIsInAABB(AABB* a, AABB* b);
void Physics_closestPointAABB(Vector3* point, AABB* aabb, Vector3* output);
void Physics_closestPointSphere(Vector3* point, Sphere* sphere, Vector3* output);
int Physics_raycast(Vector3* from, Vector3* to, int layerMask, RaycastResults* out);
int Physics_penetrationSphereInSphere(Sphere* sphere1, Sphere* sphere2, CollisionInfo* out);
void Physics_findAllOverlappingAABBs(Collider* colliderhash);
Collider* Physics_findOverlappingAABBs(Collider* collider, Collider* colliderhash);
int Physics_getCollisionInfo(Collider* a, Collider* b, CollisionInfo* output);
//int Physics_penetrationCircleInAABB(Sphere* sphere, AABB* aabb, CollisionInfo* out);

#endif