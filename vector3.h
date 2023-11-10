#ifndef VECTOR3_H
#define VECTOR3_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

/**
 * Maximum floating point difference that is considered as equal.
 */
#define VECTOR3_EPS (1e-4)

typedef enum {
	UP, FORWARD, RIGHT, BACKWARD, LEFT, DOWN, ONE, ZERO
} VectorPreset;

typedef struct Vector3 {
	float x;
	float y;
	float z;
} Vector3;

void Vector3_set(float x, float y, float z, Vector3* output);
Vector3* Vector3_create(float x, float y, float z);
Vector3* Vector3_createPreset(VectorPreset dir);
void Vector3_copy(Vector3* from, Vector3* to);
void Vector3_preset(VectorPreset dir, Vector3* output);
void Vector3_multiply(Vector3* v, float f, Vector3* output);
void Vector3_div(Vector3* v, float f, Vector3* output);
void Vector3_sub(Vector3* a, Vector3* b, Vector3* output);
void Vector3_add(Vector3* a, Vector3* b, Vector3* output);
void Vector3_addf(Vector3* v, float a, float b, float c);
void Vector3_subf(Vector3* v, float a, float b, float c);
bool Vector3_equal(Vector3* v1, Vector3* v2);
float Vector3_sqrmagnitude(Vector3* v);
float Vector3_magnitude(Vector3* v);
void Vector3_normalize(Vector3* v, Vector3* output);
void Vector3_abs(Vector3* v, Vector3* output);
float Vector3_dot(Vector3* v1, Vector3* v2);
float Vector3_anglerad(Vector3* v, Vector3* v2);
float Vector3_angledeg(Vector3* v1, Vector3* v2);
void Vector3_project(Vector3* onto, Vector3* from, Vector3* output);
float Vector3_distance(Vector3* v1, Vector3* v2);
void Vector3_lerp(Vector3* from, Vector3* to, float t, Vector3* output);
void Vector3_lerpclamped(Vector3* from, Vector3* to, float t, Vector3* output);
void Vector3_print(Vector3* v);
void Vector3_cross(Vector3* a, Vector3* b, Vector3* output);
void Vector3_orthogonal(Vector3* v, Vector3* output);

#endif