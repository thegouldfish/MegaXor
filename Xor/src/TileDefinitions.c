#include <genesis.h>
#include "TileDefinitions.h"

#include "../res/gfx.h"

const static TileDefinition Wall =
{
	0,
	0,
	0,
	0,
	TILE_PLAN_B,	
	TILE_TYPE_WALL
};

const static TileDefinition Floor =
{
	1,
	0,
	3,
	0,
	TILE_PLAN_B,
	TILE_TYPE_FLOOR
};


const static TileDefinition ForceFieldH =
{
	2,
	0,
	6,
	0,
	TILE_PLAN_A,
	TILE_TYPE_FORCEFIELD_H
};

const static TileDefinition ForceFieldV =
{
	3,
	0,
	9,
	0,
	TILE_PLAN_A,
	TILE_TYPE_FORCEFIELD_V
};

const static TileDefinition Xor =
{
	4,
	0,
	12,
	0,
	TILE_PLAN_A,
	TILE_TYPE_XOR
};


const static TileDefinition Magus =
{
	5,
	2,
	0,
	0,
	TILE_PLAN_A,
	TILE_TYPE_MAGNUS	
};


const static TileDefinition Exit =
{
	6,
	1,
	0,
	0,
	TILE_PLAN_A,
	TILE_TYPE_EXIT
};

const static TileDefinition Questor =
{
	7,
	2,
	3,
	0,
	TILE_PLAN_A,
	TILE_TYPE_QUESTOR
};

const static TileDefinition Mapdef =
{
	8,
	1,
	3,
	0,
	TILE_PLAN_A,
	TILE_TYPE_MAP
};


const static TileDefinition Fish =
{
	9,
	1,
	6,
	0,
	TILE_PLAN_A,
	TILE_TYPE_FISH
};

const static TileDefinition Chicken =
{
	10,
	1,
	9,
	0,
	TILE_PLAN_A,
	TILE_TYPE_CHICKEN
};

const TileDefinition* Tiles[TILECOUNT] =
{
	&Wall,
	&Floor,
	&ForceFieldH,
	&ForceFieldV,
	&Xor,
	&Magus,
	&Exit,
	&Questor,
	&Mapdef,
	&Fish,
	&Chicken
};