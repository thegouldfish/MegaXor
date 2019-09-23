#include <genesis.h>
#include "BombH.h"
#include "Map.h"
#include "GameMaps.h"
#include "Xor.h"
#include "Players.h"
#include "Explosion.h"
#include "BombV.h"
#include "CommonLogic.h"
#include "TileDefinitions.h"

#include "../res/sprite.h"
#include "TileLoading.h"

static ActiveTileItem _bombHsTile;

#define BOMBH_MAX 100
static ActivePoint _bombHs[BOMBH_MAX];
static u8 _bombHsCount;


void BombHMakeSprite()
{
	_bombHsTile.ActiveSprite = SPR_addSprite(LoadedTiles[TILE_TYPE_BOMB_H].GraphicsDefinition->Sprite, 0, 0, TILE_ATTR(PAL1, TRUE, FALSE, FALSE));
	SPR_setVisibility(_bombHsTile.ActiveSprite, HIDDEN);
	SPR_setFrame(_bombHsTile.ActiveSprite, 2);
}

void BombHSetup()
{
	_bombHsTile.IsActive = FALSE;
	SPR_setVisibility(_bombHsTile.ActiveSprite, HIDDEN);


	for (u8 i = 0; i < BOMBH_MAX; i++)
	{
		_bombHs[i].X = -1;
		_bombHs[i].Y = -1;
		_bombHs[i].Active = FALSE;
	}


	_bombHsCount = 0;
	for (s8 y = 0; y < MAP_HEIGHT; y++)
	{
		for (s8 x = 31; x >= 0; x--)
		{
			if (CurrentMapDataState[MAP_XY_TO_TILE(x, y)] == TILE_TYPE_BOMB_H)
			{
				_bombHs[_bombHsCount].X = x;
				_bombHs[_bombHsCount].Y = y;
				_bombHsCount++;
			}
		}
	}

}



u8 BombHPushLeft(u16 x, u16 y)
{
	u16 xCheck = x - 1;

	u8 tile = CurrentMapDataState[MAP_XY_TO_TILE(xCheck, y)];

	u8 canMove = FALSE;


	switch (tile)
	{
		case TILE_TYPE_FORCEFIELD_H:
		case TILE_TYPE_FLOOR:
			canMove = TRUE;


		default:
			break;
	}



	if (canMove)
	{
		DoMovementCommon(x, y, xCheck, y, MOVE_DIRECTION_LEFT, FindId(x, y, _bombHs, _bombHsCount), &_bombHsTile, _bombHs);		
	}

	return canMove;
}




u8 BombHPushRight(u16 x, u16 y)
{

	u16 xCheck = x + 1;

	u8 tile = CurrentMapDataState[MAP_XY_TO_TILE(xCheck, y)];

	u8 canMove = FALSE;


	switch (tile)
	{
		case TILE_TYPE_FORCEFIELD_H:
		case TILE_TYPE_FLOOR:
			canMove = TRUE;
			break;

		default:
			break;
	}



	if (canMove)
	{
		DoMovementCommon(x, y, xCheck, y, MOVE_DIRECTION_RIGHT, FindId(x, y, _bombHs, _bombHsCount), &_bombHsTile, _bombHs);		
	}

	return canMove;
}





static u8 CanMoveDown(u8 x, u8 y, u8 trackedId)
{
	u16 yCheck = y + 1;


	u8 tile = CurrentMapDataState[MAP_XY_TO_TILE(x, yCheck)];

	u8 canMove = FALSE;

	u8 canKill = _bombHs[trackedId].Active == MOVE_DIRECTION_DOWN;



	switch (tile)
	{
		case TILE_TYPE_FORCEFIELD_V:
		case TILE_TYPE_FLOOR:
			canMove = TRUE;
			break;


		case TILE_TYPE_BOMB_H:
		case TILE_TYPE_BOMB_V:
			if (canKill)
			{
				if (tile == TILE_TYPE_BOMB_H)
				{
					TriggerBombH(x, yCheck);
				}
				else if (tile == TILE_TYPE_BOMB_V)
				{
					TriggerBombV(x, yCheck);
				}

				_bombHs[trackedId].X = -1;
				_bombHs[trackedId].Y = -1;
				_bombHs[trackedId].Active = FALSE;
				PlaceExplosion(x, y);
				canMove = FALSE;
			}
			break;

		default:
			break;
	}


	if (CurrentPlayer->MetaX == x && CurrentPlayer->MetaY == yCheck && !canKill)
	{
		canMove = FALSE;
	}


	if (canMove)
	{
		DoMovementCommon(x, y, x, yCheck, MOVE_DIRECTION_DOWN, FindId(x, y, _bombHs, _bombHsCount), &_bombHsTile, _bombHs);
	}

	return canMove;
}

u8 BombHUpdateMovement()
{
	return UpdateMovementCommon(&_bombHsTile);
}

void BombHFinishMovement()
{
	if (_bombHsTile.IsActive)
	{
		_bombHs[_bombHsTile.ActiveIndex].X = _bombHsTile.DestinationMetaX;
		_bombHs[_bombHsTile.ActiveIndex].Y = _bombHsTile.DestinationMetaY;


		if (_bombHsTile.OnScreen)
		{
			UpdateTile(_bombHsTile.DestinationMetaX, _bombHsTile.DestinationMetaY, TILE_TYPE_BOMB_H);
			SPR_setVisibility(_bombHsTile.ActiveSprite, HIDDEN);
		}
		else
		{
			CurrentMapDataState[MAP_XY_TO_TILE(_bombHsTile.DestinationMetaX, _bombHsTile.DestinationMetaY)] = TILE_TYPE_BOMB_H;
		}

		_bombHsTile.IsActive = FALSE;
		
		u8 tile = CurrentMapDataState[MAP_XY_TO_TILE(_bombHsTile.DestinationMetaX, _bombHsTile.DestinationMetaY)];
		if (CurrentPlayer->MetaX == _bombHsTile.DestinationMetaX && CurrentPlayer->MetaY == _bombHsTile.DestinationMetaY)
		{
			PlayerKillCurrent();
		}
		else if (tile == TILE_TYPE_MAGNUS || tile == TILE_TYPE_QUESTOR)
		{
			PlayerKillOther();
		}
	}
}


u8 BombHUpdateLogic()
{
	for (u8 i = 0; i < _bombHsCount; i++)
	{
		if (_bombHs[i].X != -1)
		{
			if (CanMoveDown(_bombHs[i].X, _bombHs[i].Y, i))
			{
				return TRUE;
			}
			else
			{
				_bombHs[i].Active = MOVE_DIRECTION_NONE;
			}
		}
	}

	return FALSE;
}


void TriggerBombH(u16 x, u16 y)
{
	u16 index = FindId(x, y,_bombHs, _bombHsCount);

	u16 left = x - 1;
	u16 right = x + 1;

	_bombHs[index].X = -1;
	_bombHs[index].Y = -1;
	_bombHs[index].Active = FALSE;

	PlaceExplosion(x, y);
	PlaceExplosion(left, y);
	PlaceExplosion(right, y);
}