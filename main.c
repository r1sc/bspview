#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "doom_types.h"
#include "wad.h"
#include "map.h"
#include "window.h"

float vec_length(const vertex_t* v1) {
	return (float)sqrt(v1->x*v1->x + v1->y*v1->y);
}

void vec_normalize(vertex_t* v1) {
	float length = vec_length(v1);
	v1->x /= length;
	v1->y /= length;
}

float vec_dot(const vertex_t* v1, const vertex_t* v2) {
	return (v1->x * v2->x) + (v1->y * v2->y);
}

int isPointInBoundingBox(const bbox_t* box, const vertex_t* point) {
	if (box->top > point->y && box->bottom < point->y && box->left < point->x && box->right > point->x)
		return TRUE;
	return FALSE;
}

#define CP_FRONT 1
#define CP_BACK 2
#define CP_ONPLANE 3

#define NODETYPE_NODE 1
#define NODETYPE_SUBSECTOR 2

map_t map;

int classifyNode(node_t* node, vertex_t* point) {

	vertex_t normal;
	normal.x = node->partitionXOffset;
	normal.y = node->partitionYOffset;
	//vec_normalize(&normal);
	vertex_t direction;
	direction.x = node->partitionX - point->x;
	direction.y = node->partitionY - point->y;
	float result = vec_dot(&direction, &normal);
	if (result < -0.001)
		return CP_FRONT;
	if (result > 0.001)
		return CP_BACK;
	return CP_ONPLANE;
}

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

void traverseTree(node_t* node, vertex_t* eye);
void handleNode(short nodeValue, vertex_t* eye, int side) {
	short value = nodeValue & 0x7FFF;
	if (nodeValue >> 15) {
		// TODO: Render?
		ssector_t ssector = map.subSectors[value];
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

			if (lineDef->sidenum[0] != -1) {
				sidedef_t leftSide = map.sidedefs[lineDef->sidenum[0]];
				sector_t leftSector = map.sectors[leftSide.sectornum];
				floorHeightMin = leftSector.floorheight;
				floorHeightMax = leftSector.floorheight;
				ceilingHeightMin = leftSector.ceilingheight;
				ceilingHeightMax = leftSector.ceilingheight;
			}

			if (lineDef->sidenum[1] != -1) {
				sidedef_t rightSide = map.sidedefs[lineDef->sidenum[1]];
				sector_t rightSector = map.sectors[rightSide.sectornum];
				floorHeightMin = min(floorHeightMin, rightSector.floorheight);
				ceilingHeightMin = min(ceilingHeightMin, rightSector.ceilingheight);
				floorHeightMax = max(floorHeightMax, rightSector.floorheight);
				ceilingHeightMax = max(ceilingHeightMax, rightSector.ceilingheight);

			}

			glColor3ub(sectorColorMap[(seg.linedefIndex % 9) * 3], sectorColorMap[(seg.linedefIndex % 9) * 3 + 1], sectorColorMap[(seg.linedefIndex % 9) * 3 + 2]);

			if (lineDef->sidenum[0] == -1 || lineDef->sidenum[1] == -1) {
				drawQuad(v1.x, v1.y, v2.x, v2.y, floorHeightMax, ceilingHeightMin);
				continue;
			}
			if (floorHeightMin != floorHeightMax) {
				drawQuad(v1.x, v1.y, v2.x, v2.y, floorHeightMin, floorHeightMax);
			}
			if (ceilingHeightMin != ceilingHeightMax) {
				drawQuad(v1.x, v1.y, v2.x, v2.y, ceilingHeightMin, ceilingHeightMax);
			}
			/*glVertex3s(v1.x, sector.floorheight, v1.y);
			glVertex3s(v1.x, sector.ceilingheight, v1.y);*/
		}
	}
	else {
		node_t* childNode = &map.nodes[value];
		traverseTree(childNode, eye);
	}
}

void traverseTree(node_t* node, vertex_t* eye) {
	int result = classifyNode(node, eye);
	if (result == CP_BACK) {
		handleNode(node->leftNode, eye, 0);
		handleNode(node->rightNode, eye, 1);
		return;
	}
	else {
		handleNode(node->rightNode, eye, 0);
		handleNode(node->leftNode, eye, 1);
	}
	/*handleNode(node->rightNode, eye);
	handleNode(node->leftNode, eye);*/
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
	for (int i = 0; i < wadFile.numFiles; i++)
	{
		printf("%.8s\n", wadFile.lumps[i].name);
	}

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

	if (root.leftBBox.top > root.rightBBox.top)
		top = root.leftBBox.top;
	else
		top = root.rightBBox.top;

	if (root.leftBBox.bottom > root.rightBBox.bottom)
		bottom = root.rightBBox.bottom;
	else
		bottom = root.leftBBox.bottom;

	if (root.leftBBox.right > root.rightBBox.right)
		right = root.leftBBox.right;
	else
		right = root.rightBBox.right;

	if (root.leftBBox.left > root.rightBBox.left)
		left = root.rightBBox.left;
	else
		left = root.leftBBox.left;

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
		traverseTree(&root, &eye);
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