#ifndef ECS_H
#define ECS_H

#include <stdlib.h>
#include <math.h>
#include "uthash.h"
#include "Quaternion.h"

typedef enum ComponentType {
	CTYPE_MESH, CTYPE_COLLIDER, CTYPE_CAMERA, CTYPE_ENTITY, CTYPE_TRANSFORM,
	CTYPE_ENTITYROTATOR, CTYPE_PLAYERCONTROLLER, CTYPE_CAMERACONTROLLER
} ComponentType;

typedef struct Entity Entity;
typedef struct Transform Transform;

#define MACRO_ITERABLEHASHFIELDS \
int id; \
UT_hash_handle hh;

#define MACRO_COMPONENTFIELDS MACRO_ITERABLEHASHFIELDS \
 Entity* entity; \
 int parent_id; \
 int enabled;

struct Entity {
	int num_children;
	Transform* transform;
	int layer;
	
	MACRO_COMPONENTFIELDS
};

struct Transform {
	struct Vector3 position;
	struct Quaternion rotation;
	struct Vector3 scale;
	int relative;

	MACRO_COMPONENTFIELDS
};

typedef struct EventListener {
	void* component_ptr;
	ComponentType ctype;

	MACRO_ITERABLEHASHFIELDS
} EventListener;

void Transform_awake(Transform* t);
void Transform_update(float delta, Transform* t);
void Transform_lateupdate(Transform* t);
void Transform_start(Transform* t);
void Transform_hierarchyScale(Transform* t, Vector3* output);
void Transform_hierarchyRotation(Transform* t, Quaternion* output);
void Transform_hierarchyPosition(Transform* t, Vector3* output);
void Transform_hierarchyTransform(Transform* t, Transform* output);
void Transform_copy(Transform* t, Transform* out);

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
void* ECS_getComponentById(int id, ComponentType componentType);


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

void ECS_updateWorld();

/*
 * Returns hash of all instances of a given type. Should only be used in core engine functions like rendering and physics calculations.
 */
void* ECS_getAllInstancesOfComponent(ComponentType ctype);




#endif