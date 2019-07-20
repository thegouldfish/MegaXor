#ifndef MAPS_
#define MAPS_

#include <genesis.h>
#include "Xor.h"



#define MAP_STRIDE 32
#define MAP_HEIGHT 32
#define MAP_SIZE (MAP_STRIDE * MAP_HEIGHT)

#define MAP_COUNT 9


#define MAP_XY_TO_TILE(x, y) ((y << 5) + x)

typedef struct
{
	char* MapName;
	u8 MapData[MAP_SIZE];
	char Solution[MAX_MOVE_COUNT];
} MapDefinition;



extern u16 MapMoveCounts[MAP_COUNT];

const extern MapDefinition* GameMaps[MAP_COUNT];

#endif