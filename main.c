#include <stdlib.h>
#include <stdio.h>
#include "doom_types.h"
#include "wad.h"
#include "map.h"
#include "window.h"

void main() {
	wad_file_t wadFile;
	wad_open(&wadFile, "..\\DOOM1.WAD");
	for (int i = 0; i < wadFile.numFiles; i++)
	{
		printf("%.8s\n", wadFile.lumps[i].name);
	}

	map_t map;
	map_load(&map, &wadFile, "E1M1");
	
	window_open(640, 480);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	GLdouble padding = 100;

	node_t root = map.nodes[map.numNodes -1];

	short top, bottom, left, right; 

	if (root.leftBBox.top > root.rightBBox.top)
		top = root.leftBBox.top;
	else
		top = root.rightBBox.top;

	if (root.leftBBox.bottom > root.rightBBox.bottom)
		bottom = root.rightBBox.bottom;
	else
		bottom = root.leftBBox.bottom;

	if (root.leftBBox.right> root.rightBBox.right)
		right = root.leftBBox.right;
	else
		right = root.rightBBox.right;

	if (root.leftBBox.left > root.rightBBox.left)
		left = root.rightBBox.left;
	else
		left = root.leftBBox.left;



	glOrtho(left - padding, right + padding, bottom - padding, top + padding, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	while (window_update())
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		
		glBegin(GL_LINES);
		for (int i = 0; i < map.numLinedefs; i++)
		{
			vertex_t v1 = map.vertexes[map.linedefs[i].v1];
			vertex_t v2 = map.vertexes[map.linedefs[i].v2];
			glVertex2s(v1.x, v1.y);
			glVertex2s(v2.x, v2.y);
		}
		glEnd();

		window_swap();
	}

	map_free(&map);
	wad_close(&wadFile);
}