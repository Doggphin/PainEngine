#include "vector3.h"
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <stdio.h>

void Vector3_set(float x, float y, float z, Vector3* output) {
	assert(output != NULL);
	output->x = x;
	output->y = y;
	output->z = z;
}

void Vector3_preset(Vector3Preset dir, Vector3* output) {
	assert(output != NULL);
	switch (dir) {
	case VECTOR3_UP:
		Vector3_set(0, 1, 0, output);
		return;
	case VECTOR3_FORWARD:
		Vector3_set(0, 0, -1, output);
		return;
	case VECTOR3_RIGHT:
		Vector3_set(1, 0, 0, output);
		return;
	case VECTOR3_BACKWARD:
		Vector3_set(0, 0, 1, output);
		return;
	case VECTOR3_LEFT:
		Vector3_set(1, 0, 0, output);
		return;
	case VECTOR3_DOWN:
		Vector3_set(0, -1, 0, output);
		return;
	case VECTOR3_ONE:
		Vector3_set(1, 1, 1, output);
		return;
	case VECTOR3_ZERO:
		Vector3_set(0, 0, 0, output);
		return;
	default:
		return;
	}
}

Vector3* Vector3_create(float x, float y, float z) {
	Vector3* ret = malloc(sizeof(Vector3));
	Vector3_set(x, y, z, ret);
	return ret;
}

Vector3* Vector3_createPreset(Vector3Preset dir) {
	Vector3* ret = malloc(sizeof(Vector3));
	Vector3_preset(dir, ret);
	return ret;
}

float Vector3_rad2deg(float rad) {
	return 57.2957795131 * rad;
}

void Vector3_multiply(Vector3* v, float f, Vector3* output) {
	assert(output != NULL);
	Vector3_set(v->x * f, v->y * f, v->z * f, output);
}

void Vector3_div(Vector3* v, float f, Vector3* output) {
	assert(output != NULL);
	Vector3_set(v->x / f, v->y / f, v->z / f, output);
}

void Vector3_sub(Vector3* a, Vector3* b, Vector3* output) {
	assert(output != NULL);
	Vector3_set(a->x - b->x, a->y - b->y, a->z - b->z, output);
}

void Vector3_subf(Vector3* v, float x, float y, float z) {
	assert(v != NULL);
	Vector3_set(v->x - x, v->y - y, v->z - z, v);
}

void Vector3_add(Vector3* a, Vector3* b, Vector3* output) {
	assert(output != NULL);
	Vector3_set(a->x + b->x, a->y + b->y, a->z + b->z, output);
}

void Vector3_addf(Vector3* v, float x, float y, float z) {
	assert(v != NULL);
	Vector3_set(v->x + x, v->y + y, v->z + z, v);
}

void Vector3_copy(Vector3* v, Vector3* output) {
	assert(output != NULL);
	Vector3_set(v->x, v->y, v->z, output);
}

int Vector3_equal(Vector3* v1, Vector3* v2) {
	return fabs(v1->x - v2->x) <= VECTOR3_EPS &&
		fabs(v1->y - v2->y) <= VECTOR3_EPS &&
		fabs(v1->z - v2->z) <= VECTOR3_EPS;
}

float Vector3_sqrmagnitude(Vector3* v) {
	return v->x * v->x + v->y * v->y + v->z * v->z;
}

float Vector3_magnitude(Vector3* v) {
	return sqrt(Vector3_sqrmagnitude(v));
}

void Vector3_normalize(Vector3* v, Vector3* output) {
	assert(output != NULL);
	Vector3_div(v, Vector3_magnitude(v), output);
}

void Vector3_normalizeDirect(Vector3* v) {
	Vector3_normalize(v, v);
}

void Vector3_abs(Vector3* v, Vector3* output) {
	assert(output != NULL);
	Vector3_set(abs(v->x), abs(v->y), abs(v->z), output);
}

float Vector3_dot(Vector3* v1, Vector3* v2) {
	return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
}

float Vector3_anglerad(Vector3* v1, Vector3* v2) {
	return acos(Vector3_dot(v1, v2) /
		(sqrt(Vector3_sqrmagnitude(v1) * Vector3_sqrmagnitude(v2))));
}

float Vector3_angledeg(Vector3* v1, Vector3* v2) {
	return Vector3_rad2deg(Vector3_anglerad(v1, v2));
}

void Vector3_project(Vector3* onto, Vector3* from, Vector3* output) {
	assert(output != NULL);
	Vector3_multiply(onto, (Vector3_dot(from, onto) / pow(Vector3_magnitude(onto), 2)), output);
}

float Vector3_sqrdistance(Vector3* point1, Vector3* point2) {
	return pow(point1->x - point2->x, 2) + pow(point1->y - point2->y, 2) + pow(point1->x - point2->x, 2);
}
float Vector3_distance(Vector3* point1, Vector3* point2) {
	return sqrt(Vector3_sqrdistance(point1, point2));
}
float Vector3_sqrlength(Vector3* v) {
	return v->x * v->x + v->y * v->y + v->z * v->z;
}
float Vector3_length(Vector3* v) {
	return sqrt(Vector3_sqrlength(v));
}

void Vector3_lerp(Vector3* from, Vector3* to, float t, Vector3* output) {
	Vector3_set((to->x - from->x) * t + from->x,
		(to->y - from->y) * t + from->y,
		(to->z - from->z) * t + from->z,
		output);
}

void Vector3_cross(Vector3* a, Vector3* b, Vector3* output) {
	// From https://www.mathworks.com/help/matlab/ref/cross.html
	Vector3_set(a->y * b->z - a->z * b->y,
		a->z * b->x - a->x * b->z,
		a->x * b->y - a->y * b->x,
		output);
}

void Vector3_lerpclamped(Vector3* from, Vector3* to, float t, Vector3* output) {
	Vector3_lerp(from, to, fmax(fmin(t, 1), 0), output);
}

void Vector3_orthogonal(Vector3* v, Vector3* output) {
	//https://stackoverflow.com/questions/1171849/finding-quaternion-representing-the-rotation-from-one-vector-to-another#:~:text=cross(u%2C%20half))%3B%0A%7D-,The,-orthogonal%20function%20returns
	float x = abs(v->x);
	float y = abs(v->y);
	float z = abs(v->z);

	Vector3 other;
	Vector3_preset(x < y ? (x < z ? VECTOR3_RIGHT : VECTOR3_UP) : (y < z ? VECTOR3_BACKWARD : VECTOR3_UP), &other);
	Vector3_cross(v, &other, output);
}

void Vector3_multiplyEach(Vector3* v1, Vector3* v2, Vector3* output) {
	Vector3_set(v1->x * v2->x, v1->y * v2->y, v1->z * v2->z, output);
}
Vector3* Vector3_multiplyEachOut(Vector3* v1, Vector3* v2) {
	Vector3* ret = malloc(sizeof(Vector3));
	Vector3_multiplyEach(v1, v2, ret);
	return ret;
}

void Vector3_print(Vector3* v) {
	printf("(%.6f, %.6f, %.6f) \n", v->x, v->y, v->z);
}