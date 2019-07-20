#include "DollLogic.h"
#include <genesis.h>
#include "Xor.h"
#include "Map.h"
#include "TileDefinitions.h"
#include "CommonLogic.h"

#include "../res/sprite.h"

static ActiveTileItem _dollTile;


//
// Dolls will only move if pushed and will keep moving in the direction pushed till hitting an object.
// They can't set off bombs
//

void DollSetup()
{
	_dollTile.ActiveSprite = SPR_addSprite(&sp_set2, 0, 0, TILE_ATTR(PAL1, TRUE, FALSE, FALSE));
	SPR_setVisibility(_dollTile.ActiveSprite, HIDDEN);
	SPR_setFrame(_dollTile.ActiveSprite, 2);
	_dollTile.IsActive = FALSE;
}

u8 DollPushUp(u16 x, u16 y)
{
	u16 yCheck = y - 1;

	u8 tile = CurrentMapDataState[MAP_XY_TO_TILE(x, yCheck)];

	if (tile == TILE_TYPE_FLOOR)
	{
		DoMovementCommon(x, y, x, yCheck, MOVE_DIRECTION_UP, -1, &_dollTile, NULL);
		return TRUE;
	}
	
	return FALSE;
}


u8 DollPushDown(u16 x, u16 y)
{
	u16 yCheck = y + 1;

	u8 tile = CurrentMapDataState[MAP_XY_TO_TILE(x, yCheck)];

	if (tile == TILE_TYPE_FLOOR)
	{
		DoMovementCommon(x, y, x, yCheck, MOVE_DIRECTION_DOWN, -1, &_dollTile, NULL);
		return TRUE;
	}

	return FALSE;

}


u8 DollPushLeft(u16 x, u16 y)
{
	u16 xCheck = x - 1;

	u8 tile = CurrentMapDataState[MAP_XY_TO_TILE(xCheck, y)];

	if (tile == TILE_TYPE_FLOOR)
	{
		DoMovementCommon(x, y, xCheck, y, MOVE_DIRECTION_LEFT, -1, &_dollTile, NULL);
		return TRUE;
	}

	return FALSE;
}


u8 DollPushRight(u16 x, u16 y)
{
	
	u16 xCheck = x + 1;

	u8 tile = CurrentMapDataState[MAP_XY_TO_TILE(xCheck, y)];

	if (tile == TILE_TYPE_FLOOR)
	{
		DoMovementCommon(x, y, xCheck, y, MOVE_DIRECTION_RIGHT, -1, &_dollTile, NULL);
		return TRUE;
	}

	return FALSE;
}





u8 DollUpdateMovement()
{
	return UpdateMovementCommon(&_dollTile);
}


u8 DollUpdateLogic()
{
	if (_dollTile.IsActive)
	{
		u8 canMove = FALSE;

		switch (_dollTile.MoveDirection)
		{
			case MOVE_DIRECTION_UP:
				canMove = DollPushUp(_dollTile.DestinationMetaX, _dollTile.DestinationMetaY);
				break;

			case MOVE_DIRECTION_DOWN:
				canMove = DollPushDown(_dollTile.DestinationMetaX, _dollTile.DestinationMetaY);
				break;

			case MOVE_DIRECTION_LEFT:
				canMove = DollPushLeft(_dollTile.DestinationMetaX, _dollTile.DestinationMetaY);
				break;

			case MOVE_DIRECTION_RIGHT:
				canMove = DollPushRight(_dollTile.DestinationMetaX, _dollTile.DestinationMetaY);
				break;
		}

		if (canMove)
		{
			return TRUE;
		}
		else
		{
			_dollTile.IsActive = FALSE;
			return FALSE;
		}
	}


	return FALSE;
}

void DollFinishMovement()
{
	if (_dollTile.IsActive)
	{
		if (_dollTile.OnScreen)
		{
			UpdateTile(_dollTile.DestinationMetaX, _dollTile.DestinationMetaY, TILE_TYPE_DOLL);
			SPR_setVisibility(_dollTile.ActiveSprite, HIDDEN);
		}
		else
		{
			CurrentMapDataState[MAP_XY_TO_TILE(_dollTile.DestinationMetaX, _dollTile.DestinationMetaY)] = TILE_TYPE_DOLL;
		}
	}

}
