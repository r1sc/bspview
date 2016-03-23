#include <stdio.h>
#include <stdlib.h>

typedef struct {
	char header[4];
	int numFiles;
	int offset;
}t_WAD;

typedef struct {
	int offset;
	int size;
	char name[8];
}t_WAD_Lump;

typedef struct {
	int x, y; // x, y position
} vertex_t;


// SIDEDEF lump struct
typedef struct {
	int		xofs;			// Texture x offset
	int		yofs;			// Texture y offset
	char	uppertexture[8];
	char	lowertexture[8];
	char	middletexture[8];
	int		sectornum;		// Sector number this sidedef faces
} sidedef_t;

// THINGS lump struct
typedef struct {
	int x, y;  // x, y position
	int angle; // Facing angle
	int type;  // DoomEd thing type
	int flags; // Flags
} thing_t;

// LINEDEF lump struct
typedef struct {
	int v1, v2;     // Start and end vertex
	int flags;      // Flags
	int special;    // Special type
	int tag;        // Sector tag
	int sidenum[2]; // sidenum[1] is back side, -1 if one-sided
} linedef_t;

// SECTOR lump struct
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
	int			width, height;	// Dimensions of map
	vertex_t	centerv;		// Center point

	int			numthings;		// Total number of THINGS
	thing_t*	things;			// Array of all the THINGS

	int			numlinedefs;	// Total number of LINEDEFS
	linedef_t*	linedefs;		// Array of all the LINEDEFS

	int			numsidedefs;	// Total number of SIDEDEFS
	sidedef_t*	sidedefs;		// Array of all the SIDEDEFS

	int			numvertexes;	// Total number of VERTEXES
	vertex_t*	vertexes;		// Array of all the VERTEXES

	int			numsectors;		// Total number of SECTORS
	sector_t*	sectors;		// Array of all the SECTORS
} map_t;


void main() {
	FILE* wadFile = fopen("DOOM1.WAD", "rb");
	t_WAD wad;
	fread(&wad, sizeof(t_WAD), 1, wadFile);
	fseek(wadFile, wad.offset, SEEK_SET);
	t_WAD_Lump* lumps = (t_WAD_Lump*)calloc(wad.numFiles, sizeof(t_WAD_Lump));
	fread(lumps, sizeof(t_WAD_Lump), wad.numFiles, wadFile);
	for (int i = 0; i < wad.numFiles; i++)
	{
		printf("%.8s\n", lumps[i].name);
	}
	fclose(wadFile);
	getchar();
}