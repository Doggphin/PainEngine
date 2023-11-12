#ifndef VECTOR2_H
#define VECTOR2_H

#define VECTOR2_EPS (1e-4)

typedef enum {
	VECTOR2_ZERO, VECTOR2_UP, VECTOR2_RIGHT, VECTOR2_DOWN, VECTOR2_LEFT
} Vector2Preset;

typedef struct Vector2 {
    float x;
    float y;
} Vector2;

void Vector2_set(float x, float y, Vector2* output);
Vector2* Vector2_create(float x, float y);
Vector2* Vector2_createPreset(Vector2Preset dir);
void Vector2_preset(Vector2Preset, Vector2* output);
void Vector2_copy(Vector2* v, Vector2* output);
void Vector2_add(Vector2* a, Vector2* b, Vector2* output);
void Vector2_addDirect(Vector2* from, Vector2* to);
void Vector2_print(Vector2* v);
void Vector2_multiply(Vector2* v, float f, Vector2* output);
void Vector2_multiplyDirect(float f, Vector2* output);

#endif