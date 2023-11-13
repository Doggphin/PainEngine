#include "comp_playercontroller.h"
#include "ecs.h"
#include "input.h"
#include "Quaternion.h"
#include "comp_collider.h"
#include "physics.h"
#include <stdlib.h>

void PlayerController_awake(PlayerController* x) {
	x->cc = NULL;
	x->collider = NULL;
	x->moveonce = 0;
}

void PlayerController_start(PlayerController* x) {
	x->cc = ECS_getComponent(x->entity, CTYPE_CAMERACONTROLLER);
	x->collider = ECS_getComponent(x->entity, CTYPE_COLLIDER);
	if(x->cc == NULL) {printf("PlayerController requires component CameraController to function!\n");}
	if(x->collider == NULL) {printf("PlayerController requires component Collider to function!\n");}
}

void PlayerController_update(float delta, PlayerController* x) {
	if(x->cc != NULL){
		CameraController_apply(x->cc);
	}

	struct Vector2 joystick;
	Input_getJoystickNormalized(KEYCODE_W, KEYCODE_D, KEYCODE_S, KEYCODE_A, 1, &joystick);
	struct Vector3 wasd;
	Vector3_set(-joystick.x * delta, 0, joystick.y * delta, &wasd);
	struct Quaternion rotation;
	struct Vector3 up;
	Vector3_preset(VECTOR3_UP, &up);
	struct Vector3 lookVector;
	CameraController_getLookVector(&lookVector, x->cc);
	Quaternion_lookRotation(&lookVector, &up, &rotation);
	struct Vector3 finalMove;
	Quaternion_rotate(&rotation, &wasd, &finalMove);
	Vector3_multiply(&finalMove, 1 * delta, &finalMove);
	Vector3_add(&finalMove, &x->velocity, &x->velocity);
	struct Vector3 drag;
	Vector3_copy(&x->velocity, &drag);
	Vector3_multiply(&drag, 3 * delta, &drag);
	Vector3_sub(&x->velocity, &drag, &x->velocity);
	Vector3_add(&x->velocity, &x->entity->transform->position, &x->entity->transform->position);

	Collider* collisions = NULL;
	
	struct Collider* hash = (Collider*)ECS_getAllInstancesOfComponent(CTYPE_COLLIDER);
	collisions = Physics_findOverlappingAABBs(x->collider, hash);	// adding to new hash here ruins colliders- why?
	struct Collider* possibleCollision;
	for(possibleCollision = collisions; possibleCollision != NULL; possibleCollision = possibleCollision->collisionhh.next) {
		struct CollisionInfo collisionInfo;
		if(Physics_getCollisionInfo(x->collider, possibleCollision, &collisionInfo) == 1) {
			Vector3_copy(&collisionInfo.safePosition, &x->entity->transform->position);
			struct Vector3 velocityOnNormal;
			Vector3_project(&collisionInfo.normal, &x->velocity, &velocityOnNormal);
			//printf("Velocity is= "); Vector3_print(&x->velocity);
			//printf("Velocity on normal is= "); Vector3_print(&velocityOnNormal);
			Vector3_sub(&x->velocity, &velocityOnNormal, &x->velocity);
		}
	}
}

void PlayerController_lateupdate(PlayerController* x) {

}