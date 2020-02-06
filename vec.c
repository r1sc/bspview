#include "vec.h"
#include <math.h>

#define TRUE 1
#define FALSE 0

int getSideOnLine(short x, short y, short x1, short y1, short dx, short dy) {
	return (x - x1) * dy - (y - y1) * dx < 0;
}

int vector_dot(const vertex_t* v1, const vertex_t* v2) {
	 return v1->x * v2->x + v1->y * v2->y;
}

float vector_length(const vertex_t* v1) {
	return (float)sqrt(v1->x * v1->x + v1->y * v1->y);
}

void vector_normalize(vertex_t* v1) {
	float length = vector_length(v1);
	v1->x /= length;
	v1->y /= length;
}

int boxLeftOfLine(const bbox_t* box, short x1, short y1, short x2, short y2) {
	short dx = x2 - x1;
	short dy = y2 - y1;
	int tl = getSideOnLine(box->left, box->top, x1, y1, dx, dy);
	int tr = getSideOnLine(box->right, box->top, x1, y1, dx, dy);
	int bl = getSideOnLine(box->left, box->bottom, x1, y1, dx, dy);
	int br = getSideOnLine(box->right, box->bottom, x1, y1, dx, dy);
	return tl || tr || bl || br;
}

int isBoxInFrustum(const bbox_t* box, const vertex_t* point, const float angle) {
	float x2 = point->x + cos((angle - 22.5f) * DEG2RAD);
	float y2 = point->y - sin((angle - 22.5f) * DEG2RAD);
	float x3 = point->x + cos((angle + 22.5f) * DEG2RAD);
	float y3 = point->y - sin((angle + 22.5f) * DEG2RAD);

	return boxLeftOfLine(box, point->x, point->y, x2, y2) && boxLeftOfLine(box, x3, y3, point->x, point->y);
}