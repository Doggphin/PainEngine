#ifndef PROJECT_INPUT
#define PROJECT_INPUT

#include "vector2.h"

typedef struct Input_inputMap {
	int esc; int a; int b; int c; int d; int e; int f; int g; int h; int i; int j; int k; int l; int m; int n; int o; int p; int q; int r; int s; int t; int u; int v; int w; int x; int y; int z; int space;
} Input_inputMap;

typedef enum Input_Keycode {
	KEYCODE_UNUSED, KEYCODE_ESC, KEYCODE_A, KEYCODE_B, KEYCODE_C, KEYCODE_D, KEYCODE_E, KEYCODE_F, KEYCODE_G, KEYCODE_H, KEYCODE_I, KEYCODE_J, KEYCODE_K, KEYCODE_L, KEYCODE_M, KEYCODE_N, KEYCODE_O, KEYCODE_P, KEYCODE_Q, KEYCODE_R, KEYCODE_S, KEYCODE_T, KEYCODE_U, KEYCODE_V, KEYCODE_W, KEYCODE_X, KEYCODE_Y, KEYCODE_Z, KEYCODE_SPACE
} Input_Keycode;

// include "key pressed" subscriber/listener thing here

int Input_getKey(Input_Keycode key);
int Input_getKeyBuffered(Input_Keycode key);
void Input_setBufferToCurrent();
void Input_setKeyDown(unsigned char ch, int x, int y);
void Input_setKeyUp(unsigned char ch, int x, int y);
void Input_setMousePosition(Vector2* newPos);
void Input_getMousePosition(Vector2* output);
void Input_addMouseDelta(Vector2* delta);
void Input_clearMouseDelta();
void Input_clearMousePosition();
void Input_getMouseDelta(Vector2* output);
void Input_passive(int x, int y);
void Input_setCursorLocked(int locked);
void Input_getJoystick(Input_Keycode up, Input_Keycode right, Input_Keycode down, Input_Keycode left, int buffered, Vector2* output);
void Input_getJoystickNormalized(Input_Keycode up, Input_Keycode right, Input_Keycode down, Input_Keycode left, int buffered, Vector2* output);

#endif