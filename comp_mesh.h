 #ifndef COMP_MESH
#define COMP_MESH

#include "ecs.h"

typedef struct { float x, y; } coord;
typedef struct { int a, b, c; } tri;

typedef enum {
	MESHTYPE_NONE,
	MESHTYPE_CUSTOM,
	MESHTYPE_CUBE,
	MESHTYPE_TEAPOT,
	MESHTYPE_ARWING
} MeshType;

typedef struct Mesh {
	unsigned int texture_id;
	MeshType mesh_type;
	float* color;

	MACRO_COMPONENTFIELDS
} Mesh;


void Mesh_awake(Mesh* m);
void Mesh_update(float delta, Mesh* m);
void Mesh_lateupdate(Mesh* m);
void Mesh_start(Mesh* m);

/**
 * Draws this mesh to the screen. Assumes transformations have already been applied.
*/
void Mesh_draw(Mesh* m);
void Mesh_free(Mesh* m);

#endif