#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "doom_types.h"

void wad_open(wad_file_t* wad, const char* path) {
	wad->wadFile = fopen("DOOM1.WAD", "rb");
	fread(wad->header, sizeof(char), 4, wad->wadFile);
	fread(&wad->numFiles, sizeof(int), 1, wad->wadFile);
	fread(&wad->directoryOffset, sizeof(int), 1, wad->wadFile);
	fseek(wad->wadFile, wad->directoryOffset, SEEK_SET);
	wad->lumps = (wad_lump_t*)calloc(wad->numFiles, sizeof(wad_lump_t));
	fread(wad->lumps, sizeof(wad_lump_t), wad->numFiles, wad->wadFile);
}

void wad_close(wad_file_t* wad) {
	fclose(wad->wadFile);
}

void wad_seek(wad_file_t* wad, wad_lump_t* lump) {
	fseek(wad->wadFile, lump->offset, SEEK_SET);
}

wad_lump_t* wad_findLump(wad_file_t* wad, const char* name) {
	for (int i = 0; i < wad->numFiles; i++) {
		if (strcmp(wad->lumps[i].name, name) == 0)
			return &(wad->lumps[i]);
	}
	return NULL;
}

void main() {
	wad_file_t wadFile;
	wad_open(&wadFile, "..\\DOOM1.WAD");
	for (int i = 0; i < wadFile.numFiles; i++)
	{
		printf("%.8s\n", wadFile.lumps[i].name);
	}
	wad_lump_t* lump = wad_findLump(&wadFile, "DMXGUS");
	wad_seek(&wadFile, lump);
	char* buffer = (char*)malloc(lump->size);
	fread(buffer, 1, lump->size, wadFile.wadFile);
	free(buffer);

	wad_close(&wadFile);
	getchar();
}