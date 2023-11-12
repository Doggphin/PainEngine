#include "CSCIx229.h"
#include "vector2.h"

static struct Vector2 windowSize;
double asp;

void Window_reshape(int width, int height) {
    Vector2_set(width, height, &windowSize);
    glViewport(0, 0, RES * width, RES * height);
    glLoadIdentity();
    asp = (height > 0) ? (float)width / height : 1;
}

double Window_getAspectRatio() {
    return asp;
}

void Window_getWindowSize(Vector2* output) {
    Vector2_copy(&windowSize, output);
}