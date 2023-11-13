#ifndef COMP_PLAYERCONTROLLER
#define COMP_PLAYERCONTROLLER

#include "comp_cameracontroller.h"
#include "ecs.h"
#include "vector2.h"
#include "comp_collider.h"

typedef struct PlayerController {
	CameraController* cc;
	Collider* collider;
	Vector3 velocity;
	int moveonce;
	
	MACRO_COMPONENTFIELDS
} PlayerController;

void PlayerController_awake(PlayerController* x);
void PlayerController_start(PlayerController* x);
void PlayerController_update(float delta, PlayerController* x);
void PlayerController_lateupdate(PlayerController* x);

#endif