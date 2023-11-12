#ifndef WINDOW_H
#define WINDOW_H

#include "vector2.h"

void Window_reshape(int width, int height);
double Window_getAspectRatio();
void Window_getWindowSize(Vector2* output);

#endif