#include "vector2.h"
#include "macros.h"
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>

void Vector2_set(float x, float y, Vector2* output) {
	assert(output != NULL);
	output->x = x;
	output->y = y;
}

void Vector2_preset(Vector2Preset dir, Vector2* output) {
	assert(output != NULL);

	switch (dir) {
    case VECTOR2_ZERO:
		Vector2_set(0, 0, output);
		return;
	case VECTOR2_UP:
		Vector2_set(0, 1, output);
		return;
	case VECTOR2_RIGHT:
		Vector2_set(1, 0, output);
		return;
	case VECTOR2_DOWN:
		Vector2_set(0, -1, output);
		return;
	case VECTOR2_LEFT:
		Vector2_set(-1, 0, output);
		return;
	}
}

Vector2* Vector2_createPreset(Vector2Preset dir) {
    QMALLOC(ret, Vector2)
    Vector2_preset(dir, ret);
    return ret;
}

void Vector2_add(Vector2* a, Vector2* b, Vector2* output) {
    assert(output != NULL);
    Vector2_set(a->x + b->x, a->y + b->y, output);
}

void Vector2_addDirect(Vector2* from, Vector2* to) {
    Vector2_set(from->x + to->x, from->y + to->y, to);
}
 
Vector2* Vector2_create(float x, float y) {
	Vector2* ret = malloc(sizeof(Vector2));
	Vector2_set(x, y, ret);
	return ret;
}

void Vector2_multiply(Vector2* v, float f, Vector2* output) {
    assert(output != NULL);
    Vector2_set(v->x * f, v->y * f, output);
}

void Vector2_multiplyDirect(float f, Vector2* output) {
    Vector2_multiply(output, f, output);
}

void Vector2_copy(Vector2* v, Vector2* output) {
	assert(output != NULL);
	Vector2_set(v->x, v->y, output);
}

void Vector2_print(Vector2* v) {
	printf("(%.6f, %.6f) \n", v->x, v->y);
}