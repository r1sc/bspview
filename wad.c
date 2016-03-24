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

// WAD info
typedef struct {
	char id[4];
	int numlumps;
	int infotableofs;
} wadinfo_t;

typedef struct {
	wadinfo_t   info;  // WAD file header information
	lumpinfo_t* lumps; // Array of all the lump definitions
} wadfile_t;

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