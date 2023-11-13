#ifndef COMP_ENTITYROTATOR
#define COMP_ENTITYROTATOR

#include "ecs.h"

typedef struct EntityRotator {

	float totalTime;
	
	MACRO_COMPONENTFIELDS
} EntityRotator;

void EntityRotator_update(float t, EntityRotator* entityRotator);
void EntityRotator_awake(EntityRotator* x);
void EntityRotator_start(EntityRotator* x);
void EntityRotator_lateupdate(EntityRotator* x);

#endif