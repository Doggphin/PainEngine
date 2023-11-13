#include "ecs.h"
#include "includeallcomps.h"
#include "geometry.h"
#include "physics.h"

// https://gamedev.stackexchange.com/questions/172584/how-could-i-implement-an-ecs-in-c

int component_id_counter = 0;

/*
 * A table used to keep track of all instantiated components.
 */
typedef struct ComponentHashes {
	struct Transform* transforms;
	struct Entity* entities;
	struct Mesh* meshes;
	struct Collider* colliders;
	struct Camera* cameras;
	struct EntityRotator* entityRotators;
	struct PlayerController* playerControllers;
	struct EventListener* updateListeners;
	struct EventListener* startListeners;
	struct EventListener* lateupdateListeners;
	struct CameraController* cameraControllers;
	struct Rigidbody* rigidbodies;
} ComponentHashes;
/*
 * Singleton that contains references all instantiated components.
 */
static struct ComponentHashes componenthashes;


void Transform_awake(Transform* x) {
	Vector3_preset(VECTOR3_ZERO, &x->position);
	Vector3_preset(VECTOR3_ONE, &x->scale);
	Quaternion_setIdentity(&x->rotation);
	x->relative = 1;
}
void Transform_start(Transform* x) {

}
void Transform_update(float delta, Transform* x) {

}
void Transform_lateupdate(Transform* x) {

}
void Transform_copyData(Transform* t, Transform* output) {
	Vector3_copy(&t->position, &output->position);
	Vector3_copy(&t->scale, &output->scale);
	Quaternion_copy(&t->rotation, &output->rotation);
}
void Transform_hierarchyScale(Transform* t, Vector3* output) {
	Vector3_copy(&t->scale, output);
	if (t->relative == 0 || t->entity->parent_id == 0) { return; }
	struct Transform* transformBuffer = t->entity->entity->transform;
	while (1) {
		Vector3_multiplyEach(output, &transformBuffer->scale, output);
		if (transformBuffer->relative == 0 || transformBuffer->entity->parent_id == 0)
		{
			return;
		}
		else {
			transformBuffer = transformBuffer->entity->transform;
		}
	}
}
void Transform_hierarchyRotation(Transform* t, Quaternion* output) {
	Quaternion_copy(&t->rotation, output);
	if (t->relative == 0 || t->entity->parent_id == 0) { return; }
	struct Transform* transformBuffer = t->entity->entity->transform;
	while (1) {
		Quaternion_multiply(output, &transformBuffer->rotation, output);
		if (transformBuffer->relative == 0 || transformBuffer->entity->parent_id == 0) {
			 return;
			  }
		else {
			transformBuffer = transformBuffer->entity->transform;
		}
	}
}
void Transform_hierarchyPosition(Transform* t, Vector3* output) {
	Vector3_copy(&t->position, output);
	if (t->relative == 0 || t->entity->parent_id == 0) { return; }
	struct Transform* transformBuffer = t->entity->entity->transform;
	while (1) {
		Vector3_add(output, &transformBuffer->position, output);
		if (transformBuffer->relative == 0 || transformBuffer->entity->parent_id == 0) { return; }
		else {
			transformBuffer = transformBuffer->entity->transform;
		}
	}
}
void Transform_hierarchyTransform(Transform* t, Transform* output) {
	Transform_hierarchyScale(t, &output->scale);
	Transform_hierarchyPosition(t, &output->position);
	Transform_hierarchyRotation(t, &output->rotation);
}

// Private functions to add event listeners to newly instantiated components
/*
 * Subscribes a component to update events.
 */
void ECS_createUpdateListener(void* component_ptr, int id, ComponentType ctype) {
	struct EventListener* u = malloc(sizeof(struct EventListener));
	u->component_ptr = component_ptr;
	u->ctype = ctype;
	u->id = id;
	HASH_ADD_INT((componenthashes.updateListeners), id, u);
}
/*
 * Subscribes a component to lateupdate events.
 */
void ECS_createLateUpdateListener(void* component_ptr, int id, ComponentType ctype) {
	struct EventListener* u = malloc(sizeof(struct EventListener));
	u->component_ptr = component_ptr;
	u->ctype = ctype;
	u->id = id;
	HASH_ADD_INT((componenthashes.lateupdateListeners), id, u);
}
/*
 * Subscribes a component to a start event.
 */
