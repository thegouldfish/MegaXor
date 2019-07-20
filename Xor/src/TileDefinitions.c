#include <genesis.h>
#include "TileDefinitions.h"

#include "../res/gfx.h"

const static TileDefinition Wall =
{
	TILE_TYPE_WALL,
	0,
	0,
	0,
	TILE_PLAN_B,		
	FALSE
};

const static TileDefinition Floor =
{
	TILE_TYPE_FLOOR,
	0,
	3,
	0,
	TILE_PLAN_B,
	FALSE
};


const static TileDefinition ForceFieldH =
{
	TILE_TYPE_FORCEFIELD_H,
	0,
	6,
	3,
	TILE_PLAN_A,
	FALSE
};

const static TileDefinition ForceFieldV =
{
	TILE_TYPE_FORCEFIELD_V,
	0,
	9,
	3,
	TILE_PLAN_A,	
	FALSE
};

const static TileDefinition Xor =
{
	TILE_TYPE_XOR,
	0,
	12,
	0,
	TILE_PLAN_A,	
	FALSE
};


const static TileDefinition Magus =
{
	TILE_TYPE_MAGNUS,
	2,
	0,
	0,
	TILE_PLAN_A,	
	FALSE
};


const static TileDefinition Exit =
{
	TILE_TYPE_EXIT,
	1,
	0,
	0,
	TILE_PLAN_A,	
	FALSE
};

const static TileDefinition Questor =
{
	TILE_TYPE_QUESTOR,
	2,
	3,
	0,
	TILE_PLAN_A,	
	FALSE
};

const static TileDefinition Mapdef =
{
	TILE_TYPE_MAP,
	1,
	3,
	0,
	TILE_PLAN_A,
	FALSE
};


const static TileDefinition Fish =
{
	TILE_TYPE_FISH,
	1,
	6,
	0,
	TILE_PLAN_A,	
	FALSE
};

const static TileDefinition Chicken =
{
	TILE_TYPE_CHICKEN,
	1,
	9,
	0,
	TILE_PLAN_A,
	FALSE
};


const static TileDefinition BombH =
{
	TILE_TYPE_BOMB_H,
	1,
	12,
	0,
	TILE_PLAN_A,	
	FALSE
};

const static TileDefinition BombV =
{
	TILE_TYPE_BOMB_V,
	1,
	15,
	0,
	TILE_PLAN_A,	
	FALSE
};


const static TileDefinition Explosion =
{
	TILE_TYPE_EXPLOSION,
	0,
	15,
	0,
	TILE_PLAN_A,	
	TRUE
};


const static TileDefinition Doll =
{
	TILE_TYPE_DOLL,
	1,
	18,
	0,
	TILE_PLAN_A,	
	FALSE
};


const static TileDefinition Switch =
{
	TILE_TYPE_SWITCH,
	0,
	18,
	0,
	TILE_PLAN_A,	
	FALSE
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
	&Explosion,
	&Doll,
	&Switch
};