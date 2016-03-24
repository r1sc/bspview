#pragma once
#include "doom_types.h"

void wad_open(wad_file_t* wad, const char* path);
void wad_close(wad_file_t* wad);
void wad_seek(wad_file_t* wad, wad_lump_t* lump);
wad_lump_t* wad_findLump(wad_file_t* wad, const char* name, wad_lump_t* after);