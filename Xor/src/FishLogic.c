#include <genesis.h>
#include "FishLogic.h"
#include "Map.h"
#include "GameMaps.h"
#include "TileDefinitions.h"
#include "Xor.h"
#include "Players.h"

#include "BombH.h"
#include "BombV.h"
#include "Explosion.h"
#include "CommonLogic.h"

#include "TileLoading.h"

static ActiveTileItem _fishTile;

#define FISH_MAX 100
static ActivePoint _fishes[FISH_MAX];
static u8 _fishCount;


void FishMakeSprite()
{
	_fishTile.ActiveSprite = SPR_addSprite(LoadedTiles[TILE_TYPE_FISH].GraphicsDefinition->Sprite, 0, 0, TILE_ATTR(LoadedTiles[TILE_TYPE_FISH].GraphicsDefinition->Palette, TRUE, FALSE, FALSE));
	SPR_setVisibility(_fishTile.ActiveSprite, HIDDEN);
}

void FishSetup()
{

	_fishTile.IsActive = FALSE;
	SPR_setVisibility(_fishTile.ActiveSprite, HIDDEN);



	for (u8 i = 0; i < FISH_MAX; i++)
	{
		_fishes[i].X = -1;
		_fishes[i].Y = -1;
		_fishes[i].Active = FALSE;
	}


	_fishCount = 0;
	for (s8 y = 0; y < MAP_HEIGHT; y++)
	{
		for (s8 x = 31; x >= 0; x--)
		{
			if (CurrentMapDataState[MAP_XY_TO_TILE(x, y)] == TILE_TYPE_FISH)
			{
				_fishes[_fishCount].X = x;
				_fishes[_fishCount].Y = y;
				_fishCount++;
			}
		}
	}

}



u8 FishPushLeft(u16 x, u16 y)
{
	u16 xCheck = x - 1;

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
		DoMovementCommon(x, y, xCheck, y, MOVE_DIRECTION_LEFT, FindId(x, y, _fishes, _fishCount), &_fishTile, _fishes);
	}

	return canMove;
}




u8 FishPushRight(u16 x, u16 y)
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
		DoMovementCommon(x, y, xCheck, y, MOVE_DIRECTION_RIGHT, FindId(x, y, _fishes, _fishCount), &_fishTile, _fishes);
	}

	return canMove;
}





static u8 CanMoveDown(u8 x, u8 y, u8 trackedId)
{
	u16 yCheck = y + 1;
	
	u8 tile = CurrentMapDataState[MAP_XY_TO_TILE(x, yCheck)];
	u8 canMove = FALSE;
	u8 canKill = _fishes[trackedId].Active == MOVE_DIRECTION_DOWN;


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
				CurrentMapDataState[MAP_XY_TO_TILE(x, y)] = TILE_TYPE_FLOOR;
				if (tile == TILE_TYPE_BOMB_H)
				{
					TriggerBombH(x, yCheck);
				}
				else if (tile == TILE_TYPE_BOMB_V)
				{
					TriggerBombV(x, yCheck);
				}

				_fishes[trackedId].X = -1;
				_fishes[trackedId].Y = -1;
				_fishes[trackedId].Active = FALSE;
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
		DoMovementCommon(x, y, x, yCheck, MOVE_DIRECTION_DOWN, FindId(x, y, _fishes, _fishCount), &_fishTile, _fishes);
	}

	return canMove;
}

u8 FishUpdateMovement()
{
	return UpdateMovementCommon(&_fishTile);
}

void FishFinishMovement()
{
	if (_fishTile.IsActive)
	{
		_fishes[_fishTile.ActiveIndex].X = _fishTile.DestinationMetaX;
		_fishes[_fishTile.ActiveIndex].Y = _fishTile.DestinationMetaY;


		u8 tile = CurrentMapDataState[MAP_XY_TO_TILE(_fishTile.DestinationMetaX, _fishTile.DestinationMetaY)];


		_fishTile.IsActive = FALSE;

		if (_fishTile.OnScreen)
		{
			UpdateTile(_fishTile.DestinationMetaX, _fishTile.DestinationMetaY, TILE_TYPE_FISH);
			SPR_setVisibility(_fishTile.ActiveSprite, HIDDEN);

		}
		else
		{
			CurrentMapDataState[MAP_XY_TO_TILE(_fishTile.DestinationMetaX, _fishTile.DestinationMetaY)] = TILE_TYPE_FISH;
		}

		if (CurrentPlayer->MetaX == _fishTile.DestinationMetaX && CurrentPlayer->MetaY == _fishTile.DestinationMetaY)
		{
			PlayerKillCurrent();
		}
		else if (tile == TILE_TYPE_MAGNUS || tile == TILE_TYPE_QUESTOR)
		{
			PlayerKillOther();
		}
	}
}

void KillFish(u16 x, u16 y)
{
	s8 id = FindId(x, y, _fishes, _fishCount);
	
	if (id != -1)
	{
		_fishes[(u8)id].X = -1;
		_fishes[(u8)id].Y = -1;
		_fishes[(u8)id].Active = FALSE;
	}
	else
	{
		KLog("All fishes");
		for (int i = 0; i < FISH_MAX; i++)
		{
			KLog_U2("  fish x : ", _fishes[i].X, " y: ", _fishes[i].Y);
		}
	}
}

u8 FishUpdateLogic()
{
	for (u8 i = 0; i < _fishCount; i++)
	{
		if (_fishes[i].X != -1)
		{
			if (CanMoveDown(_fishes[i].X, _fishes[i].Y, i))
			{
				return TRUE;
			}
			else
			{
				_fishes[i].Active = MOVE_DIRECTION_NONE;
			}
		}
	}

	return FALSE;
}
