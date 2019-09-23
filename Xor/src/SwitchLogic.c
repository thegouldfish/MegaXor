#include "SwitchLogic.h"
#include <genesis.h>
#include "Map.h"
#include "TileDefinitions.h"

#include "TileLoading.h"

#include "../res/gfx.h"

static u8 _switchActive;


static void SetSwitchTiles()
{
	if (_switchActive)
	{
		BlankTiles(TILE_TYPE_WALL);
		BlankTiles(TILE_TYPE_FLOOR);
	}
	else
	{
		ReLoadTiles(TILE_TYPE_WALL);
		ReLoadTiles(TILE_TYPE_FLOOR);
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