void ECS_createStartListener(void* component_ptr, int id, ComponentType ctype) {
	struct EventListener* u = malloc(sizeof(struct EventListener));
	u->component_ptr = component_ptr;
	u->ctype = ctype;
	u->id = id;
	HASH_ADD_INT((componenthashes.startListeners), id, u);
}


#define MACRO_FUNCDEF_ECS_CREATEXCOMPONENT(COMPONENTNAME, HASHNAME, CTYPE) \
	void ECS_update ## COMPONENTNAME ## Component(float delta, EventListener* listener) { \
		COMPONENTNAME ## _update(delta, (COMPONENTNAME*)listener->component_ptr); \
	} \
	void ECS_lateupdate ## COMPONENTNAME ## Component(EventListener* listener) { \
		COMPONENTNAME ## _lateupdate((COMPONENTNAME*)listener->component_ptr); \
	} \
	void ECS_start ## COMPONENTNAME ## Component(EventListener* listener) { \
		COMPONENTNAME ## _start((COMPONENTNAME*)listener->component_ptr); \
	} \
	\
    COMPONENTNAME* ECS_create ## COMPONENTNAME ## Component(Entity* entity, int id) { \
    struct COMPONENTNAME* x = malloc(sizeof(struct COMPONENTNAME)); \
    x->parent_id = entity->id; \
	x->id = id; \
	x->entity = entity; \
	x->enabled = entity->enabled; \
    HASH_ADD(hh, (HASHNAME), parent_id, sizeof(int), x);\
	ECS_createUpdateListener(x, id, CTYPE);\
	ECS_createStartListener(x, id, CTYPE);\
	ECS_createLateUpdateListener(x, id, CTYPE); \
    return x;\
}

// Macros to define create(), start(), update() and lateupdate() functions
MACRO_FUNCDEF_ECS_CREATEXCOMPONENT(EntityRotator, componenthashes.entityRotators, CTYPE_ENTITYROTATOR)
MACRO_FUNCDEF_ECS_CREATEXCOMPONENT(PlayerController, componenthashes.playerControllers, CTYPE_PLAYERCONTROLLER)
MACRO_FUNCDEF_ECS_CREATEXCOMPONENT(Transform, componenthashes.transforms, CTYPE_TRANSFORM)
MACRO_FUNCDEF_ECS_CREATEXCOMPONENT(Camera, componenthashes.cameras, CTYPE_CAMERA)
MACRO_FUNCDEF_ECS_CREATEXCOMPONENT(Mesh, componenthashes.meshes, CTYPE_MESH)
MACRO_FUNCDEF_ECS_CREATEXCOMPONENT(Collider, componenthashes.colliders, CTYPE_COLLIDER)
MACRO_FUNCDEF_ECS_CREATEXCOMPONENT(CameraController, componenthashes.cameraControllers, CTYPE_CAMERACONTROLLER)
MACRO_FUNCDEF_ECS_CREATEXCOMPONENT(Rigidbody, componenthashes.rigidbodies, CTYPE_RIGIDBODY)

/*
 * Creates an entity of a given parent ID and personal ID.
 */
Entity* ECS_createEntity(int parent_id, int id) {
	struct Entity* e = malloc(sizeof(struct Entity));
	e->parent_id = parent_id;
	e->id = id;
	e->num_children = 0;
	component_id_counter++;
	e->transform = ECS_createTransformComponent(e, component_id_counter);
	HASH_ADD_INT((componenthashes.entities), id, e);
	return e;
}
Entity* ECS_instantiate() {
	component_id_counter++;
	return ECS_createEntity(0, component_id_counter);
}


