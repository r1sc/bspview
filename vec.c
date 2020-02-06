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

typedef struct {
	short x;
	short y;
	float dx;
	float dy;
} linesegment_t;

int getSideOnLineFloat(short x, short y, short x1, short y1, float dx, float dy) {
	return (x - x1) * dy - (y - y1) * dx < 0;
}

int pointInFrustum(short x, short y, const linesegment_t* planes) {
	for (size_t i = 0; i < 2; i++)
	{
		if (!getSideOnLineFloat(x, y, planes[i].x, planes[i].y, planes[i].dx, planes[i].dy)) {
			return FALSE;
		}
	}
	return TRUE;
}

int boxInPlanes(const bbox_t* box, const linesegment_t* planes) {
	if (pointInFrustum(box->left, box->top, planes))
		return TRUE;

	if (pointInFrustum(box->right, box->top, planes))
		return TRUE;

	if (pointInFrustum(box->left, box->bottom, planes))
		return TRUE;

	if (pointInFrustum(box->right, box->bottom, planes))
		return TRUE;
	return FALSE;
}

int isBoxInFrustum(const bbox_t* box, const vertex_t* point, const float angle) {
	if (point->x >= box->left && point->x < box->right && point->y <= box->top && point->y > box->bottom)
		return TRUE;
	
	float dx2 = cos((angle - 22.5f) * DEG2RAD);
	float dy2 = -sin((angle - 22.5f) * DEG2RAD);
	float dx3 = cos((angle + 22.5f) * DEG2RAD);
	float dy3 = -sin((angle + 22.5f) * DEG2RAD);


	linesegment_t planes[2] = {
		{point->x, point->y, -dx2, -dy2},
		{point->x, point->y, dx3, dy3}
	};

	return boxInPlanes(box, planes);
}