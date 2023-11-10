#include "comp_mesh.h"
#include "CSCIx229.h"
#include "vector3.h"
#include "Quaternion.h"

void Mesh_awake(Mesh* m) {
	m->texture_id = 0;
	m->mesh_type = MESHTYPE_NONE;
    m->color = malloc(sizeof(float) * 3);   
    m->color[0] = 1;
    m->color[1] = 1;
    m->color[2] = 1;
}

void Mesh_start(Mesh* m) {

}

void Mesh_update(float delta, Mesh* m) {

}

void Mesh_lateupdate(Mesh* m) {

}

#pragma region arwing

const Vector3 ar_xyz[] =
{
    {9, .25, 5},            //0
    { 4, 0.5, -2},
    { 2, 0, 0},
    {2.5, -.5, -1.5},
    { -9, .25, 5},
    { -4, 0.5, -2},
    {-2, 0, 0},
    {-2.5, -.5, -1.5},

    {3, 3, 3},
    {2, .5, 2},
    {2, -.5, -2},           // 10
    {-3, 3, 3},             // 11
    {-2, .5, 2},            // 12
    {-2, -.5, -2 },         // 13

    { 0, .63, 2 },          // 14
    { 0, 1.25, 0 },         // 15
    { 0, 0, -8 },           // 16
    { 0, -.5, 0 },          // 17
    { 0, 0, 1.5 },          // 18
    { 2, 0, 0 },            // 19
    { -2, 0, 0 },           // 20
    { 0, -.5, 0 },          // 21
    { 2, 0, 0 },            // 22
    { 0, 1.25, 0 },         // 23

    { -2, 0, 0 },           // 24
    { -.72, .2, 0.1 },      // 25
    { 0, .66, 0.1 },        // 26
    { .72, .2, 0.1 },       // 27
    { 0, 0, 0.1 },          // 28

    {3.01, 3, 3},           // 29
    {2.01, .5, 2},          // 30
    {2.01, -.5, -2},        // 31
    {-3.01, 3, 3},          // 32
    {-2.01, .5, 2},         // 33
    {-2.01, -.5, -2 },      // 34

    { 0, .63, 1.99},        // 35
    { 0, 0, 1.49 },         // 36
};

const coord ar_uv[] = {
{0.480, 0.021}, {0.480, 0.648}, {0.273, 0.608},
{0.480, 0.648}, {0.273, 0.608}, {0.370, 0.690},
{0.230, 0.692}, {0.021, 0.102}, {0.122, 0.021},
{0.230, 0.692}, {0.122, 0.021}, {0.230, 0.064},
{0.480, 0.021}, {0.273, 0.608}, {0.480, 0.648},
{0.273, 0.608}, {0.370, 0.690}, {0.480, 0.648},
{0.230, 0.692}, {0.122, 0.021}, {0.021, 0.102},
{0.230, 0.692}, {0.230, 0.064}, {0.122, 0.021},

{0.857, 0.021}, {0.847, 0.164}, {0.718, 0.174},
{0.847, 0.164}, {0.857, 0.762}, {0.718, 0.174},
{0.629, 0.021}, {0.675, 0.609}, {0.523, 0.619},
{0.675, 0.722}, {0.523, 0.619}, {0.675, 0.609},
{0.857, 0.021}, {0.718, 0.174}, {0.847, 0.164},
{0.847, 0.164}, {0.718, 0.174}, {0.857, 0.762},
{0.629, 0.021}, {0.523, 0.619}, {0.675, 0.609},
{0.675, 0.722}, {0.675, 0.609}, {0.523, 0.619},

{0.273, 0.801}, {0.382, 0.905}, {0.305, 0.781},
{0.382, 0.905}, {0.382, 0.733}, {0.345, 0.756},

{0.111, 0.936}, {0.021, 0.848}, {0.111, 0.735},
{0.111, 0.936}, {0.021, 0.848}, {0.111, 0.735},

{0.616, 0.947}, {0.616, 0.765}, {0.523, 0.883},
{0.616, 0.947}, {0.616, 0.765}, {0.523, 0.883},
};

