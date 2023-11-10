#include "comp_entityrotator.h"
#include "ecs.h"
#include "comp_mesh.h"

void EntityRotator_update(float delta, EntityRotator* entityRotator) {
	Entity* e = ECS_getEntity(entityRotator->parent_id);
	Mesh* m = ECS_getComponent(e, CTYPE_MESH);
	if (m != NULL) {
		m->color[0] = 0.5;
	}
}

void EntityRotator_awake(EntityRotator* x) {
	// Blank
}

void EntityRotator_start(EntityRotator* x) {

}

void EntityRotator_lateupdate(EntityRotator* x) {

}