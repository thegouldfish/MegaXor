#include <genesis.h>
#include "BombV.h"
#include "Map.h"
#include "GameMaps.h"
#include "TileDefinitions.h"
#include "Xor.h"
#include "Players.h"
#include "BombH.h"
#include "Explosion.h"
#include "CommonLogic.h"

#include "../res/sprite.h"


static ActiveTileItem _bombVTile;


#define BOMBV_MAX 100
static ActivePoint _bombVs[BOMBV_MAX];
static u8 _bombVCount;

void BombVSetup()
{
	_bombVTile.ActiveSprite = SPR_addSprite(&sp_set2, 0, 0, TILE_ATTR(PAL1, TRUE, FALSE, FALSE));
	SPR_setVisibility(_bombVTile.ActiveSprite, HIDDEN);
	SPR_setFrame(_bombVTile.ActiveSprite, 1);
	_bombVTile.IsActive = FALSE;


	for (u8 i = 0; i < BOMBV_MAX; i++)
	{
		_bombVs[i].X = -1;
		_bombVs[i].Y = -1;
		_bombVs[i].Active = FALSE;
	}


	_bombVCount = 0;
	for (s8 y = 0; y < MAP_HEIGHT; y++)
	{
		for (s8 x = 31; x >= 0; x--)
		{
			if (CurrentMapDataState[MAP_XY_TO_TILE(x, y)] == TILE_TYPE_BOMB_V)
			{
				_bombVs[_bombVCount].X = x;
				_bombVs[_bombVCount].Y = y;
				_bombVCount++;
			}
		}
	}
}




u8 BombVPushUp(u16 x, u16 y)
{
	u16 yCheck = y - 1;

	u8 tile = CurrentMapDataState[MAP_XY_TO_TILE(x, yCheck)];

	u8 canMove = FALSE;


	switch (tile)
	{
		case TILE_TYPE_FORCEFIELD_V:
		case TILE_TYPE_FLOOR:
			canMove = TRUE;
			break;

		default:
			break;
	}



	if (canMove)
	{
		DoMovementCommon(x, y, x, yCheck, MOVE_DIRECTION_UP, FindId(x, y, _bombVs, _bombVCount), &_bombVTile, _bombVs);
	}

	return canMove;
}




u8 BombVPushDown(u16 x, u16 y)
{
	u16 yCheck = y + 1;

	u8 tile = CurrentMapDataState[MAP_XY_TO_TILE(x, yCheck)];

	u8 canMove = FALSE;


	switch (tile)
	{
		case TILE_TYPE_FORCEFIELD_V:
		case TILE_TYPE_FLOOR:
			canMove = TRUE;
			break;


		default:
			break;
	}



	if (canMove)
	{
		DoMovementCommon(x, y, x, yCheck, MOVE_DIRECTION_DOWN, FindId(x, y, _bombVs, _bombVCount), &_bombVTile, _bombVs);
	}

	return canMove;
}





static u8 CanMoveLeft(u8 x, u8 y, u8 trackedId)
{
	u16 xCheck = x - 1;


	u8 tile = CurrentMapDataState[MAP_XY_TO_TILE(xCheck, y)];

	u8 canMove = FALSE;


	u8 canKill = _bombVs[trackedId].Active;



	switch (tile)
	{
		case TILE_TYPE_FORCEFIELD_H:
		case TILE_TYPE_FLOOR:
			canMove = TRUE;
			break;

		case TILE_TYPE_BOMB_H:
		case TILE_TYPE_BOMB_V:
			if (canKill)
			{
				_bombVTile.IsActive = FALSE;

				if (tile == TILE_TYPE_BOMB_H)
				{
					TriggerBombH(xCheck, y);
				}
				else if (tile == TILE_TYPE_BOMB_V)
				{
					TriggerBombV(xCheck, y);
				}

				_bombVs[trackedId].X = -1;
				_bombVs[trackedId].Y = -1;
				_bombVs[trackedId].Active = FALSE;
				PlaceExplosion(x, y);
				canMove = FALSE;
			}
			break;

		default:
			break;
	}


	if (CurrentPlayer->MetaX == xCheck && CurrentPlayer->MetaY == y && !canKill)
	{
		canMove = FALSE;
	}


	if (canMove)
	{
		KLog("BombV - canMove");
		DoMovementCommon(x, y, xCheck, y, MOVE_DIRECTION_LEFT, trackedId, &_bombVTile, _bombVs);
	}

	return canMove;
}



u8 BombVUpdateMovement()
{
	return UpdateMovementCommon(&_bombVTile);
}

void BombVFinishMovement()
{
	if (_bombVTile.IsActive)
	{
		_bombVs[_bombVTile.ActiveIndex].X = _bombVTile.DestinationMetaX;
		_bombVs[_bombVTile.ActiveIndex].Y = _bombVTile.DestinationMetaY;

		_bombVTile.IsActive = FALSE;

		if (_bombVTile.OnScreen)
		{
			UpdateTile(_bombVTile.DestinationMetaX, _bombVTile.DestinationMetaY, TILE_TYPE_BOMB_V);
			SPR_setVisibility(_bombVTile.ActiveSprite, HIDDEN);
		}
		else
		{
			CurrentMapDataState[MAP_XY_TO_TILE(_bombVTile.DestinationMetaX, _bombVTile.DestinationMetaY)] = TILE_TYPE_BOMB_V;
		}

		u8 tile = CurrentMapDataState[MAP_XY_TO_TILE(_bombVTile.DestinationMetaX, _bombVTile.DestinationMetaY)];
		if (CurrentPlayer->MetaX == _bombVTile.DestinationMetaX && CurrentPlayer->MetaY == _bombVTile.DestinationMetaY)
		{
			PlayerKillCurrent();
		}
		else if (tile == TILE_TYPE_MAGNUS || tile == TILE_TYPE_QUESTOR)
		{
			PlayerKillOther();
		}
	}
}


u8 BombVUpdateLogic()
{
	KLog("BombVUpdateLogic");
	for (u8 i = 0; i < _bombVCount; i++)
	{
		if (_bombVs[i].X != -1)
		{
			if (CanMoveLeft(_bombVs[i].X, _bombVs[i].Y, i))
			{
				KLog_U1(" Can move left ", i);
				return TRUE;
			}
			else
			{
				_bombVs[i].Active = FALSE;
			}
		}
	}

	return FALSE;
}


void TriggerBombV(u16 x, u16 y)
{
	u16 index = FindId(x, y, _bombVs, _bombVCount);

	u16 up = y - 1;
	u16 down = y + 1;

	_bombVs[index].X = -1;
	_bombVs[index].Y = -1;
	_bombVs[index].Active = FALSE;
	_bombVTile.IsActive = FALSE;

	PlaceExplosion(x, y);
	PlaceExplosion(x, up);
	PlaceExplosion(x, down);
}