#include "SwitchLogic.h"
#include <genesis.h>
#include "Map.h"
#include "TileDefinitions.h"
#include "../res/gfx.h"

static u8 _switchActive;


static void SetSwitchTiles()
{
	if (_switchActive)
	{
		
		u32 tilePtr[3 * 8] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
		
		// load empty tiles into the wall vram slot
		u16 index = TILE_USERINDEX;
		for (int i = 0; i < 3; i++)
		{
			VDP_loadTileData(tilePtr, index, 3, CPU);
			index += TileIndexOffset;
		}

		// load empty tiles into the ground vram slot
		index = TILE_USERINDEX + 3;
		for (int i = 0; i < 3; i++)
		{
			VDP_loadTileData(tilePtr, index, 3, CPU);
			index += TileIndexOffset;
		}
	}
	else
	{
		// Load wall tiledata into vram
		u16 index = TILE_USERINDEX;
		u32* tilePtr = tile_set1.tileset->tiles;

		for (int i = 0; i < 3; i++)
		{
			VDP_loadTileData(tilePtr, index, 3, CPU);
			index += TileIndexOffset;
			tilePtr += 192;
		}

		// load ground tiledata into vram
		index = TILE_USERINDEX + 3;
		tilePtr = tile_set1.tileset->tiles;
		tilePtr += 24;

		for (int i = 0; i < 3; i++)
		{
			VDP_loadTileData(tilePtr, index, 3, CPU);
			index += TileIndexOffset;
			tilePtr += 192;
		}
	}
}



void ResetSwitches()
{
	_switchActive = FALSE;
	SetSwitchTiles();
}




void SwitchToggle(u8 x, u8 y)
{
	_switchActive = (_switchActive) ? FALSE : TRUE;
	UpdateTile(x, y, TILE_TYPE_FLOOR);
	SetSwitchTiles();
}
