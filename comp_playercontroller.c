#include "comp_playercontroller.h"
#include "ecs.h"
#include "input.h"
#include "Quaternion.h"
#include <stdlib.h>

void PlayerController_awake(PlayerController* x) {
	x->cc = NULL;
}

void PlayerController_start(PlayerController* x) {
	x->cc = ECS_getComponent(x->entity, CTYPE_CAMERACONTROLLER);
	if(x->cc == NULL) {printf("PlayerController requires component CameraController to function!\n");}
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
	Vector3_add(&finalMove, &x->entity->transform->position, &x->entity->transform->position);
}

void PlayerController_lateupdate(PlayerController* x) {

}