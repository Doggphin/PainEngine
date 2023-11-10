#include "ecs.h"
#include "includeallcomps.h"

// https://gamedev.stackexchange.com/questions/172584/how-could-i-implement-an-ecs-in-c

int component_id_counter = 0;

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
} ComponentHashes;
static struct ComponentHashes componenthashes;

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
    COMPONENTNAME* ECS_create ## COMPONENTNAME ## Component(int parent_id, int id) { \
    struct COMPONENTNAME* x = malloc(sizeof(struct COMPONENTNAME)); \
	COMPONENTNAME ##_awake(x); \
    x->parent_id = parent_id; \
	x->id = id; \
    HASH_ADD_INT((HASHNAME), parent_id, x);\
	ECS_createUpdateListener(x, id, CTYPE);\
	ECS_createStartListener(x, id, CTYPE);\
	ECS_createLateUpdateListener(x, id, CTYPE); \
    return x;\
}
#define MACRO_CASE_UPDATELISTENEROFCTYPE(COMPONENTNAME, CTYPE) \
case CTYPE: \
	ECS_update ## COMPONENTNAME ## Component(delta, updateListener); \
	break;
#define MACRO_CASE_LATEUPDATELISTENEROFCTYPE(COMPONENTNAME, CTYPE) \
case CTYPE: \
	ECS_lateupdate ## COMPONENTNAME ## Component(lateupdateListener); \
	break;
#define MACRO_CASE_STARTLISTENEROFCTYPE(COMPONENTNAME, CTYPE) \
case CTYPE: \
	ECS_start ## COMPONENTNAME ## Component(startListener); \
	break;

void ECS_createUpdateListener(void* component_ptr, int id, ComponentType ctype) {
	struct EventListener* u = malloc(sizeof(struct EventListener));
	u->component_ptr = component_ptr;
	u->ctype = ctype;
	u->id = id;
	HASH_ADD_INT((componenthashes.updateListeners), id, u);
}
void ECS_createLateUpdateListener(void* component_ptr, int id, ComponentType ctype) {
	struct EventListener* u = malloc(sizeof(struct EventListener));
	u->component_ptr = component_ptr;
	u->ctype = ctype;
	u->id = id;
	HASH_ADD_INT((componenthashes.lateupdateListeners), id, u);
}
void ECS_createStartListener(void* component_ptr, int id, ComponentType ctype) {
	struct EventListener* u = malloc(sizeof(struct EventListener));
	u->component_ptr = component_ptr;
	u->ctype = ctype;
	u->id = id;
	HASH_ADD_INT((componenthashes.startListeners), id, u);
}


MACRO_FUNCDEF_ECS_CREATEXCOMPONENT(EntityRotator, componenthashes.entityRotators, CTYPE_ENTITYROTATOR)
MACRO_FUNCDEF_ECS_CREATEXCOMPONENT(PlayerController, componenthashes.playerControllers, CTYPE_PLAYERCONTROLLER)
MACRO_FUNCDEF_ECS_CREATEXCOMPONENT(Transform, componenthashes.transforms, CTYPE_TRANSFORM)
MACRO_FUNCDEF_ECS_CREATEXCOMPONENT(Camera, componenthashes.cameras, CTYPE_CAMERA)
MACRO_FUNCDEF_ECS_CREATEXCOMPONENT(Mesh, componenthashes.meshes, CTYPE_MESH)
MACRO_FUNCDEF_ECS_CREATEXCOMPONENT(Collider, componenthashes.colliders, CTYPE_COLLIDER)
MACRO_FUNCDEF_ECS_CREATEXCOMPONENT(CameraController, componenthashes.cameraControllers, CTYPE_CAMERACONTROLLER)


Entity* ECS_createEntity(int parent_id, int id) {
	struct Entity* e = malloc(sizeof(struct Entity));
	e->parent_id = parent_id;
	e->id = id;
	e->num_children = 0;
	e->transform = ECS_createTransformComponent(parent_id, id);
	HASH_ADD_INT((componenthashes.entities), id, e);
	return e;
}


