#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "doom_types.h"
#include "wad.h"
#include "map.h"
#include "window.h"
#include "bsp.h"
#include "vec.h"

map_t map;

void drawQuad(short x1, short z1, short x2, short z2, short ybottom, short ytop) {
	glVertex3s(x1, ytop, z1);
	glVertex3s(x2, ytop, z2);
	glVertex3s(x2, ybottom, z2);
	glVertex3s(x1, ybottom, z1);
}

GLubyte sectorColorMap[] = {
	255, 255, 255,
	255, 0, 0,
	0, 255, 0,
	0, 0, 255,
	255, 128, 0,
	255, 0, 128,
	128, 255, 0,
	0, 255, 128,
	0, 128, 255
};

#define NODE_SSECTOR 0x7FFF

void renderSubsector(short ssectorIdx, int side) {
	ssector_t ssector = map.subSectors[ssectorIdx];
	seg_t seg;
	for (int i = ssector.firstSegIndex; i < ssector.firstSegIndex + ssector.segCount; i++)
	{
		seg = map.segs[i];
		linedef_t* lineDef = &map.linedefs[seg.linedefIndex];
		lineDef->tag = side;
		/*if (lineDef->sidenum[side] == -1)
			continue;*/
		vertex_t v1 = map.vertexes[seg.startVertexIndex];
		vertex_t v2 = map.vertexes[seg.endVertexIndex];

		short floorHeightMin = SHRT_MAX, floorHeightMax = 0, ceilingHeightMin = SHRT_MAX, ceilingHeightMax = 0;

		if (lineDef->sidenum[side] != -1) {
			sidedef_t leftSide = map.sidedefs[lineDef->sidenum[side]];
			sector_t leftSector = map.sectors[leftSide.sectornum];
			floorHeightMin = leftSector.floorheight;
			floorHeightMax = leftSector.floorheight;
			ceilingHeightMin = leftSector.ceilingheight;
			ceilingHeightMax = leftSector.ceilingheight;
			
			glColor3ub(sectorColorMap[(seg.linedefIndex % 9) * 3], sectorColorMap[(seg.linedefIndex % 9) * 3 + 1], sectorColorMap[(seg.linedefIndex % 9) * 3 + 2]);

			drawQuad(v1.x, v1.y, v2.x, v2.y, floorHeightMax, ceilingHeightMin);
			if (floorHeightMin != floorHeightMax) {
				drawQuad(v1.x, v1.y, v2.x, v2.y, floorHeightMin, floorHeightMax);
			}
			if (ceilingHeightMin != ceilingHeightMax) {
				drawQuad(v1.x, v1.y, v2.x, v2.y, ceilingHeightMin, ceilingHeightMax);
			}
		}
		/*glVertex3s(v1.x, sector.floorheight, v1.y);
		glVertex3s(v1.x, sector.ceilingheight, v1.y);*/
	}
}

void traverseTree(short nodeIdx, vertex_t* eye, int side) {
	if (nodeIdx >> 15) { // Subsector
		if (nodeIdx == -1)
			renderSubsector(0, side);
		else
			renderSubsector(nodeIdx & NODE_SSECTOR, side);
	}
	else {
		node_t* node = &map.nodes[nodeIdx];
		int side = getSideOnLine(node, eye);

		traverseTree(node->children[side], eye, side);

		if (isPointInBoundingBox(&node->bbox[1 - side], eye, 1 - side)) {
			traverseTree(node->children[1 - side], eye, 1 - side);
		}
	}
}

const GLdouble pi = 3.1415926535897932384626433832795;
void perspectiveGL(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
	GLdouble fW, fH;
	fH = tan(fovY / 360 * pi) * zNear;
	fW = fH * aspect;
	glFrustum(fW, -fW, -fH, fH, zNear, zFar);
}

void main() {
	wad_file_t wadFile;
	wad_open(&wadFile, "..\\DOOM1.WAD");

	map_load(&map, &wadFile, "E1M1");

	window_open(640, 480);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	GLdouble padding = 100;

	vertex_t eye;
	eye.x = 1000;
	eye.y = 500;
	node_t root = map.nodes[map.numNodes - 1];

	short top, bottom, left, right;

	if (root.bbox[0].top > root.bbox[1].top)
		top = root.bbox[0].top;
	else
		top = root.bbox[1].top;

	if (root.bbox[0].bottom > root.bbox[1].bottom)
		bottom = root.bbox[1].bottom;
	else
		bottom = root.bbox[0].bottom;

	if (root.bbox[0].right > root.bbox[1].right)
		right = root.bbox[0].right;
	else
		right = root.bbox[1].right;

	if (root.bbox[0].left > root.bbox[1].left)
		left = root.bbox[1].left;
	else
		left = root.bbox[0].left;

	glEnable(GL_DEPTH_TEST);
	float rotation = 90;
	while (running)
	{
		window_update();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		perspectiveGL(45, 640.0 / 480.0, 0.1f, 5000);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glRotatef(rotation - 90, 0, 1, 0);
		glTranslatef(-eye.x, -50, -eye.y);
		glBegin(GL_QUADS);
		traverseTree(map.numNodes - 1, &eye, 0);
		glEnd();


		glDisable(GL_DEPTH_TEST);
		glColor3f(1, 1, 1);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(left - padding, right + padding, bottom - padding, top + padding, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glBegin(GL_LINES);
		for (int i = 0; i < map.numLinedefs; i++)
		{
			if (map.linedefs[i].tag == 1)
				glColor3ub(255, 0, 0);
			else
			{
				glColor3ub(255, 255, 255);
			}
			vertex_t v1 = map.vertexes[map.linedefs[i].v1];
			vertex_t v2 = map.vertexes[map.linedefs[i].v2];
			glVertex2s(v1.x, v1.y);
			glVertex2s(v2.x, v2.y);
		}
		glEnd();
		glColor3f(0, 1, 1);
		glBegin(GL_LINES);
		glVertex2s(eye.x, eye.y);
		glVertex2s(eye.x - cos(rotation * (pi / 180.0)) * 100, eye.y - sin(rotation * (pi / 180.0)) * 100);
		glEnd();

		if (KEYSDOWN[VK_LEFT])
			rotation += 3;
		if (KEYSDOWN[VK_RIGHT])
			rotation -= 3;
		if (KEYSDOWN[VK_UP])
		{
			eye.x -= cos(rotation * (pi / 180.0)) * 20;
			eye.y -= sin(rotation * (pi / 180.0)) * 20;
		}
		if (KEYSDOWN[VK_DOWN])
		{
			eye.x += cos(rotation * (pi / 180.0)) * 20;
			eye.y += sin(rotation * (pi / 180.0)) * 20;
		}
		window_swap();
	}

	map_free(&map);
	wad_close(&wadFile);
}