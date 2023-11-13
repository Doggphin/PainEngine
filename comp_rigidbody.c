#include "comp_rigidbody.h"
#include "geometry.h"
#include "physics.h"

struct Vector3 gravity = {0, -.98, 0};

void Rigidbody_awake(Rigidbody* c) {
	Vector3_set(-.00005, -.000003, 0.0004, &c->velocity);
}

void Rigidbody_start(Rigidbody* c) {
	c->collider = ECS_getComponent(c->entity, CTYPE_COLLIDER);
}

void Rigidbody_update(float delta, Rigidbody* c) {
	c->totalTime += delta;
	struct Vector3 gravityAcceleration;
	struct Vector3 zero;
	Vector3_preset(VECTOR3_ZERO, &zero);
	//Vector3_set(0, -.98, 0, &gravityAcceleration);
	Vector3_sub(&zero, &c->entity->transform->position, &gravityAcceleration);
	float distance = Vector3_magnitude(&gravityAcceleration);
	Vector3_multiply(&gravityAcceleration, 1/distance * 0.000001, &gravityAcceleration);
	Vector3_multiply(&gravityAcceleration, 0.05, &gravityAcceleration);
	//Vector3_multiply(&gravityAcceleration, delta * 0.001, &gravityAcceleration);
	//Vector3_addf(&gravityAcceleration, sin(c->totalTime) * delta * 0.0001, sin(c->totalTime * 1.7071)* delta * 0.0001, cos(c->totalTime)* delta * 0.0001);
    Vector3_add(&c->velocity, &gravityAcceleration, &c->velocity);

    Collider* collisions = NULL;
	collisions = Physics_findOverlappingAABBs(c->collider, (Collider*)ECS_getAllInstancesOfComponent(CTYPE_COLLIDER));
	struct Collider* possibleCollision;

	for(possibleCollision = collisions; possibleCollision != NULL; possibleCollision = possibleCollision->collisionhh.next) {
		struct CollisionInfo collisionInfo;
		if(Physics_getCollisionInfo(c->collider, possibleCollision, &collisionInfo) == 1) {
			Vector3_copy(&collisionInfo.safePosition, &c->entity->transform->position);
			float speed = Vector3_magnitude(&c->velocity);
			Vector3_copy(&collisionInfo.normal, &c->velocity);
			Vector3_multiply(&c->velocity, speed, &c->velocity);
			Vector3_multiply(&c->velocity, 0.99, &c->velocity);
			//struct Vector3 test;
			//Vector3_preset(VECTOR3_UP, &test);
			//Vector3_multiply(&test, 0.001, &test);
			//Vector3_copy(&test, &c->velocity);
		}
	}

	//printf("Moving by ");
	//Vector3_print(&c->velocity);
    Vector3_add(&c->velocity, &c->entity->transform->position, &c->entity->transform->position);
}

void Rigidbody_lateupdate(Rigidbody* c) {

}