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

/*
 * Creates a new entity.
 * All entities are instantiated with a transform component.
 * @return
 *      A pointer to the created entity.
 */
Entity* ECS_instantiate();


/*
 * Finds and returns an entity based on its id if found, otherwise returns NULL.
 */
Entity* ECS_getEntity(int parent_id);


/*
 * Queries a component for a component and returns it.
 * @param entity
 *		The entity to query.
 * @param componentType
 *		The type of component to search for.
 * @return
 *      A pointer to the component if found, otherwise NULL.
 */
void* ECS_getComponent(Entity* entity, ComponentType componentType);


/*
 * Attempts to delete a component attached to an entity.
 * @param entity
 *		The entity to query.
 * @param componentType
 *		The type of component to remove.
 * @return
 *      1 if component was successfully removed; 0 if component was not found.
 */
int ECS_removeComponent(Entity* entity, ComponentType componentType);


/*
 * Creates and adds a component to an entity.
 * @param entity
 *		The entity to query.
 * @param componentType
 *		The type of component to add.
 * @return
 *      A pointer to the component if created successfully; NULL if entity already has component of the given type.
 */
void* ECS_addComponent(Entity* entity, ComponentType componentType);


/*
 * Runs all update functions across all subscribed components.
 * @param delta
 *		The time that has passed between the last frame and now.
 */
void ECS_runUpdates(float delta);


/*
 * Runs all lateupdate functions across all subscribed components.
 */
void ECS_runLateUpdates();


/*
 * Runs all start functions across all subscribed components.
 */
void ECS_runStarts();


/*
 * Returns hash of all instances of a given type. Should only be used in core engine functions like rendering and physics calculations.
 */
void* ECS_getAllInstancesOfComponent(ComponentType ctype);

#endif