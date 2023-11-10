#include "comp_collider.h"

void Collider_awake(Collider* x) {
	Vector3_set(1, 1, 1, &(x->scale));
	x->shape = COLL_NONE;
}

void Collider_start(Collider* x) {

}

void Collider_update(float delta, Collider* x) {

}

void Collider_lateupdate(Collider* x) {

}