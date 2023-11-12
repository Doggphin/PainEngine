#ifndef COMP_PLAYERCONTROLLER
#define COMP_PLAYERCONTROLLER

#include "comp_camera.h"
#include "ecs.h"

typedef struct PlayerController {
	float lookX;
	float lookY;
	float joystickX;
	float joystickY;
	Camera* camera;

	MACRO_COMPONENTFIELDS
} PlayerController;

void PlayerController_awake(PlayerController* x);
void PlayerController_start(PlayerController* x);
void PlayerController_update(float delta, PlayerController* x);
void PlayerController_lateupdate(PlayerController* x);

#endif