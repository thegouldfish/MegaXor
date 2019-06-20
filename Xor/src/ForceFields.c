#include <genesis.h>

#include "ForceFields.h"
#include "TileDefinitions.h"
#include "Map.h"

#include "../res/gfx.h"


static u8 _horizontalFrame;

static u8 _forceFieldHTimer;
static u8 _forceFieldHWait;


static u8 _verticalFrame;

static u8 _forceFieldVTimer;
static u8 _forceFieldVWait;


void SetupForceFields()
{
	if (IS_PALSYSTEM)
	{
		_forceFieldHWait = 13;
		_forceFieldVWait = 13;
	}
	else
	{
		_forceFieldHWait = 16;
		_forceFieldVWait = 16;
	}

	_verticalFrame = 0;
	_horizontalFrame = 0;

	_forceFieldHTimer = 0;
	_forceFieldVTimer = 0;
}


void UpdateForceFieldH()
{
	_forceFieldHTimer++;
	if (_forceFieldHTimer >= _forceFieldHWait)
	{
		_horizontalFrame++;
		if (_horizontalFrame > Tiles[TILE_TYPE_FORCEFIELD_H]->AnimationCount)
		{
			_horizontalFrame = 0;
		}

		u16 index = TILE_USERINDEX + Tiles[TILE_TYPE_FORCEFIELD_H]->TileId;
		u32* tilePtr = Forcefield_H_anim.tileset->tiles;
		tilePtr += (_horizontalFrame * 24);
		for (int i = 0; i < 3; i++)
		{
			VDP_loadTileData(tilePtr, index, 3, CPU);
			index += TileIndexOffset;
			tilePtr += 96;
		}
		
		_forceFieldHTimer = 0;
	}
}





void UpdateForceFieldV()
{
	_forceFieldVTimer++;
	if (_forceFieldVTimer >= _forceFieldVWait)
	{
		_forceFieldVTimer = 0;
		_verticalFrame++;
		if (_verticalFrame > Tiles[TILE_TYPE_FORCEFIELD_V]->AnimationCount)
		{
			_verticalFrame = 0;
		}

		u16 index = TILE_USERINDEX + Tiles[TILE_TYPE_FORCEFIELD_V]->TileId;
		u32* tilePtr = Forcefield_V_anim.tileset->tiles;
		tilePtr += (_verticalFrame * 24);
		for (int i = 0; i < 3; i++)
		{
			VDP_loadTileData(tilePtr, index, 3, CPU);
			index += TileIndexOffset;
			tilePtr += 96;
		}
	}
}