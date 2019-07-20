#include <genesis.h>
#include "ChickenLogic.h"
#include "Map.h"
#include "GameMaps.h"
#include "TileDefinitions.h"
#include "Xor.h"
#include "Players.h"
#include "BombH.h"
#include "BombV.h"
#include "Explosion.h"
#include "CommonLogic.h"
#include "../res/sprite.h"





#define CHICKEN_MAX 100
static ActivePoint _chickens[CHICKEN_MAX];
static u8 _chickenCount;

static ActiveTileItem _chickenTile;


void ChickenMakeSprite()
{
	_chickenTile.ActiveSprite = SPR_addSprite(&ShieldSprites, 0, 0, TILE_ATTR(PAL2, TRUE, FALSE, FALSE));
	SPR_setVisibility(_chickenTile.ActiveSprite, HIDDEN);
	SPR_setFrame(_chickenTile.ActiveSprite, 3);
}

void ChickenSetup()
{
	SPR_setVisibility(_chickenTile.ActiveSprite, HIDDEN);

	_chickenTile.IsActive = FALSE;


	for (u8 i = 0; i < CHICKEN_MAX; i++)
	{
		_chickens[i].X = -1;
		_chickens[i].Y = -1;
		_chickens[i].Active = FALSE;
	}


	_chickenCount = 0;
	for (s8 y = 0; y < MAP_HEIGHT; y++)
	{
		for (s8 x = 31; x >= 0; x--)
		{
			if (CurrentMapDataState[MAP_XY_TO_TILE(x, y)] == TILE_TYPE_CHICKEN)
			{
				_chickens[_chickenCount].X = x;
				_chickens[_chickenCount].Y = y;
				_chickenCount++;
			}
		}
	}
}




u8 ChickenPushUp(u16 x, u16 y)
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
		DoMovementCommon(x, y, x, yCheck, MOVE_DIRECTION_UP, FindId(x, y, _chickens, _chickenCount), &_chickenTile, _chickens);
	}

	return canMove;
}




u8 ChickenPushDown(u16 x, u16 y)
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
		DoMovementCommon(x, y, x, yCheck, MOVE_DIRECTION_DOWN, FindId(x, y, _chickens, _chickenCount), &_chickenTile, _chickens);		
	}

	return canMove;
}





static u8 CanMoveLeft(u8 x, u8 y, u8 trackedId)
{
	u16 xCheck = x - 1;

	u8 tile = CurrentMapDataState[MAP_XY_TO_TILE(xCheck, y)];
	u8 canMove = FALSE;
	u8 canKill = _chickens[trackedId].Active == MOVE_DIRECTION_LEFT;

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
				CurrentMapDataState[MAP_XY_TO_TILE(x, y)] = TILE_TYPE_FLOOR;

				if (tile == TILE_TYPE_BOMB_H)
				{
					TriggerBombH(xCheck, y);
				}
				else if (tile == TILE_TYPE_BOMB_V)
				{
					TriggerBombV(xCheck, y);
				}

				_chickens[trackedId].X = -1;
				_chickens[trackedId].Y = -1;
				_chickens[trackedId].Active = FALSE;
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
		DoMovementCommon(x, y, xCheck, y, MOVE_DIRECTION_LEFT, FindId(x, y, _chickens, _chickenCount), &_chickenTile, _chickens);		
	}

	return canMove;
}



u8 ChickenUpdateMovement()
{
	return UpdateMovementCommon(&_chickenTile);
}

void ChickenFinishMovement()
{
	if (_chickenTile.IsActive)
	{		
		_chickens[_chickenTile.ActiveIndex].X = _chickenTile.DestinationMetaX;
		_chickens[_chickenTile.ActiveIndex].Y = _chickenTile.DestinationMetaY;

		_chickenTile.IsActive = FALSE;
		
		
		if (_chickenTile.OnScreen)
		{
			UpdateTile(_chickenTile.DestinationMetaX, _chickenTile.DestinationMetaY, TILE_TYPE_CHICKEN);
			SPR_setVisibility(_chickenTile.ActiveSprite, HIDDEN);
			SPR_update();
		}
		else
		{
			CurrentMapDataState[MAP_XY_TO_TILE(_chickenTile.DestinationMetaX, _chickenTile.DestinationMetaY)] = TILE_TYPE_CHICKEN;
		}
		

		u8 tile = CurrentMapDataState[MAP_XY_TO_TILE(_chickenTile.DestinationMetaX, _chickenTile.DestinationMetaY)];
		if (CurrentPlayer->MetaX == _chickenTile.DestinationMetaX && CurrentPlayer->MetaY == _chickenTile.DestinationMetaY)
		{
			PlayerKillCurrent();
		}
		else if (tile == TILE_TYPE_MAGNUS || tile == TILE_TYPE_QUESTOR)
		{
			PlayerKillOther();
		}
	}
}


void KillChicken(u16 x, u16 y)
{
	KLog("Killing chicken");
	s8 id = FindId(x, y, _chickens, _chickenCount);
	
	KLog_S1("Chicken id is ", id);
	if (id != -1)
	{
		_chickens[id].X = -1;
		_chickens[id].Y = -1;
		_chickens[id].Active = FALSE;
	}
}

u8 ChickenUpdateLogic()
{
	
	for (u8 i = 0; i < _chickenCount; i++)
	{
		if (_chickens[i].X != -1)
		{
			if (CanMoveLeft(_chickens[i].X, _chickens[i].Y, i))
			{
				return TRUE;
			}
			else
			{
				_chickens[i].Active = MOVE_DIRECTION_NONE;
			}
		}
	}

	return FALSE;
}
