#ifndef ECS_H
#define ECS_H

#include "macros.h"
#include "comp_transform.h"
#include <stdlib.h>
#include <assert.h>
#include <math.h>

typedef enum ComponentType {
	CTYPE_MESH, CTYPE_COLLIDER, CTYPE_CAMERA, CTYPE_ENTITY, CTYPE_TRANSFORM,
	CTYPE_ENTITYROTATOR, CTYPE_PLAYERCONTROLLER, CTYPE_CAMERACONTROLLER
} ComponentType;

typedef struct UpdateListener {
	void (*fun_ptr)(float, void*);
	void* component_ptr;

	MACRO_ITERABLEHASHFIELDS
} UpdateListener;

typedef struct ComponentReferences {
	ComponentType type;
	void (*fun_ptr)(int, int);
	void (*start_ptr)(void*);
	void (*update_ptr)(float, void*);
} ComponentReferences;

typedef struct StartListener {
	void(*fun_ptr);

	MACRO_COMPONENTFIELDS
} StartListener;

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
void ECS_runUpdatesOfType(float delta);
void* ECS_getAllInstancesOfComponent(ComponentType ctype);

#endif