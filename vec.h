#pragma once
#include "doom_types.h"

int getSideOnLine(short x, short y, short x1, short y1, short dx, short dy);
int vector_dot(const vertex_t* v1, const vertex_t* v2);
int isBoxInFrustum(const bbox_t* box, const vertex_t* point, const float angle);

extern const float pi;

#define DEG2RAD (pi / 180.0f)