#pragma once
#include "doom_types.h"

int vector_dot(const vertex_t* v1, const vertex_t* v2);
int isPointInBoundingBox(const bbox_t* box, const vertex_t* point);