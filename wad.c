#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "doom_types.h"

void assertexit(int cond, const char* format, ...) {
	if (!cond) {
		va_list args;
		va_start(args, format);
		printf("Error! ");
		vprintf(format, args);
		printf("\n");
		va_end(args);
		exit(1);
	}
}

void wad_open(wad_file_t* wad, const char* path) {
	wad->file = fopen("DOOM1.WAD", "rb");
	fread(wad->header, sizeof(char), 4, wad->file);
	fread(&wad->numFiles, sizeof(int), 1, wad->file);
	fread(&wad->directoryOffset, sizeof(int), 1, wad->file);
	fseek(wad->file, wad->directoryOffset, SEEK_SET);
	wad->lumps = (wad_lump_t*)calloc(wad->numFiles, sizeof(wad_lump_t));	
	fread(wad->lumps, sizeof(wad_lump_t), wad->numFiles, wad->file);
}

void wad_close(wad_file_t* wad) {
	fclose(wad->file);
}

void wad_seek(wad_file_t* wad, wad_lump_t* lump) {
	fseek(wad->file, lump->offset, SEEK_SET);
}

int wad_findLumpIndex(wad_file_t* wad, const char* name, int startIndex) {
	for (int i = startIndex; i < wad->numFiles; i++) {
		if (strncmp(wad->lumps[i].name, name, 8) == 0)
			return i;
	}
	assertexit(0, "Lump '%s' not found!", name);
}

wad_lump_t* wad_findLump(wad_file_t* wad, const char* name, wad_lump_t* after) {
	int startIndex = 0;
	if (after != NULL) {
		startIndex = wad_findLumpIndex(wad, after->name, 0);
	}
	int wadIndex = wad_findLumpIndex(wad, name, startIndex);
	wad_lump_t* lump = &(wad->lumps[wadIndex]);
	wad_seek(wad, lump);
	return lump;
}
