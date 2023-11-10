#ifndef ECS_H
#define ECS_H

#include "macros.h"
#include "comp_transform.h"
#include <stdlib.h>
#include <math.h>

typedef enum ComponentType {
	CTYPE_MESH, CTYPE_COLLIDER, CTYPE_CAMERA, CTYPE_ENTITY, CTYPE_TRANSFORM,
	CTYPE_ENTITYROTATOR, CTYPE_PLAYERCONTROLLER, CTYPE_CAMERACONTROLLER
} ComponentType;

typedef struct EventListener {
	void* component_ptr;
	ComponentType ctype;

	MACRO_COMPONENTFIELDS
} EventListener;

typedef struct Entity {
	Transform* transform;
	int num_children;

	MACRO_COMPONENTFIELDS
} Entity;

Entity* ECS_instantiate();
Entity* ECS_getEntity(int parent_id);
void* ECS_getComponent(Entity* entity, ComponentType componentType);
bool ECS_removeComponent(Entity* entity, ComponentType componentType);
void* ECS_addComponent(Entity* entity, ComponentType componentType);
void ECS_runUpdates(float delta);
void ECS_runLateUpdates();
void ECS_runStarts();
void* ECS_getAllInstancesOfComponent(ComponentType ctype);

#endif