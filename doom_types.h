#pragma once
#include <stdio.h>

// WAD
typedef struct {
	int offset;
	int size;
	char name[8];
} wad_lump_t;

typedef struct {
	FILE* file;
	char header[4];
	int numFiles;
	int directoryOffset;
	wad_lump_t* lumps;
} wad_file_t;


typedef struct {
	short x, y; // x, y position
} vertex_t;


// SIDEDEF 
typedef struct {
	short	xofs;			// Texture x offset
	short	yofs;			// Texture y offset
	char	uppertexture[8];
	char	lowertexture[8];
	char	middletexture[8];
	short	sectornum;		// Sector number this sidedef faces
} sidedef_t;

// ALL TEH THINGS!!
typedef struct {
	short x, y;  // x, y position
	short angle; // Facing angle
	short type;  // DoomEd thing type
	short flags; // Flags
} thing_t;

// LINEDEF 
typedef struct {
	short v1, v2;     // Start and end vertex
	short flags;      // Flags
	short special;    // Special type
	short tag;        // Sector tag
	short sidenum[2]; // sidenum[1] is back side, -1 if one-sided
} linedef_t;

typedef struct {
	short startVertexIndex;
	short endVertexIndex;
	short angle;
	short linedefIndex;
	short direction;
	short offset;
}  seg_t;

typedef struct {
	short segCount;
	short firstSegIndex;
} ssector_t;

typedef struct {
	short top, bottom, left, right;
} bbox_t;

typedef struct {
	short partitionX;
	short partitionY;
	short partitionXOffset;
	short partitionYOffset;
	bbox_t leftBBox;
	bbox_t rightBBox;
	short leftNode;
	short rightNode;
} node_t;

// SECTOR 
typedef struct {
	short	floorheight;
	short	ceilingheight;
	char	floorflat[8];
	char    ceilingflat[8];
	short	lightlevel;
	short	type;
	short	tag;
} sector_t;

typedef struct {
	char		name[8];
	int			width, height;
	vertex_t	centerv;

	int			numThings;
	thing_t*	things;

	int			numLinedefs;
	linedef_t*	linedefs;

	int			numSidedefs;
	sidedef_t*	sidedefs;

	int			numVertexes;
	vertex_t*	vertexes;

	int			numSectors;
	sector_t*	sectors;

	int			numSegs;
	seg_t*		segs;

	int			numNodes;
	node_t*		nodes;

	int			numSubSectors;
	seg_t*		subSectors;
} map_t;

// Lump info
typedef struct {
	int filepos;
	int size;
	char name[8];
} lumpinfo_t;