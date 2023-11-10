#include "comp_playercontroller.h"
#include "ecs.h"
#include "input.h"

void PlayerController_init(int parent_id, int id, PlayerController* x) {
	x->parent_id = parent_id;
	x->id = id;
}

/* This runs immediatedly after the component is created.
*/
void PlayerController_start(PlayerController* x) {
	if (x->camera == NULL) {
		x->camera = ECS_getComponent(ECS_getEntity(x->parent_id), CTYPE_CAMERA);
		if (x->camera == NULL) {
			printf("No camera specified for PlayerController and no camera found on parent entity.");
		}
	}
}

void PlayerController_update(float delta, PlayerController* x) {
	if (Input_isKeyDown(KEYCODE_A, 0)) {
		printf("Key A pressed \n");
	}
}

void PlayerController_updatePointer(float delta, void* x) {
	PlayerController* playerController = (PlayerController*)x;
	PlayerController_update(delta, playerController);
}