Entity* ECS_instantiate() {
	component_id_counter++;
	return ECS_createEntity(0, component_id_counter);
}

// WORK ON ME
void* ECS_getComponent(Entity* entity, ComponentType componentType) {
	switch (componentType) {
		case CTYPE_MESH:
			Mesh *mesh;
			HASH_FIND_INT((componenthashes.meshes), &(entity->id), mesh);
			return mesh;
			break;
		case CTYPE_COLLIDER:
			Collider *coll;
			HASH_FIND_INT((componenthashes.colliders), &(entity->id), coll);
			return coll;
			break;
		default:
			printf("Unrecognized component. \n");
	}
	return NULL;
}

// WORK ON ME
bool ECS_removeComponent(Entity* entity, ComponentType componentType) {
	if (entity == NULL) {
		return false;
	}
	switch (componentType) {
		case CTYPE_MESH:
			Mesh* removal = NULL;
			HASH_FIND_INT((componenthashes.meshes), &entity->id, removal);
			if (removal != NULL) {
				HASH_DEL((componenthashes.meshes), removal);
				free(removal);
				return true;
			}
			break;
	
		case CTYPE_COLLIDER:
			return true;

		default:
			printf("Unrecognized component.\n");
			return false;
			break;
	}
	return false;
}


void* ECS_addComponent(Entity* entity, ComponentType componentType) {

	component_id_counter++;
	switch (componentType) {
		case CTYPE_MESH:
			return ECS_createMeshComponent(entity->id, component_id_counter);
		case CTYPE_COLLIDER:
			return ECS_createColliderComponent(entity->id, component_id_counter);
		case CTYPE_CAMERA:
			return ECS_createCameraComponent(entity->id, component_id_counter);
		case CTYPE_ENTITYROTATOR:
			return ECS_createEntityRotatorComponent(entity->id, component_id_counter);
		case CTYPE_PLAYERCONTROLLER:
			return ECS_createPlayerControllerComponent(entity->id, component_id_counter);
		default:
			printf("Unrecognized component, or component cannot be added.\n");
			return NULL;
			break;
	}
}


void* ECS_getAllInstancesOfComponent(ComponentType ctype) {
	switch (ctype) {
		case CTYPE_CAMERA:
			return componenthashes.cameras;
		case CTYPE_MESH:
			return componenthashes.meshes;
		default:
			return NULL;
	}
}


Entity* ECS_getEntity(int id) {
	Entity* ret = NULL;
	HASH_FIND_INT((componenthashes.entities), &id, ret);
	return ret;
}


void ECS_runUpdates(float delta) {
	struct EventListener* updateListener, *tmp;
	HASH_ITER(hh, componenthashes.updateListeners, updateListener, tmp) {
		switch (updateListener->ctype) {
			MACRO_CASE_UPDATELISTENEROFCTYPE(PlayerController, CTYPE_PLAYERCONTROLLER)
			MACRO_CASE_UPDATELISTENEROFCTYPE(CameraController, CTYPE_CAMERACONTROLLER)
			MACRO_CASE_UPDATELISTENEROFCTYPE(EntityRotator, CTYPE_ENTITYROTATOR)
			default:
				//printf("Unused update function for CTYPE ID %d. Deleting listener.\n", (int)updateListener->ctype);
				HASH_DEL(componenthashes.updateListeners, updateListener);
				free(updateListener);
				break;
		}
	}
}


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


void ECS_runStarts() {
	struct EventListener* startListener, * tmp;
	HASH_ITER(hh, componenthashes.startListeners, startListener, tmp) {
		switch (startListener->ctype) {
			MACRO_CASE_STARTLISTENEROFCTYPE(CameraController, CTYPE_CAMERACONTROLLER)
			default:
				//printf("Unused start function for CTYPE ID %d.\n", (int)startListener->ctype);
				break;
		}
		HASH_DEL(componenthashes.startListeners, startListener);
		free(startListener);
	}
}
