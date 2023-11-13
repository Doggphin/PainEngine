#include "CSCIx229.h"
#include "vector3.h"
#include "vector2.h"
#include "Quaternion.h"
#include "ecs.h"
#include "uthash.h"
#include "input.h"
#include "window.h"
#include "includeallcomps.h"
#include <math.h>


double DIM = 2;
int windowLength = 800;
int windowHeight = 800;

float current_fov = 170;
float current_dim = 3;

float deltaTime = 0;
float cumulativeTime = 0;
int mouseDeltaX = 0;
int mouseDeltaY = 0;

int shadingMode = GL_FLAT;

int axes = 1;
int drawColliderGizmos = 1;


Entity* testEntity;

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(shadingMode);

	Camera* cameras = (Camera*)ECS_getAllInstancesOfComponent(CTYPE_CAMERA);
	struct Camera* c = malloc(sizeof(Camera) * HASH_COUNT(cameras));
	int i = 0;
	for (c = cameras; c != NULL; c = c->hh.next) {
		if (i != 0) {
			printf("Multiple cameras being used.\n");
		}
		glLoadIdentity();
		Entity* e = ECS_getEntity(c->parent_id);
		//current_dim = c->dim;
		current_fov = c->fov;
		glMatrixMode(GL_PROJECTION);
		Project(current_fov, Window_getAspectRatio(), current_dim);
		Vector3* position = &(e->transform->position);
		Vector3* positionLookTemp = Vector3_createPreset(VECTOR3_FORWARD);
		Vector3* positionLook = Quaternion_createRotated(&(e->transform->rotation), positionLookTemp);
		free(positionLookTemp);
		Vector3_add(position, positionLook, positionLook);
		Vector3* upVector = Vector3_createPreset(VECTOR3_UP);
		gluLookAt(position->x, position->y, position->z, positionLook->x, positionLook->y, positionLook->z, upVector->x, upVector->y, upVector->z);
		free(positionLook); free(upVector);
		i++;
	}
	free(c);

	Mesh* meshes = (Mesh*)ECS_getAllInstancesOfComponent(CTYPE_MESH);
	struct Mesh* m = malloc(sizeof(Mesh) * HASH_COUNT(meshes));
	for (m = meshes; m != NULL; m = m->hh.next) {
		glPushMatrix();

		struct Entity* e = ECS_getEntity(m->parent_id);
		float* qm = Quaternion_toMat4(&(e->transform->rotation));
		GLdouble rotMatrix[16] = {qm[0], qm[1], qm[2], qm[3], qm[4], qm[5], qm[6], qm[7], qm[8], qm[9], qm[10], qm[11], qm[12], qm[13], qm[14], qm[15]};
		free(qm);
		Vector3* pos = &(e->transform->position);
		glTranslated(pos->x, pos->y, pos->z);
		glMultMatrixd(rotMatrix);
		Mesh_draw(m);

		glPopMatrix();
	}
	free(m);

	Collider* colliders = (Collider*)ECS_getAllInstancesOfComponent(CTYPE_COLLIDER);
	struct Collider* col = malloc(sizeof(Collider) * HASH_COUNT(colliders));
	for (col = colliders; col != NULL; col = col->hh.next) {
		glPushMatrix();

		AABB* aabb = col->AABB;

		printf("The aabb minimum is: ");
		Vector3_print(&aabb->min);
		printf("The aabb extents are: ");
		Vector3_print(&aabb->extents);

		glColor3f(1, 0, 0);
		glBegin(GL_LINE_STRIP);
		glVertex3d(aabb->min.x, aabb->min.y, aabb->min.z);
		glVertex3d(aabb->min.x + aabb->extents.x, aabb->min.y, aabb->min.z);
		glVertex3d(aabb->min.x + aabb->extents.x, aabb->min.y + aabb->extents.y, aabb->min.z);
		glVertex3d(aabb->min.x, aabb->min.y + aabb->extents.y, aabb->min.z);
		glVertex3d(aabb->min.x, aabb->min.y, aabb->min.z);
		glVertex3d(aabb->min.x, aabb->min.y, aabb->min.z + aabb->extents.z);
		glVertex3d(aabb->min.x, aabb->min.y + aabb->extents.y, aabb->min.z + aabb->extents.z);
		glVertex3d(aabb->min.x, aabb->min.y + aabb->extents.y, aabb->min.z);
		glEnd();
		glBegin(GL_LINES);
		glVertex3d(aabb->max.x, aabb->max.y, aabb->max.z);
		glVertex3d(aabb->max.x - aabb->extents.x, aabb->max.y, aabb->max.z);
		glVertex3d(aabb->max.x, aabb->max.y, aabb->max.z);
		glVertex3d(aabb->max.x, aabb->max.y - aabb->extents.y, aabb->max.z);
		glVertex3d(aabb->max.x, aabb->max.y, aabb->max.z);
		glVertex3d(aabb->max.x, aabb->max.y, aabb->max.z - aabb->extents.z);
		glVertex3d(aabb->max.x, aabb->max.y - aabb->extents.y, aabb->max.z);
		glVertex3d(aabb->max.x, aabb->max.y - aabb->extents.y, aabb->max.z - aabb->extents.z);
		glVertex3d(aabb->max.x, aabb->max.y - aabb->extents.y, aabb->max.z);
		glVertex3d(aabb->max.x - aabb->extents.x, aabb->max.y - aabb->extents.y, aabb->max.z);
		

		glEnd();

		glPopMatrix();
	}
	//free(col);

	glColor3f(1, 1, 1);
	if (axes)
	{
		const double len = 1.5;
		glBegin(GL_LINES);
		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d(len, 0.0, 0.0);
		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d(0.0, len, 0.0);
		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d(0.0, 0.0, len);
		glEnd();
		//  Label axes
		glRasterPos3d(len, 0.0, 0.0);
		Print("X");
		glRasterPos3d(0.0, len, 0.0);
		Print("Y");
		glRasterPos3d(0.0, 0.0, len);
		Print("Z");
	}

	ErrCheck("display");
	glFlush();
	glutSwapBuffers();
}

