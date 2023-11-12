#include "input.h"
#include "CSCIx229.h"
#include "window.h"
#include <assert.h>

static struct Input_inputMap current_inputs;
static struct Input_inputMap buffered_inputs;
static const struct Input_inputMap empty_inputs;

static struct Vector2 mousePosition;
static struct Vector2 mouseDeltaBuffer;

int lockCursor = 1;

#define MACRO_KEYCASE(A, B) {\
	case A:\
		B = value;\
}

void Input_setKeyValue(Input_Keycode keycode, int value, int buffered) {
	Input_inputMap* selected = buffered ? &buffered_inputs : &current_inputs;

	if (keycode == KEYCODE_UNUSED) {
		return;
	}
	switch (keycode) {
		case KEYCODE_ESC:
			if (value) { exit(0); }		// temp
			selected->esc = value;
			break;
		MACRO_KEYCASE(KEYCODE_A, selected->a)
		case KEYCODE_B:
			selected->b = value;
			break;
		case KEYCODE_C:
			selected->c = value;
			break;
		case KEYCODE_D:
			selected->d = value;
			break;
		case KEYCODE_E:
			selected->e = value;
			break;
		case KEYCODE_F:
			selected->f = value;
			break;
		case KEYCODE_G:
			selected->g = value;
			break;
		case KEYCODE_H:
			selected->h = value;
			break;
		case KEYCODE_I:
			selected->i = value;
			break;
		case KEYCODE_J:
			selected->j = value;
			break;
		case KEYCODE_K:
			selected->k = value;
			break;
		case KEYCODE_L:
			selected->l = value;
			break;
		case KEYCODE_M:
			selected->m = value;
			break;
		case KEYCODE_N:
			selected->n = value;
			break;
		case KEYCODE_O:
			selected->o = value;
			break;
		case KEYCODE_P:
			selected->p = value;
			break;
		case KEYCODE_Q:
			selected->q = value;
			break;
		case KEYCODE_R:
			selected->r = value;
			break;
		case KEYCODE_S:
			selected->s = value;
			break;
		case KEYCODE_T:
			selected->t = value;
			break;
		case KEYCODE_U:
			selected->u = value;
			break;
		case KEYCODE_V:
			selected->v = value;
			break;
		case KEYCODE_W:
			selected->w = value;
			break;
		case KEYCODE_X:
			selected->x = value;
			break;
		case KEYCODE_Y:
			selected->y = value;
			break;
		case KEYCODE_Z:
			selected->z = value;
			break;
		MACRO_KEYCASE(KEYCODE_SPACE, selected->space)
		default:
			printf("Unimplemented setInput return for given keycode\n");
			break;
	}
}

int Input_isKeyDownSwitch(Input_Keycode key, int buffered) {
	Input_inputMap* selected = buffered ? &buffered_inputs : &current_inputs;

	switch (key) {
		case KEYCODE_ESC:
			return selected->esc;
		case KEYCODE_A:
			return selected->a;
		case KEYCODE_B:
			return selected->b;
		case KEYCODE_C:
			return selected->c;
		case KEYCODE_D:
			return selected->d;
		case KEYCODE_E:
			return selected->e;
		case KEYCODE_F:
			return selected->f;
		case KEYCODE_G:
			return selected->g;
		case KEYCODE_H:
			return selected->h;
		case KEYCODE_I:
			return selected->i;
		case KEYCODE_J:
			return selected->j;
		case KEYCODE_K:
			return selected->k;
		case KEYCODE_L:
			return selected->l;
		case KEYCODE_M:
			return selected->m;
		case KEYCODE_N:
			return selected->n;
		case KEYCODE_O:
			return selected->o;
		case KEYCODE_P:
			return selected->p;
		case KEYCODE_Q:
			return selected->q;
		case KEYCODE_R:
			return selected->r;
		case KEYCODE_S:
			return selected->s;
		case KEYCODE_T:
			return selected->t;
		case KEYCODE_U:
			return selected->u;
		case KEYCODE_V:
			return selected->v;
		case KEYCODE_W:
			return selected->w;
		case KEYCODE_X:
			return selected->x;
		case KEYCODE_Y:
			return selected->y;
		case KEYCODE_Z:
			return selected->z;
		case KEYCODE_SPACE:
			return selected->space;
		default:
			printf("Unimplemented getInput return for given keycode\n");
			return 0;
	}
}


int Input_getKeyBuffered(Input_Keycode key) {
	return Input_isKeyDownSwitch(key, 1);
}
int Input_getKey(Input_Keycode key) {
	return Input_isKeyDownSwitch(key, 0);
}

#define CHARTOKEYCODECASE(UPPER, LOWER, KEYCODE) {\
	case UPPER:\
	case LOWER:\
		return KEYCODE;\
}
#define CHARTOKEYCODECASESIMP(KEY, KEYCODE) {\
	case KEY:\
		return KEYCODE;\
}

