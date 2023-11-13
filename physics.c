#include "physics.h"
#include <assert.h>
#include "uthash.h"
#include "ecs.h"

// https://gdbooks.gitbooks.io/3dcollisions/content/


int Physics_pointIsInAABB(Vector3* point, AABB* aabb) {
	return point->x > aabb->min.x &&
		point->y > aabb->min.y &&
		point->z > aabb->min.z &&
		point->x < aabb->max.x &&
		point->y < aabb->max.y &&
		point->z > aabb->max.z;
}

int Physics_AABBIsInAABB(AABB* a, AABB* b) {
	return (a->min.x <= b->max.x && a->max.x >= b->min.x) &&
	(a->min.y <= b->max.y && a->max.y >= b->min.y) &&
	(a->min.z <= b->max.z && a->max.z >= b->min.z);
}

int Physics_pointIsInSphere(Vector3* point, Sphere* sphere) {
	return Vector3_sqrdistance(point, &sphere->center) <= sphere->radius * sphere->radius;
}

void Physics_closestPointAABB(Vector3* point, AABB* aabb, Vector3* output) {
	assert(output != NULL);
	Vector3_set(
		point->x > aabb->max.x ? aabb->max.x :
		point->x < aabb->min.x ? aabb->min.x :
		point->x,

		point->y > aabb->max.y ? aabb->max.y :
		point->y < aabb->min.y ? aabb->min.y :
		point->y,

		point->z > aabb->max.z ? aabb->max.z :
		point->z < aabb->min.z ? aabb->min.z :
		point->z,

		output
	);
}

void Physics_closestPointSphere(Vector3* point, Sphere* sphere, Vector3* output) {
	if (Physics_pointIsInSphere(point, sphere)) {
		Vector3_copy(point, output);
	}
	else {
		// Get point relative to origin. Treat output as a buffer and store it there
		Vector3_sub(point, &sphere->center, output);
		// Get magnitude of point relative to origin to normalize it later
		float magnitude = Vector3_magnitude(output);
		// Normalize vector and scale it by the radius of the sphere
		// This MIGHT cause problems because it's reading and writing to same buffer but I don't think it will. check later
		Vector3_set((output->x / magnitude) * sphere->radius, (output->y / magnitude) * sphere->radius, (output->z / magnitude) * sphere->radius, output);
	}
}

int Physics_raycast(Vector3* from, Vector3* to, int layerMask, RaycastResults* out) {
	return 0;
}

int Physics_penetrationSphereInSphere(Sphere* sphereDynamic, Sphere* sphereStatic, CollisionInfo* out) {
	assert(out != NULL);

	struct Vector3 dynamicToStatic;
	Vector3_sub(&sphereStatic->center, &sphereDynamic->center, &dynamicToStatic);
	float cumradius = sphereDynamic->radius + sphereStatic->radius;
	float distance = Vector3_length(&dynamicToStatic);

	//printf("Distance is %.6f, cumulative radius is %.6f\n", distance, cumradius);
	if (distance >= cumradius) { return 0; }
	// First, set distance of out distance
	out->distance = distance;

	// Next, generate normalized fromTo and assign out normal to its negative
	struct Vector3 fromTo;
	Vector3_copy(&dynamicToStatic, &fromTo);
	Vector3_normalize(&dynamicToStatic, &dynamicToStatic);
	Vector3_copy(&fromTo, &out->normal);
	Vector3_multiply(&out->normal, -1, &out->normal);

	// Set depth to distance minus their radii
	out->depth = distance - cumradius;

	// Set safeposition to dynamic sphere's center minus depth * the negative fromTo vector
	Vector3_copy(&fromTo, &out->safePosition);
	Vector3_multiply(&out->safePosition, out->depth - sphereDynamic->skin, &out->safePosition);
	Vector3_add(&out->safePosition, &sphereDynamic->center, &out->safePosition);
	//Vector3_print(&out->safePosition);

	// Set point to bounded point trying to go from safeposition to static sphere's center
	Vector3_multiply(&fromTo, sphereDynamic->radius, &out->point);
	Vector3_add(&out->point, &out->safePosition, &out->point);

	return 1;
}

void Physics_findAllOverlappingAABBs(Collider* colliderhash) {
	struct Collider* base;
	// Check for all overlaps between all AABBs.
    for (base = colliderhash; base != NULL; base = base->hh.next) {
		struct Collider* compare = base;
		compare = compare->hh.next;
		while(compare != NULL) {
			printf("Comparing the colliders of entities %d and %d. \n", base->entity->id, compare->entity->id);
			if(Physics_AABBIsInAABB(base->AABB, compare->AABB)) {
				printf("Overlap!\n");
			}
			compare = compare->hh.next;
		}
    }
}

Collider* Physics_findOverlappingAABBs(Collider* collider, Collider* colliderhash) {
	struct Collider* check = malloc(sizeof(Collider));
	struct Collider* rethash = NULL;

	for(check = colliderhash; check != NULL; check = check->hh.next) {
		
		if(check->id == collider->id) {continue;}
		if(Physics_AABBIsInAABB(collider->AABB, check->AABB)) {
			HASH_ADD(collisionhh, rethash, id2, sizeof(int), check);	// duh, this removes it from the hash
		}
		
	}

	return rethash;
}

int Physics_getCollisionInfo(Collider* a, Collider* b, CollisionInfo* output) {
	switch(a->shape) {
		case SHAPE_SPHERE:
			switch(b->shape) {
				case SHAPE_SPHERE:
					if(Physics_penetrationSphereInSphere((Sphere*)a->shapeStruct, (Sphere*)b->shapeStruct, output)) {
						return 1;
					}
					return 0;
				default:
					printf("Unimplemented collision detection for shape types %d and %d!\n", a->shape, b->shape);
					return 0;
			}
		default:
			printf("Unimplemented collision detection for shape types %d and %d!\n", a->shape, b->shape);
			return 0;
	}
}