const tri ar_idx[] =
{
    {0, 2, 1},  //c
    {1, 2, 3},  //c
    {0, 1, 3},  //c
    {0, 3, 2},  //c right
    {4, 5, 6},  //c
    {5, 7, 6},  //c
    {4, 7, 5},  //c
    {4, 6, 7},  //c left

    {14, 15, 19},   //c
    {15, 16, 19},   //c
    {16, 17, 19},   //c
    {18, 19, 17},   //c right
    {14, 20, 15},   //c
    {15, 20, 16},   //c
    {16, 20, 17},   //c
    {18, 17, 20},   //c left

    {24, 22, 21},   //c
    {24, 23, 22},   //c

    {35, 19, 15},   //c
    {15, 20, 35},   //c

    {36, 17, 19},   //c
    {36, 20, 17},   //c
};

const tri ar_idx2[] =
{
    {8, 9, 10},
    {11, 13, 12},
    {29, 31, 30},
    {32, 33, 34}
};

const coord ar_uv2[] = {
    {0.975, 0.873}, {0.900, 0.756},  {0.975, 0.464},
    {0.975, 0.873}, {0.975, 0.464}, {0.900, 0.756},
     {0.979, 0.021}, {0.979, 0.421},{0.900, 0.133},
    {0.979, 0.021}, {0.900, 0.133}, {0.979, 0.421},
};

const tri ar_idx3[] =
{
    {25, 27, 28},
    {25, 26, 27}
};
const coord ar_uv3[] = {
    {0.0, 0.0}, {0.001, 0}, {0.0, 0.001},
};

#pragma endregion arwing

void Mesh_drawTriangle(Vector3 A, Vector3 B, Vector3 C, coord D, coord E, coord F)
{
    float dx0 = 0;
    float dy0 = 0;
    float dz0 = 0;

    float dx1 = 0;
    float dy1 = 0;
    float dz1 = 0;

    //  Planar vector 0
    dx0 = A.x - B.x;
    dy0 = A.y - B.y;
    dz0 = A.z - B.z;
    //  Planar vector 1
    dx1 = C.x - A.x;
    dy1 = C.y - A.y;
    dz1 = C.z - A.z;

    //  Normal
    float Nx = dy0 * dz1 - dy1 * dz0;
    float Ny = dz0 * dx1 - dz1 * dx0;
    float Nz = dx0 * dy1 - dx1 * dy0;

    //  Draw triangle
    glNormal3f(Nx, Ny, Nz);
    glBegin(GL_TRIANGLES);
    glTexCoord2f(D.x, D.y);
    glVertex3f(A.x, A.y, A.z);
    glTexCoord2f(E.x, E.y);
    glVertex3f(B.x, B.y, B.z);
    glTexCoord2f(F.x, F.y);
    glVertex3f(C.x, C.y, C.z);
    glEnd();
}

void Mesh_drawArwing() {
    for (int i = 0; i < 22; i++) {
        Mesh_drawTriangle(ar_xyz[ar_idx[i].a], ar_xyz[ar_idx[i].b], ar_xyz[ar_idx[i].c], ar_uv[i * 3], ar_uv[i * 3 + 1], ar_uv[i * 3 + 2]);
    }
    for (int i = 0; i < 4; i++) {
        Mesh_drawTriangle(ar_xyz[ar_idx2[i].a], ar_xyz[ar_idx2[i].b], ar_xyz[ar_idx2[i].c], ar_uv2[i * 3], ar_uv2[i * 3 + 1], ar_uv2[i * 3 + 2]);
    }
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.1, 0.45, 0.15);
    for (int i = 0; i < 2; i++) {
        Mesh_drawTriangle(ar_xyz[ar_idx3[i].a], ar_xyz[ar_idx3[i].b], ar_xyz[ar_idx3[i].c], ar_uv3[0], ar_uv3[1], ar_uv3[2]);
    }
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
}

void Mesh_draw(Mesh* m) {
    glColor3f(m->color[0], m->color[1], m->color[2]);
	MeshType meshType = m->mesh_type;
	switch (meshType) {
		case MESHTYPE_TEAPOT:
			glutSolidTeapot(0.2);
			return;
        case MESHTYPE_ARWING:
            Mesh_drawArwing();
		case MESHTYPE_NONE:
		default:
			return;
	}
}

void Mesh_free(Mesh* m) {
    free(m->color);
    free(m);
}