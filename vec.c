#include "vec.h"
#include <math.h>

#define TRUE 1
#define FALSE 0

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

int isPointInBoundingBox(const bbox_t* box, const vertex_t* point) {
	if (point->y > box->bottom && point->y <= box->top && point->x >= box->left && point->x < box->right)
		return TRUE;
	return FALSE;
}