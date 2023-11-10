#include "CSCIx229.h"
#include "vector3.h"
#include "Quaternion.h"
#include "ecs.h"
#include "uthash.h"
#include "input.h"
#include <math.h>
#include "includeallcomps.h"


double DIM = 2;
int windowLength = 800;
int windowHeight = 800;
double asp = 1;
int lockCursor = 0;

float current_fov = 170;
float current_dim = 3;

float deltaTime = 0;
float cumulativeTime = 0;
int mouseDeltaX = 0;
int mouseDeltaY = 0;

int shadingMode = GL_FLAT;

int th = 0;          //  Azimuth of view angle
int ph = 0;          //  Elevation of view angle

int axes = 1;

Quaternion objectRotation;
Entity* testEntity;

int useTestCamera = 0;
void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(shadingMode);

	Camera* cameras = (Camera*)ECS_getAllInstancesOfComponent(CTYPE_CAMERA);
	struct Camera* c = malloc(sizeof(Camera) * HASH_COUNT(cameras));
	int i = 0;
	for (c = cameras; c != NULL; c = c->hh.next) {
		if (i != 0) {
			printf("Multiple cameras being used.");

		}
		glLoadIdentity();
		Entity* e = ECS_getEntity(c->parent_id);
		e->transform->rotation = objectRotation;
		//current_dim = c->dim;
		current_fov = c->fov;
		glMatrixMode(GL_PROJECTION);
		Project(current_fov, asp, current_dim);			// this might have a problem?
		Vector3* position = &(e->transform->position);
		Vector3* positionLookTemp = Vector3_createPreset(FORWARD);
		Vector3* positionLook = Quaternion_createRotated(&(e->transform->rotation), positionLookTemp);
		free(positionLookTemp);
		Vector3_add(position, positionLook, positionLook);
		Vector3* upVector = Vector3_createPreset(UP);
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
		// Could probably combine these matrices
		GLdouble rotMatrix[16] = {qm[0], qm[1], qm[2], qm[3], qm[4], qm[5], qm[6], qm[7], qm[8], qm[9], qm[10], qm[11], qm[12], qm[13], qm[14], qm[15]};
		free(qm);
		Vector3* pos = &(e->transform->position);
		glTranslated(pos->x, pos->y, pos->z);
		glMultMatrixd(rotMatrix);
		Mesh_draw(m);

		glPopMatrix();
	}
	free(m);

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
	static double t0 = -1.;
	double dt, t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
	if (t0 < 0.0)
	  t0 = t;
	dt = t - t0;
	t0 = t;

	deltaTime = dt;
	cumulativeTime += deltaTime;

	// First run start functions on any new components
	ECS_runStarts();
	// Next, run updates
	ECS_runUpdates(deltaTime);
	// Next, draw scene to the screen
	glutPostRedisplay();
	// Next, run late updates
	ECS_runLateUpdates();
	// Input buffer can now be cleared
	Input_clearBuffer();
	
	// set test rotator
	Vector3 forward;
	Vector3_preset(FORWARD, &forward);
	Vector3 rotator;
	Vector3_set(cos(cumulativeTime), 0, sin(cumulativeTime), &rotator);
	Quaternion_fromToRotation(&forward, &rotator, &objectRotation);
	//Quaternion_setIdentity(&objectRotation);
}


void passive(int x, int y) {
	mouseDeltaX += x;
	mouseDeltaY += y;
	if (lockCursor) {
		glutSetCursor(GLUT_CURSOR_NONE);	// Can be optimized
		glutWarpPointer(windowLength / 2, windowHeight / 2);
	}
}

/*void lateUpdate() {
	glutTimerFunc(50, timer??, 0);
}*/


void reshape(int width, int height) {
	windowLength = width;
	windowHeight = height;
	glViewport(0, 0, RES * width, RES * height);
	glLoadIdentity();
	asp = (height > 0) ? (float)width / height : 1;
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
	glutReshapeFunc(reshape);
	glutKeyboardFunc(Input_setKeyDown);
	glutKeyboardUpFunc(Input_setKeyUp);
	glutPassiveMotionFunc(passive);
	glutIdleFunc(idle);
	ErrCheck("init");

	Entity* player = ECS_instantiate();						// creates a new entity with a transform component
	Vector3_set(2, 0.5, 0, &(player->transform->position));	// transform component is referenced in the entity struct for ease of access
	ECS_addComponent(player, CTYPE_COLLIDER);				// defaults to not having a collider type
	ECS_addComponent(player, CTYPE_ENTITYROTATOR);			// just rotates parent entity transform and changes its color to turquoise
	PlayerController* player_pController = ECS_addComponent(player, CTYPE_PLAYERCONTROLLER);
	Camera* player_camera = ECS_addComponent(player, CTYPE_CAMERA);		// defaults to the position + rotation of parent entity's transform
	Mesh* player_mesh = ECS_addComponent(player, CTYPE_MESH);			// defaults to white color and no mesh
	player_camera->fov = 100;
	player_camera->dim = 1;
	player_mesh->mesh_type = MESHTYPE_TEAPOT;
	player_pController->camera = player_camera;

	Entity* entity2 = ECS_instantiate();						// creates a new entity with a transform component
	Vector3_set(0, 0, 0, &(entity2->transform->position));	// transform component is referenced in the entity struct for ease of access
	ECS_addComponent(entity2, CTYPE_COLLIDER);				// defaults to not having a collider type
	ECS_addComponent(entity2, CTYPE_ENTITYROTATOR);			// just rotates parent entity transform and changes its color to turquoise
	Mesh* m2 = ECS_addComponent(entity2, CTYPE_MESH);			// defaults to white color and no mesh
	m2->mesh_type = MESHTYPE_TEAPOT;

	testEntity = player;

	glutMainLoop();
	return 0;
}