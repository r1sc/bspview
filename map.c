#include <stdlib.h>
#include "doom_types.h"
#include "wad.h"

void map_read_part(wad_file_t* wad, wad_lump_t* mapLump, size_t structSize, const char* partName, int* pNumContainer, void** pBuffer) {
	wad_lump_t* lump = wad_findLump(wad, partName, mapLump);
	*pNumContainer = lump->size / structSize;
	*pBuffer = malloc(lump->size);
	fread(*pBuffer, structSize, *pNumContainer, wad->file);
}

void map_load(map_t* map, wad_file_t* wad, const char* name) {
	wad_lump_t* mapLump = wad_findLump(wad, name, NULL);
	map_read_part(wad, mapLump, sizeof(thing_t), "THINGS", &map->numThings, &map->things);
	map_read_part(wad, mapLump, sizeof(linedef_t), "LINEDEFS", &map->numLinedefs, &map->linedefs);
	map_read_part(wad, mapLump, sizeof(sidedef_t), "SIDEDEFS", &map->numSidedefs, &map->sidedefs);
	map_read_part(wad, mapLump, sizeof(vertex_t), "VERTEXES", &map->numVertexes, &map->vertexes);
	map_read_part(wad, mapLump, sizeof(seg_t), "SEGS", &map->numSegs, &map->segs);
	map_read_part(wad, mapLump, sizeof(ssector_t), "SSECTORS", &map->numSubSectors, &map->subSectors);
	map_read_part(wad, mapLump, sizeof(node_t), "NODES", &map->numNodes, &map->nodes);
	map_read_part(wad, mapLump, sizeof(sector_t), "SECTORS", &map->numSectors, &map->sectors);
}

void map_free(map_t* map) {
	free(map->things);
	free(map->linedefs);
	free(map->sidedefs);
	free(map->vertexes);
	free(map->segs);
	free(map->subSectors);
	free(map->nodes);
	free(map->sectors);
}