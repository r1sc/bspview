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
}