void idle() {
	// Do time thingies
	static double t0 = -1.;
	double dt, t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
	if (t0 < 0.0)
	  t0 = t;
	dt = t - t0;
	t0 = t;
	deltaTime = dt;
	cumulativeTime += deltaTime;

	// Run game loop step
	ECS_runStarts();				// First run start functions on any newly instantiated components
	ECS_updateWorld();
	//Vector3_print(&(((Collider*)ECS_getComponent(testEntity, CTYPE_COLLIDER))->AABB->extents));
	ECS_runUpdates(deltaTime);		// Next, run updates on all subscribed components
	Input_clearMouseDelta();		// Clear mouse delta buffer after updates are run
	glutPostRedisplay();			// Next, draw scene to the screen
	ECS_runLateUpdates();			// Next, run late updates on all subscribed components
	Input_setBufferToCurrent();			// Clear buffered inputs for next frame
}


void initScene() {
	Entity* player = ECS_instantiate();
	Vector3_set(-2, 0.5, 0, &(player->transform->position));
	Camera* player_camera = ECS_addComponent(player, CTYPE_CAMERA);
	ECS_addComponent(player, CTYPE_CAMERACONTROLLER);
	ECS_addComponent(player, CTYPE_PLAYERCONTROLLER);
	player_camera->fov = 100;
	player_camera->dim = 1;

	Entity* entity2 = ECS_instantiate();
	Vector3_set(0, 0, 0, &(entity2->transform->position));
	ECS_addComponent(entity2, CTYPE_ENTITYROTATOR);
	Mesh* m2 = ECS_addComponent(entity2, CTYPE_MESH);
	m2->mesh_type = MESHTYPE_TEAPOT;

	Collider* e2c = ECS_addComponent(entity2, CTYPE_COLLIDER);
	e2c->shapeStruct = Geometry_createSphere(.4);
	e2c->shape = SHAPE_SPHERE;
	

	Input_setCursorLocked(1);
	testEntity = player;

	ECS_updateWorld();
}


int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(windowLength, windowHeight);
	glutCreateWindow("PainEngine");
#ifdef USEGLEW
	//  Initialize GLEW
	if (glewInit() != GLEW_OK) Fatal("Error initializing GLEW\n");
#endif
	glutDisplayFunc(display);
	glutReshapeFunc(Window_reshape);
	glutKeyboardFunc(Input_setKeyDown);
	glutKeyboardUpFunc(Input_setKeyUp);
	glutPassiveMotionFunc(Input_passive);
	glutIdleFunc(idle);
	ErrCheck("init");

	initScene();

	glutMainLoop();
	return 0;
}