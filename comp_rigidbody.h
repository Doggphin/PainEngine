#ifndef COMP_RIGIDBODY
#define COMP_RIGIDBODY

#include "ecs.h"
#include "vector3.h"
#include "comp_collider.h"

typedef struct Rigidbody {
	struct Vector3 velocity;
	Collider* collider;
	float totalTime;


	MACRO_COMPONENTFIELDS	// Generates necessary fields for all components
} Rigidbody;

void Rigidbody_awake(Rigidbody* c);
void Rigidbody_start(Rigidbody* c);
void Rigidbody_update(float t, Rigidbody* c);
void Rigidbody_lateupdate(Rigidbody* c);

// Insert functions here

#endif