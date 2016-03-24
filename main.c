#include <stdlib.h>
#include <stdio.h>
#include "doom_types.h"
#include "wad.h"
#include "map.h"


void main() {
	wad_file_t wadFile;
	wad_open(&wadFile, "..\\DOOM1.WAD");
	for (int i = 0; i < wadFile.numFiles; i++)
	{
		printf("%.8s\n", wadFile.lumps[i].name);
	}
	wad_lump_t* lump = wad_findLump(&wadFile, "THINGS", NULL);
	wad_seek(&wadFile, lump);
	char* buffer = (char*)malloc(lump->size);
	fread(buffer, 1, lump->size, wadFile.file);
	free(buffer);

	map_t map;
	map_load(&map, &wadFile, "E1M1");

	wad_close(&wadFile);
	getchar();
}