Input_Keycode Input_charToKeycode(unsigned char ch) {
	switch (ch) {
		CHARTOKEYCODECASE('A', 'a', KEYCODE_A);
		CHARTOKEYCODECASE('B', 'b', KEYCODE_B);
		CHARTOKEYCODECASE('C', 'c', KEYCODE_C);
		CHARTOKEYCODECASE('D', 'd', KEYCODE_D);
		CHARTOKEYCODECASE('E', 'e', KEYCODE_E);
		CHARTOKEYCODECASE('F', 'f', KEYCODE_F);
		CHARTOKEYCODECASE('G', 'g', KEYCODE_G);
		CHARTOKEYCODECASE('H', 'h', KEYCODE_H);
		CHARTOKEYCODECASE('I', 'i', KEYCODE_I);
		CHARTOKEYCODECASE('J', 'j', KEYCODE_J);
		CHARTOKEYCODECASE('K', 'k', KEYCODE_K);
		CHARTOKEYCODECASE('L', 'l', KEYCODE_L);
		CHARTOKEYCODECASE('M', 'm', KEYCODE_M);
		CHARTOKEYCODECASE('N', 'n', KEYCODE_N);
		CHARTOKEYCODECASE('O', 'o', KEYCODE_O);
		CHARTOKEYCODECASE('P', 'p', KEYCODE_P);
		CHARTOKEYCODECASE('Q', 'q', KEYCODE_Q);
		CHARTOKEYCODECASE('R', 'r', KEYCODE_R);
		CHARTOKEYCODECASE('S', 's', KEYCODE_S);
		CHARTOKEYCODECASE('T', 't', KEYCODE_T);
		CHARTOKEYCODECASE('U', 'u', KEYCODE_U);
		CHARTOKEYCODECASE('V', 'v', KEYCODE_V);
		CHARTOKEYCODECASE('W', 'w', KEYCODE_W);
		CHARTOKEYCODECASE('X', 'x', KEYCODE_X);
		CHARTOKEYCODECASE('Y', 'y', KEYCODE_Y);
		CHARTOKEYCODECASE('Z', 'z', KEYCODE_Z);
		CHARTOKEYCODECASESIMP(39, KEYCODE_SPACE)
		case 27:
			return KEYCODE_ESC;
		default:
			return KEYCODE_UNUSED;
	}
}

void Input_setBufferToCurrent() {
	buffered_inputs = current_inputs;
}

void Input_setKeyDown(unsigned char ch, int x, int y) {
	Input_Keycode keycode = Input_charToKeycode(ch);
	Input_setKeyValue(keycode, 1, 0);
	Input_setKeyValue(keycode, 1, 1);
}

void Input_setKeyUp(unsigned char ch, int x, int y) {
	Input_setKeyValue(Input_charToKeycode(ch), 0, 0);
}


void Input_setMousePosition(Vector2* newPos) {
	Vector2_copy(newPos, &mousePosition);
}

void Input_clearMousePosition() {
	Vector2_preset(VECTOR2_ZERO, &mousePosition);
}

void Input_getMousePosition(Vector2* output) {
	Vector2_copy(&mousePosition, output);
}

void Input_getMouseDelta(Vector2* output) {
	Vector2_copy(&mouseDeltaBuffer, output);
}

void Input_addMouseDelta(Vector2* delta) {
	Vector2_addDirect(delta, &mouseDeltaBuffer);
}

void Input_clearMouseDelta() {
	Vector2_preset(VECTOR2_ZERO, &mouseDeltaBuffer);
}

void Input_passive(int x, int y) {
	struct Vector2 windowSize;
	Window_getWindowSize(&windowSize);

	int relativeMouseX = x - windowSize.x / 2;
	int relativeMouseY = y - windowSize.y / 2;
	Vector2_set(x, y, &mousePosition);
	if (lockCursor) {
		Vector2_addDirect(&(Vector2) { relativeMouseX, relativeMouseY }, &mouseDeltaBuffer);
		glutWarpPointer(windowSize.x / 2, windowSize.y / 2);
		Input_clearMousePosition();
	}
	else {
		Vector2_set(relativeMouseX, relativeMouseY, &mousePosition);
	} 
}

void Input_setCursorLocked(int locked) {
	lockCursor = locked;
	glutSetCursor(locked ? GLUT_CURSOR_NONE : GLUT_CURSOR_LEFT_ARROW);
}

void Input_getJoystick(Input_Keycode up, Input_Keycode right, Input_Keycode down, Input_Keycode left, int buffered, Vector2* output) {
	Vector2_set(Input_isKeyDownSwitch(right, buffered) - Input_isKeyDownSwitch(left, buffered), Input_isKeyDownSwitch(up, buffered) - Input_isKeyDownSwitch(down, buffered), output);
	assert(output != NULL);
}

void Input_getJoystickNormalized(Input_Keycode up, Input_Keycode right, Input_Keycode down, Input_Keycode left, int buffered, Vector2* output) {
	assert(output != NULL);
	Input_getJoystick(up, right, down, left, buffered, output);
	if(fabsf(output->x) == 1 && fabsf(output->y) == 1) {
		Vector2_multiplyDirect(0.7071, output);
	}
}