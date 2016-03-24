#pragma once
#include "doom_types.h"

void map_load(map_t* map, wad_file_t* wad, const char* name);
void map_free(map_t* map);