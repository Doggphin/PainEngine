#include "input.h"
#include "CSCIx229.h"

static struct Input_keysPressed current_inputs;
static struct Input_keysPressed buffered_inputs;
static const struct Input_keysPressed empty_inputs;

#define MACRO_KEYCASE(A, B) {\
	case A:\
		B = value;\
}

void Input_setKeyValue(Input_Keycode keycode, int value, int buffered) {
	Input_keysPressed* selected = buffered ? &buffered_inputs : &current_inputs;

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
		default:
			printf("Unimplemented setInput return for given keycode\n");
			break;
	}
}

int Input_isKeyDown(Input_Keycode key, int buffered) {
	Input_keysPressed* selected = buffered ? &buffered_inputs : &current_inputs;

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
		default:
			printf("Unimplemented getInput return for given keycode\n");
			return 0;
	}
}

Input_Keycode Input_charToKeycode(unsigned char ch) {
	switch (ch) {
		case 'a':
		case 'A':
			return KEYCODE_A;
		case 'b':
		case 'B':
			return KEYCODE_B;
		case 27:
			return KEYCODE_ESC;
		default:
			return KEYCODE_UNUSED;
	}
}

void Input_clearBuffer() {
	buffered_inputs = empty_inputs;
}

void Input_setKeyDown(unsigned char ch, int x, int y) {
	Input_Keycode keycode = Input_charToKeycode(ch);
	Input_setKeyValue(keycode, 1, 0);
	Input_setKeyValue(keycode, 1, 1);
}

void Input_setKeyUp(unsigned char ch, int x, int y)
{
	Input_setKeyValue(Input_charToKeycode(ch), 0, 0);
}