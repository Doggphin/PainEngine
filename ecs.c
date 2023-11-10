#include "ecs.h"
#include "comp_mesh.h"
#include "comp_entityrotator.h"
#include "comp_playercontroller.h"
#include "comp_transform.h"
#include "comp_camera.h"
#include "comp_collider.h"
#include "comp_cameracontroller.h"

// https://gamedev.stackexchange.com/questions/172584/how-could-i-implement-an-ecs-in-c

typedef struct ComponentHashes {
	struct Transform* transforms;
	struct Entity* entities;
	struct Mesh* meshes;
	struct Collider* colliders;
	struct Camera* cameras;
	struct EntityRotator* entityRotators;
	struct PlayerController* playerControllers;
	struct UpdateListener* updateListeners;
	struct CameraController* cameraControllers;
} ComponentHashes;

static ComponentHashes componenthashes = {};

int component_id_counter = 0;

#define MACRO_FUNCDEF_ECS_CREATEXCOMPONENT(COMPONENTNAME, HASHNAME) \
    COMPONENTNAME* ECS_create ## COMPONENTNAME ## Component(int parent_id, int id) { \
     struct COMPONENTNAME* x = malloc(sizeof(struct COMPONENTNAME)); \
	 COMPONENTNAME ##_awake(x); \
     x->parent_id = parent_id; x->id = id; \
     HASH_ADD_INT((HASHNAME), parent_id, x);\
     return x;\
    }\

// https://stackoverflow.com/questions/617554/override-a-function-call-in-c look into this for update functions and stuff
MACRO_FUNCDEF_ECS_CREATEXCOMPONENT(EntityRotator, componenthashes.entityRotators)
MACRO_FUNCDEF_ECS_CREATEXCOMPONENT(Transform, componenthashes.transforms)
MACRO_FUNCDEF_ECS_CREATEXCOMPONENT(Camera, componenthashes.cameras)
MACRO_FUNCDEF_ECS_CREATEXCOMPONENT(Mesh, componenthashes.meshes)
MACRO_FUNCDEF_ECS_CREATEXCOMPONENT(Collider, componenthashes.colliders)

Entity* ECS_createEntity(int parent_id, int id) {
	struct Entity* e = malloc(sizeof(struct Entity));
	e->parent_id = parent_id;
	e->id = id;
	e->num_children = 0;
	e->transform = ECS_createTransformComponent(parent_id, id);
	HASH_ADD_INT((componenthashes.entities), id, e);
	return e;
}

void ECS_createUpdateListener(void* component_ptr, ComponentType ctype) {
	component_id_counter++;
	struct UpdateListener* updateListener = malloc(sizeof(struct UpdateListener));
	updateListener->component_ptr = component_ptr;
	updateListener->id = component_id_counter;
	switch(ctype) {
		case CTYPE_PLAYERCONTROLLER:
			updateListener->fun_ptr = &PlayerController_updatePointer;
		default:
			break;
	}

	HASH_ADD_INT((componenthashes.updateListeners), id, updateListener);
}

PlayerController* ECS_createPlayerController(int parent_id, int id) {
	struct PlayerController* x = malloc(sizeof(struct PlayerController));
	x->parent_id = parent_id;
	x->id = id;
	PlayerController_init(parent_id, id, x);
	HASH_ADD_INT((componenthashes.playerControllers), parent_id, x);
	ECS_createUpdateListener(x, CTYPE_PLAYERCONTROLLER);
	return x;
}

Entity* ECS_instantiate() {
	component_id_counter++;
	return ECS_createEntity(0, component_id_counter);
}


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
			return ECS_createPlayerController(entity->id, component_id_counter);
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

void ECS_runUpdatesOfType(float delta) {
	struct UpdateListener* updateListener = malloc(sizeof(UpdateListener) * HASH_COUNT(componenthashes.updateListeners));
	for(updateListener = componenthashes.updateListeners; updateListener != NULL; updateListener = updateListener->hh.next) {
		updateListener->fun_ptr(delta, updateListener->component_ptr);
	}
	free(updateListener);
}
