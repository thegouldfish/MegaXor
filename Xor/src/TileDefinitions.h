#ifndef TILE_DEFINITIONS_
#define TILE_DEFINITIONS_

#include <genesis.h>

#define TILECOUNT 16

// All the various Tile Types and the id that will be provided by the map conversion tool

#define TILE_TYPE_WALL 0
#define TILE_TYPE_FLOOR 1
#define TILE_TYPE_FORCEFIELD_H 2
#define TILE_TYPE_FORCEFIELD_V 3
#define TILE_TYPE_XOR 4

#define TILE_TYPE_MAGNUS 5
#define TILE_TYPE_EXIT 6
#define TILE_TYPE_QUESTOR 7
#define TILE_TYPE_MAP 8

#define TILE_TYPE_FISH 9
#define TILE_TYPE_CHICKEN 10

#define TILE_TYPE_BOMB_H 11
#define TILE_TYPE_BOMB_V 12

#define TILE_TYPE_EXPLOSION 13

#define TILE_TYPE_DOLL 14

#define TILE_TYPE_SWITCH 15

#define TILE_TYPE_TELEPORTER 16


// The plan the tile should be shown on
#define TILE_PLAN_A 0
#define TILE_PLAN_B 1
#define TILE_PLAN_S 2


// Each tile type requires details on what to draw, the priority and the set and animation.
typedef struct
{
	u8 TiledNumber;
	
	u8 TileSet;
	s8 TileId;
	u8 AnimationCount;
	u8 Plan;

	u8 Priority;
} TileDefinition;




const extern TileDefinition* Tiles[TILECOUNT];

#endif