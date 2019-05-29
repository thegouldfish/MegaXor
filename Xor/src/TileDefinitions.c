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
	TILE_TYPE_WALL,
	FALSE
};

const static TileDefinition Floor =
{
	1,
	0,
	3,
	0,
	TILE_PLAN_B,
	TILE_TYPE_FLOOR,
	FALSE
};


const static TileDefinition ForceFieldH =
{
	2,
	0,
	6,
	0,
	TILE_PLAN_A,
	TILE_TYPE_FORCEFIELD_H,
	FALSE
};

const static TileDefinition ForceFieldV =
{
	3,
	0,
	9,
	0,
	TILE_PLAN_A,
	TILE_TYPE_FORCEFIELD_V,
	FALSE
};

const static TileDefinition Xor =
{
	4,
	0,
	12,
	0,
	TILE_PLAN_A,
	TILE_TYPE_XOR,
	FALSE
};


const static TileDefinition Magus =
{
	5,
	2,
	0,
	0,
	TILE_PLAN_A,
	TILE_TYPE_MAGNUS,
	FALSE
};


const static TileDefinition Exit =
{
	6,
	1,
	0,
	0,
	TILE_PLAN_A,
	TILE_TYPE_EXIT,
	FALSE
};

const static TileDefinition Questor =
{
	7,
	2,
	3,
	0,
	TILE_PLAN_A,
	TILE_TYPE_QUESTOR,
	FALSE
};

const static TileDefinition Mapdef =
{
	8,
	1,
	3,
	0,
	TILE_PLAN_A,
	TILE_TYPE_MAP,
	FALSE
};


const static TileDefinition Fish =
{
	9,
	1,
	6,
	0,
	TILE_PLAN_A,
	TILE_TYPE_FISH,
	FALSE
};

const static TileDefinition Chicken =
{
	10,
	1,
	9,
	0,
	TILE_PLAN_A,
	TILE_TYPE_CHICKEN,
	FALSE
};


const static TileDefinition BombH =
{
	11,
	1,
	12,
	0,
	TILE_PLAN_A,
	TILE_TYPE_BOMB_H,
	FALSE
};

const static TileDefinition BombV =
{
	12,
	1,
	15,
	0,
	TILE_PLAN_A,
	TILE_TYPE_BOMB_V,
	FALSE
};


const static TileDefinition Explosion =
{
	13,
	0,
	15,
	0,
	TILE_PLAN_A,
	TILE_TYPE_EXPLOSION,
	TRUE
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
	&Chicken,
	&BombH,
	&BombV,
	&Explosion
};