#include <genesis.h>
#include "FishLogic.h"
#include "Map.h"
#include "GameMaps.h"
#include "TileDefinitions.h"
#include "Xor.h"
#include "Players.h"
#include "../res/sprite.h"

static ActiveTileItem _fishTile;

#define FISH_MAX 100
static ActivePoint _fishes[FISH_MAX];
static u8 _fishCount;


void FishSetup()
{
	_fishTile.ActiveSprite = SPR_addSprite(&ShieldSprites, 0, 0, TILE_ATTR(PAL2, TRUE, FALSE, FALSE));
	SPR_setVisibility(_fishTile.ActiveSprite, HIDDEN);
	SPR_setFrame(_fishTile.ActiveSprite, 2);
	_fishTile.IsActive = FALSE;



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







static void DoMovement(u8 x, u8 y, u8 newX, u8 newY, u8 direction, u8 trackedId)
{
	_fishTile.CurrentMetaX = x;
	_fishTile.CurrentMetaY = y;

	_fishTile.DestinationMetaX = newX;
	_fishTile.DestinationMetaY = newY;


	_fishTile.DestinationScreenX = newX * 24;
	_fishTile.DestinationScreenY = newY * 24;


	_fishTile.MoveRemaining = fix32Add(MovementPerFrame, FIX32(24));
	_fishTile.MoveDirection = direction;

	_fishTile.IsActive = TRUE;

	CurrentMapDataState[MAP_XY_TO_TILE(x, y)] = TILE_TYPE_FLOOR;
	FishUpdateMovement();

	_fishes[trackedId].Active = TRUE;

	UpdateTile(_fishTile.CurrentMetaX, _fishTile.CurrentMetaY, TILE_TYPE_FLOOR);


	for (u8 i = 0; i < _fishCount; i++)
	{
		if (_fishes[i].X == x && _fishes[i].Y == y)
		{
			_fishTile.ActiveIndex = i;
			break;
		}
	}
}



static u8 FindId(u16 x, u16 y)
{
	for (u8 i = 0; i < _fishCount; i++)
	{
		if (_fishes[i].X == -x && _fishes[i].Y == y)
		{
			return i;
		}
	}

	return 0;
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


		default:
			break;
	}



	if (canMove)
	{
		DoMovement(x, y, xCheck, y, MOVE_DIRECTION_LEFT, FindId(x,y));
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


		default:
			break;
	}



	if (canMove)
	{
		DoMovement(x, y, xCheck, y, MOVE_DIRECTION_RIGHT, FindId(x, y));
	}

	return canMove;
}





static u8 CanMoveDown(u8 x, u8 y, u8 trackedId)
{
	u16 yCheck = y + 1;

	
	u8 tile = CurrentMapDataState[MAP_XY_TO_TILE(x, yCheck)];

	u8 canMove = FALSE;

	u8 canKill = _fishes[trackedId].Active;



	switch (tile)
	{
		case TILE_TYPE_FORCEFIELD_V:
		case TILE_TYPE_FLOOR:
			canMove = TRUE;


		default:
			break;
	}


	if (CurrentPlayer->MetaX == x && CurrentPlayer->MetaY == yCheck && !canKill)
	{
		canMove = FALSE;
	}


	if (canMove)
	{
		DoMovement(x, y, x, yCheck, MOVE_DIRECTION_DOWN, trackedId);
	}

	return canMove;
}

u8 FishUpdateMovement()
{
	u8 done = TRUE;

	if (_fishTile.IsActive)
	{	
		_fishTile.MoveRemaining = fix32Sub(_fishTile.MoveRemaining, MovementPerFrame);

		if (fix32ToInt(_fishTile.MoveRemaining) < 0)
		{
			_fishTile.MoveRemaining = 0;
		}
		else
		{
			done = FALSE;
		}

		u16 x = 0;
		u16 y = 0;
		switch (_fishTile.MoveDirection)
		{

			case MOVE_DIRECTION_LEFT:
			{
				y = _fishTile.DestinationScreenY;
				x = _fishTile.DestinationScreenX + fix32ToInt(_fishTile.MoveRemaining);
				break;
			}

			case MOVE_DIRECTION_RIGHT:
			{
				y = _fishTile.DestinationScreenY;
				x = _fishTile.DestinationScreenX - fix32ToInt(_fishTile.MoveRemaining);
				break;
			}

			case MOVE_DIRECTION_DOWN:
			{
				y = _fishTile.DestinationScreenY - fix32ToInt(_fishTile.MoveRemaining);
				x = _fishTile.DestinationScreenX;
				break;
			}

			default:
				break;
		}

		//KLog_U3("fish direction ",_fishTile.MoveDirection, " fish x ", x, " fish y ", y);
		//KLog_F1("MoveRemaining ", _fishTile.MoveRemaining);


		x += CameraXOffset;
		y += CameraYOffset;

		//KLog_U2("Offsets fish x ", x, " fish y ", y);

		SPR_setPosition(_fishTile.ActiveSprite, x, y);
		SPR_setVisibility(_fishTile.ActiveSprite, VISIBLE);		
		SPR_update();
	}

	return done;
}

void FishFinishMovement()
{
	if (_fishTile.IsActive)
	{
		_fishes[_fishTile.ActiveIndex].X = _fishTile.DestinationMetaX;
		_fishes[_fishTile.ActiveIndex].Y = _fishTile.DestinationMetaY;


		u8 tile = CurrentMapDataState[MAP_XY_TO_TILE(_fishTile.DestinationMetaX, _fishTile.DestinationMetaY)];


		_fishTile.IsActive = FALSE;
		CurrentMapDataState[MAP_XY_TO_TILE(_fishTile.DestinationMetaX, _fishTile.DestinationMetaY)] = TILE_TYPE_FISH;
		UpdateTile(_fishTile.DestinationMetaX, _fishTile.DestinationMetaY, TILE_TYPE_FISH);
		SPR_setVisibility(_fishTile.ActiveSprite, HIDDEN);		

		if (CurrentPlayer->MetaX == _fishTile.DestinationMetaX && CurrentPlayer->MetaY == _fishTile.DestinationMetaY)
		{
			KLog("Fish Killing Current Player");
			PlayerKillCurrent();
		}
		else if (tile == TILE_TYPE_MAGNUS || tile == TILE_TYPE_QUESTOR)
		{
			KLog("Fish Killing Other Player");
			PlayerKillOther();
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
				_fishes[i].Active = FALSE;
			}
		}
	}

	return FALSE;
}
