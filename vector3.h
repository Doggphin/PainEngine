#ifndef VECTOR3_H
#define VECTOR3_H

#include <stdint.h>
#include <stdio.h>
#include <math.h>

/**
 * Maximum floating point difference that is considered as equal.
 */
#define VECTOR3_EPS (1e-4)

typedef enum {
	VECTOR3_UP, VECTOR3_FORWARD, VECTOR3_RIGHT, VECTOR3_BACKWARD, VECTOR3_LEFT, VECTOR3_DOWN, VECTOR3_ONE, VECTOR3_ZERO
} Vector3Preset;

typedef struct Vector3 {
	float x;
	float y;
	float z;
} Vector3;

void Vector3_set(float x, float y, float z, Vector3* output);
Vector3* Vector3_create(float x, float y, float z);
Vector3* Vector3_createPreset(Vector3Preset dir);
void Vector3_copy(Vector3* from, Vector3* to);
void Vector3_preset(Vector3Preset dir, Vector3* output);
void Vector3_multiply(Vector3* v, float f, Vector3* output);
void Vector3_multiplyEach(Vector3* v1, Vector3* v2, Vector3* output);
Vector3* Vector3_multiplyEachOut(Vector3* v1, Vector3* v2);
void Vector3_div(Vector3* v, float f, Vector3* output);
void Vector3_sub(Vector3* a, Vector3* b, Vector3* output);
void Vector3_add(Vector3* a, Vector3* b, Vector3* output);
void Vector3_addf(Vector3* v, float a, float b, float c);
void Vector3_subf(Vector3* v, float a, float b, float c);
int Vector3_equal(Vector3* v1, Vector3* v2);
float Vector3_sqrmagnitude(Vector3* v);
float Vector3_magnitude(Vector3* v);
void Vector3_normalize(Vector3* v, Vector3* output);
void Vector3_abs(Vector3* v, Vector3* output);
float Vector3_dot(Vector3* v1, Vector3* v2);
float Vector3_anglerad(Vector3* v, Vector3* v2);
float Vector3_angledeg(Vector3* v1, Vector3* v2);
void Vector3_project(Vector3* onto, Vector3* from, Vector3* output);
float Vector3_sqrdistance(Vector3* point1, Vector3* point2);
float Vector3_distance(Vector3* point1, Vector3* point2);
void Vector3_lerp(Vector3* from, Vector3* to, float t, Vector3* output);
void Vector3_lerpclamped(Vector3* from, Vector3* to, float t, Vector3* output);
void Vector3_print(Vector3* v);
void Vector3_cross(Vector3* a, Vector3* b, Vector3* output);
void Vector3_orthogonal(Vector3* v, Vector3* output);
float Vector3_sqrlength(Vector3* v);
float Vector3_length(Vector3* v);
void Vector3_normalizeDirect(Vector3* v);

#endif