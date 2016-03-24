#pragma once

// WAD
typedef struct {
	int offset;
	int size;
	char name[8];
} wad_lump_t;

typedef struct {
	FILE* wadFile;
	char header[4];
	int numFiles;
	int directoryOffset;
	wad_lump_t* lumps;
} wad_file_t;


typedef struct {
	int x, y; // x, y position
} vertex_t;


// SIDEDEF 
typedef struct {
	int		xofs;			// Texture x offset
	int		yofs;			// Texture y offset
	char	uppertexture[8];
	char	lowertexture[8];
	char	middletexture[8];
	int		sectornum;		// Sector number this sidedef faces
} sidedef_t;

// ALL TEH THINGS!!
typedef struct {
	int x, y;  // x, y position
	int angle; // Facing angle
	int type;  // DoomEd thing type
	int flags; // Flags
} thing_t;

// LINEDEF 
typedef struct {
	int v1, v2;     // Start and end vertex
	int flags;      // Flags
	int special;    // Special type
	int tag;        // Sector tag
	int sidenum[2]; // sidenum[1] is back side, -1 if one-sided
} linedef_t;

// SECTOR 
typedef struct {
	int		floorheight;
	int		ceilingheight;
	char	floorflat[8];
	char    ceilingflat[8];
	int		lightlevel;
	int		type;
	int		tag;
} sector_t;

typedef struct {
	char		name[8];
	int			width, height;
	vertex_t	centerv;

	int			numthings;
	thing_t*	things;

	int			numlinedefs;
	linedef_t*	linedefs;

	int			numsidedefs;
	sidedef_t*	sidedefs;

	int			numvertexes;
	vertex_t*	vertexes;

	int			numsectors;
	sector_t*	sectors;
} map_t;

// Lump info
typedef struct {
	int filepos;
	int size;
	char name[8];
} lumpinfo_t;