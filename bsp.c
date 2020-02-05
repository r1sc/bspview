#include "bsp.h"
#include "vec.h"
#include "doom_types.h"

int getSideOnLine(node_t* node, vertex_t* point) {	
	return (point->x - node->x) * node->dy - (point->y - node->y) * node->dx > 0;
}
