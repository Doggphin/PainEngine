#ifndef COMP_PLAYERCONTROLLER
#define COMP_PLAYERCONTROLLER

#define basedata int id; int parent_id; UT_hash_handle hh;
#include "comp_camera.h"
#include "uthash.h"

typedef struct BaseComponentData {
	int id;
	int parent_id;
	UT_hash_handle hh;
} BaseComponentData;

typedef struct PlayerController {
	float lookX;
	float lookY;
	float joystickX;
	float joystickY;
	Camera* camera;

	basedata
	/*
	int id;
	int parent_id;
	UT_hash_handle hh;
	*/
} PlayerController;

void PlayerController_init(int parent_id, int id, PlayerController* x);

void PlayerController_start(PlayerController* x);
void PlayerController_update(float delta, PlayerController* x);
void PlayerController_updatePointer(float delta, void* x);

#endif