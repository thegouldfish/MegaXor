#include <genesis.h>
#include "ChickenLogic.h"
#include "Map.h"
#include "GameMaps.h"
#include "TileDefinitions.h"
#include "Xor.h"
#include "Players.h"

#include "../res/sprite.h"


static ActiveTileItem _chickenTile;


#define CHICKEN_MAX 100
static ActivePoint _chickens[CHICKEN_MAX];
static u8 _chickenCount;

void ChickenSetup()
{
	
	_chickenTile.ActiveSprite = SPR_addSprite(&ShieldSprites, 0, 0, TILE_ATTR(PAL2, TRUE, FALSE, FALSE));
	SPR_setVisibility(_chickenTile.ActiveSprite, HIDDEN);
	SPR_setFrame(_chickenTile.ActiveSprite, 3);
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



static void DoMovement(u8 x, u8 y, u8 newX, u8 newY, u8 direction, u8 trackedId)
{
	_chickenTile.CurrentMetaX = x;
	_chickenTile.CurrentMetaY = y;

	_chickenTile.DestinationMetaX = newX;
	_chickenTile.DestinationMetaY = newY;


	_chickenTile.DestinationScreenX = newX * 24;
	_chickenTile.DestinationScreenY = newY * 24;


	_chickenTile.MoveRemaining = fix32Add(MovementPerFrame, FIX32(24));
	_chickenTile.MoveDirection = direction;

	_chickenTile.IsActive = TRUE;

	CurrentMapDataState[MAP_XY_TO_TILE(x, y)] = TILE_TYPE_FLOOR;
	ChickenUpdateMovement();

	_chickens[trackedId].Active = TRUE;

	UpdateTile(_chickenTile.CurrentMetaX, _chickenTile.CurrentMetaY, TILE_TYPE_FLOOR);

	for (u8 i = 0; i < _chickenCount; i++)
	{
		if (_chickens[i].X == x && _chickens[i].Y == y)
		{
			_chickenTile.ActiveIndex = i;
			break;
		}
	}
}


static u8 FindId(u16 x, u16 y)
{
	for (u8 i = 0; i < _chickenCount; i++)
	{
		if (_chickens[i].X == -x && _chickens[i].Y == y)
		{
			return i;
		}
	}

	return 0;
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


		default:
			break;
	}



	if (canMove)
	{
		DoMovement(x, y, x, yCheck, MOVE_DIRECTION_UP, FindId(x,y));
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


	default:
		break;
	}



	if (canMove)
	{
		DoMovement(x, y, x, yCheck, MOVE_DIRECTION_DOWN, FindId(x,y));
	}

	return canMove;
}





static u8 CanMoveLeft(u8 x, u8 y, u8 trackedId)
{
	u16 xCheck = x - 1;


	u8 tile = CurrentMapDataState[MAP_XY_TO_TILE(xCheck, y)];

	u8 canMove = FALSE;


	u8 canKill = _chickens[trackedId].Active;



	switch (tile)
	{
		case TILE_TYPE_FORCEFIELD_H:
		case TILE_TYPE_FLOOR:
			canMove = TRUE;


		default:
			break;
	}


	if (CurrentPlayer->MetaX == xCheck && CurrentPlayer->MetaY == y && !canKill)
	{
		canMove = FALSE;
	}


	if (canMove)
	{
		DoMovement(x, y, xCheck, y, MOVE_DIRECTION_LEFT, trackedId);
	}

	return canMove;
}



u8 ChickenUpdateMovement()
{
	u8 done = TRUE;

	if (_chickenTile.IsActive)
	{
		_chickenTile.MoveRemaining = fix32Sub(_chickenTile.MoveRemaining, MovementPerFrame);

		if (fix32ToInt(_chickenTile.MoveRemaining) < 0)
		{
			_chickenTile.MoveRemaining = 0;
		}
		else
		{
			done = FALSE;
		}

		u16 x = 0;
		u16 y = 0;
		switch (_chickenTile.MoveDirection)
		{
			case MOVE_DIRECTION_LEFT:
			{
				y = _chickenTile.DestinationScreenY;
				x = _chickenTile.DestinationScreenX + fix32ToInt(_chickenTile.MoveRemaining);
				break;
			}

			case MOVE_DIRECTION_RIGHT:
			{
				y = _chickenTile.DestinationScreenY;
				x = _chickenTile.DestinationScreenX - fix32ToInt(_chickenTile.MoveRemaining);
				break;
			}

			case MOVE_DIRECTION_UP:
			{
				y = _chickenTile.DestinationScreenY + fix32ToInt(_chickenTile.MoveRemaining);
				x = _chickenTile.DestinationScreenX;
				break;
			}

			case MOVE_DIRECTION_DOWN:
			{
				y = _chickenTile.DestinationScreenY - fix32ToInt(_chickenTile.MoveRemaining);
				x = _chickenTile.DestinationScreenX;
				break;
			}

			default:
				break;
		}

		x += CameraXOffset;
		y += CameraYOffset;

		SPR_setPosition(_chickenTile.ActiveSprite, x, y);
		SPR_setVisibility(_chickenTile.ActiveSprite, VISIBLE);
		SPR_update();
	}

	return done;
}

void ChickenFinishMovement()
{
	if (_chickenTile.IsActive)
	{		
		_chickens[_chickenTile.ActiveIndex].X = _chickenTile.DestinationMetaX;
		_chickens[_chickenTile.ActiveIndex].Y = _chickenTile.DestinationMetaY;

		_chickenTile.IsActive = FALSE;
		CurrentMapDataState[MAP_XY_TO_TILE(_chickenTile.DestinationMetaX, _chickenTile.DestinationMetaY)] = TILE_TYPE_CHICKEN;
		UpdateTile(_chickenTile.DestinationMetaX, _chickenTile.DestinationMetaY, TILE_TYPE_CHICKEN);
		SPR_setVisibility(_chickenTile.ActiveSprite, HIDDEN);


		u8 tile = CurrentMapDataState[MAP_XY_TO_TILE(_chickenTile.DestinationMetaX, _chickenTile.DestinationMetaY)];
		if (CurrentPlayer->MetaX == _chickenTile.DestinationMetaX && CurrentPlayer->MetaY == _chickenTile.DestinationMetaY)
		{
			KLog("Chicken Killing Current Player");
			PlayerKillCurrent();
		}
		else if (tile == TILE_TYPE_MAGNUS || tile == TILE_TYPE_QUESTOR)
		{
			KLog("Chicken Killing Other Player");
			PlayerKillOther();
		}
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
				_chickens[i].Active = FALSE;
			}
		}
	}

	return FALSE;
}