#define MACRO_CASE_GETCOMPONENTOFCTYPE(COMPONENTNAME, HASHNAME, CTYPE) \
case CTYPE: \
	COMPONENTNAME* ge ## COMPONENTNAME ## t = malloc(sizeof(COMPONENTNAME)); \
	HASH_FIND_INT((HASHNAME), &entityid, ge ## COMPONENTNAME ## t); \
	return ge ## COMPONENTNAME ## t;
void* ECS_getComponentById(int entityid, ComponentType componentType) {
	switch (componentType) {
		MACRO_CASE_GETCOMPONENTOFCTYPE(Camera, componenthashes.cameras, CTYPE_CAMERA)
		MACRO_CASE_GETCOMPONENTOFCTYPE(Transform, componenthashes.transforms, CTYPE_TRANSFORM)
		MACRO_CASE_GETCOMPONENTOFCTYPE(Mesh, componenthashes.meshes, CTYPE_MESH)
		MACRO_CASE_GETCOMPONENTOFCTYPE(EntityRotator, componenthashes.entityRotators, CTYPE_ENTITYROTATOR)
		MACRO_CASE_GETCOMPONENTOFCTYPE(PlayerController, componenthashes.playerControllers, CTYPE_PLAYERCONTROLLER)
		MACRO_CASE_GETCOMPONENTOFCTYPE(CameraController, componenthashes.cameraControllers, CTYPE_CAMERACONTROLLER)
		MACRO_CASE_GETCOMPONENTOFCTYPE(Collider, componenthashes.colliders, CTYPE_COLLIDER)
		MACRO_CASE_GETCOMPONENTOFCTYPE(Rigidbody, componenthashes.rigidbodies, CTYPE_RIGIDBODY)
		case CTYPE_ENTITY:
			printf("Cannot use GetComponent to retrieve entities.");
			return NULL;
	}
	return NULL;
}
void* ECS_getComponent(Entity* entity, ComponentType componentType) {
	return ECS_getComponentById(entity->id, componentType);
}


#define MACRO_CASE_REMOVECOMPONENTOFCTYPE(COMPONENTNAME, HASHNAME, CTYPE) \
case CTYPE: \
	COMPONENTNAME* rem ## COMPONENTNAME ## oval = NULL; \
	HASH_FIND_INT((HASHNAME), &(entity->id), rem ## COMPONENTNAME ## oval); \
	if (rem ## COMPONENTNAME ## oval != NULL) { \
			HASH_DEL((HASHNAME), rem ## COMPONENTNAME ## oval); \
			free(rem ## COMPONENTNAME ## oval); \
			return 1; \
	} \
	break;
int ECS_removeComponent(Entity* entity, ComponentType componentType) {
	if (entity == NULL) {
		return 0;
	}
	switch (componentType) {
		MACRO_CASE_REMOVECOMPONENTOFCTYPE(Camera, componenthashes.cameras, CTYPE_CAMERA)
		MACRO_CASE_REMOVECOMPONENTOFCTYPE(Transform, componenthashes.transforms, CTYPE_TRANSFORM)
		MACRO_CASE_REMOVECOMPONENTOFCTYPE(Mesh, componenthashes.meshes, CTYPE_MESH)
		MACRO_CASE_REMOVECOMPONENTOFCTYPE(EntityRotator, componenthashes.entityRotators, CTYPE_ENTITYROTATOR)
		MACRO_CASE_REMOVECOMPONENTOFCTYPE(PlayerController, componenthashes.playerControllers, CTYPE_PLAYERCONTROLLER)
		MACRO_CASE_REMOVECOMPONENTOFCTYPE(CameraController, componenthashes.cameraControllers, CTYPE_CAMERACONTROLLER)
		MACRO_CASE_REMOVECOMPONENTOFCTYPE(Collider, componenthashes.colliders, CTYPE_COLLIDER)
		MACRO_CASE_REMOVECOMPONENTOFCTYPE(Rigidbody, componenthashes.rigidbodies, CTYPE_RIGIDBODY)
	case CTYPE_ENTITY:
		printf("Entity components must be removed through the destroy function.");
	}
	return 0;
}


#define MACRO_CASE_ADDCOMPONENTOFCTYPE(COMPONENTNAME, CTYPE) \
case CTYPE: \
	if(ECS_getComponent(entity, CTYPE) == NULL) { \
		ret = ECS_create ## COMPONENTNAME ## Component(entity, component_id_counter); \
		COMPONENTNAME ## _awake(ret); \
		break; \
	} else { \
		return NULL; \
	}
	
void* ECS_addComponent(Entity* entity, ComponentType componentType) {
	void* ret = NULL;
	component_id_counter++;
	switch (componentType) {
		MACRO_CASE_ADDCOMPONENTOFCTYPE(Mesh, CTYPE_MESH)
		MACRO_CASE_ADDCOMPONENTOFCTYPE(Collider, CTYPE_COLLIDER)
		MACRO_CASE_ADDCOMPONENTOFCTYPE(Camera, CTYPE_CAMERA)
		MACRO_CASE_ADDCOMPONENTOFCTYPE(EntityRotator, CTYPE_ENTITYROTATOR)
		MACRO_CASE_ADDCOMPONENTOFCTYPE(PlayerController, CTYPE_PLAYERCONTROLLER)
		MACRO_CASE_ADDCOMPONENTOFCTYPE(CameraController, CTYPE_CAMERACONTROLLER)
		MACRO_CASE_ADDCOMPONENTOFCTYPE(Rigidbody, CTYPE_RIGIDBODY)

		case CTYPE_ENTITY:
			printf("Entity components must be added through the instantiate function.\n");
			return NULL;
		case CTYPE_TRANSFORM:
			printf("Transform components are already attached to entities by default.\n");
			return NULL;
	}
	if (ret == NULL) {
		printf("Entity already has component attached. Returning NULL.\n");
	}
	return ret;
}


void* ECS_getAllInstancesOfComponent(ComponentType ctype) {
	switch (ctype) {
		case CTYPE_CAMERA:
			return componenthashes.cameras;
		case CTYPE_MESH:
			return componenthashes.meshes;
		case CTYPE_COLLIDER:
			return componenthashes.colliders;
		default:
			printf("This component hash has no reason to be iterated over and has not been implemented..");
			return NULL;
	}
}


Entity* ECS_getEntity(int id) {
	Entity* ret = NULL;
	HASH_FIND_INT((componenthashes.entities), &id, ret);
	return ret;
}

void ECS_updateWorld() {
	struct Collider* collider, *tmp;
	HASH_ITER(hh, componenthashes.colliders, collider, tmp) {
		Collider_updateWorldShape(collider);
	}
}


#define MACRO_CASE_STARTLISTENEROFCTYPE(COMPONENTNAME, CTYPE) \
case CTYPE: \
	ECS_start ## COMPONENTNAME ## Component(startListener); \
	break;
void ECS_runStarts() {
	struct EventListener* startListener, * tmp;
	HASH_ITER(hh, componenthashes.startListeners, startListener, tmp) {
		switch (startListener->ctype) {
			MACRO_CASE_STARTLISTENEROFCTYPE(CameraController, CTYPE_CAMERACONTROLLER)
			MACRO_CASE_STARTLISTENEROFCTYPE(PlayerController, CTYPE_PLAYERCONTROLLER)
			MACRO_CASE_STARTLISTENEROFCTYPE(Rigidbody, CTYPE_RIGIDBODY)
			default:
				//printf("Unused start function for CTYPE ID %d.\n", (int)startListener->ctype);
				break;
		}
		HASH_DEL(componenthashes.startListeners, startListener);
		free(startListener);
	}
}


#define MACRO_CASE_UPDATELISTENEROFCTYPE(COMPONENTNAME, CTYPE) \
case CTYPE: \
	ECS_update ## COMPONENTNAME ## Component(delta, updateListener); \
	break;
void ECS_runUpdates(float delta) {
	struct EventListener* updateListener, *tmp;
	HASH_ITER(hh, componenthashes.updateListeners, updateListener, tmp) {
		switch (updateListener->ctype) {
			MACRO_CASE_UPDATELISTENEROFCTYPE(PlayerController, CTYPE_PLAYERCONTROLLER)
			MACRO_CASE_UPDATELISTENEROFCTYPE(EntityRotator, CTYPE_ENTITYROTATOR)
			MACRO_CASE_UPDATELISTENEROFCTYPE(Rigidbody, CTYPE_RIGIDBODY)
			default:
				//printf("Unused update function for CTYPE ID %d. Deleting listener.\n", (int)updateListener->ctype);
				HASH_DEL(componenthashes.updateListeners, updateListener);
				free(updateListener);
				break;
		}
	}
}


#define MACRO_CASE_LATEUPDATELISTENEROFCTYPE(COMPONENTNAME, CTYPE) \
case CTYPE: \
	ECS_lateupdate ## COMPONENTNAME ## Component(lateupdateListener); \
	break;
void ECS_runLateUpdates() {
	struct EventListener* lateupdateListener, * tmp;
	HASH_ITER(hh, componenthashes.lateupdateListeners, lateupdateListener, tmp) {
		switch (lateupdateListener->ctype) {
			//MACRO_CASE_LATEUPDATELISTENEROFCTYPE(CameraController, CTYPE_CAMERACONTROLLER)
			default:
				//printf("Unused lateUpdate function for CTYPE ID %d. Deleting listener.\n", (int)lateupdateListener->ctype);
				HASH_DEL(componenthashes.lateupdateListeners, lateupdateListener);
				free(lateupdateListener);
				break;
		}
	}
}

