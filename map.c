#include <stdlib.h>
#include "doom_types.h"
#include "wad.h"

void map_load(map_t* map, wad_file_t* wad, const char* name) {
	wad_lump_t* mapLump = wad_findLump(wad, name, NULL);
	wad_lump_t* thingsLump = wad_findLump(wad, "THINGS", mapLump);
	map->numthings = thingsLump->size / 10;
	map->things = malloc(thingsLump->size);
	fread(map->things, sizeof(thing_t), map->numthings, wad->file);
}