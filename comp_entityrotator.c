#include "comp_entityrotator.h"
#include "ecs.h"
#include "comp_mesh.h"

void EntityRotator_update(float delta, EntityRotator* x) {
	x->totalTime += delta;
	Mesh* m = ECS_getComponent(x->entity, CTYPE_MESH);
	if (m != NULL) {
		Vector3_set(sin(x->totalTime) / 2 + .5, cos(x->totalTime / 1.414) / 2 + .5, cos(x->totalTime)  / 2 + .5, &m->color);
		Vector3_normalizeDirect(&m->color);
	}
	Vector3 forward;
    Vector3_preset(VECTOR3_FORWARD, &forward);
    Vector3 rotator;
    Vector3_set(cos(x->totalTime), 0, sin(x->totalTime), &rotator);
    Quaternion_fromToRotation(&forward, &rotator, &x->entity->transform->rotation);

	//Vector3_set(cos(x->totalTime / 3), sin(x->totalTime * 1.2) / 5, sin(x->totalTime / 3), &x->entity->transform->position);
}

void EntityRotator_awake(EntityRotator* x) {
	// Blank
}

void EntityRotator_start(EntityRotator* x) {

}

void EntityRotator_lateupdate(EntityRotator* x) {

}