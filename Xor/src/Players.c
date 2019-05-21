#include <genesis.h>

#include "Players.h"
#include "Map.h"
#include "../res/sprite.h"
#include "TileDefinitions.h"
#include "Xor.h"
#include "FishLogic.h"
#include "ChickenLogic.h"

// externs
u8 PlayerState = PLAYER_STATE_WAITING;

Player Players[2];
Player* CurrentPlayer;
Player* OtherPlayer;

s16 StepsTaken;
s16 MasksCollected;

s16 TotalMasks;
s16 MapsCollected;


// consts
const static s16 _drawWidth = 288; // draw width is different to the screen width as we have the window on the side
const static fix32 _squareSize = FIX32(24);



// Local Variables
static s16 _cameraX;
static s16 _cameraY;

static fix32 _xMoveLeft;
static fix32 _yMoveLeft;

static s16 _midX;
static s16 _midY;


void PlayersSetup()
{
	_midX = _drawWidth / 2;
	_midY = screenHeight / 2;

	// How long in seconds it takes to move a square
	fix32 timeToMove = FIX32(0.100);

	// Movement timing for NTSC
	fix32 divider = fix32Mul(timeToMove, FIX32(60));
	
	if (IS_PALSYSTEM)
	{
		// Movement timing for PAL
		divider = fix32Mul(timeToMove, FIX32(50));
	}
	
	// how many pixels to move per frame (based on PAL or NTSC)
	MovementPerFrame = fix32Div(_squareSize, divider);
	

	// work out how many masks the player needs to collect and where the 2 sheilds are located
	MapsCollected = 0;
	TotalMasks = 0;
	if (CurrentMap != 0)
	{
		int i = 0;
		for (int y = 0; y < 32; y++)
		{
			for (int x = 0; x < 32; x++)
			{
				if (Tiles[CurrentMap->MapData[i]]->TileType == TILE_TYPE_MAGNUS)
				{
					Players[0].MetaX = x;
					Players[0].MetaY = y;
				}
				else if (Tiles[CurrentMap->MapData[i]]->TileType == TILE_TYPE_QUESTOR)
				{
					Players[1].MetaX = x;
					Players[1].MetaY = y;
				}
				else if (Tiles[CurrentMap->MapData[i]]->TileType == TILE_TYPE_XOR)
				{
					TotalMasks++;
				}

				i++;
			}
		}
	}

	// set up the shield players
	Players[0].Id = 0;
	Players[0].TileId = TILE_TYPE_MAGNUS;
	Players[0].SpriteFrame = 0;
	Players[0].Alive = PLAYER_ALIVE_YES;

	Players[1].Id = 1;
	Players[1].TileId = TILE_TYPE_QUESTOR;
	Players[1].SpriteFrame = 1;
	Players[1].Alive = PLAYER_ALIVE_YES;

	CurrentPlayer = &Players[0];
	OtherPlayer = &Players[1];
	// Reset that tile to floor
	CurrentMapDataState[MAP_XY_TO_TILE(CurrentPlayer->MetaX, CurrentPlayer->MetaY)] = TILE_TYPE_FLOOR;

	StepsTaken = 0;
	MasksCollected = 0;	

	PlayerUpdateMove(TRUE);
	PlayerState = PLAYER_STATE_WAITING;
}



void PlayerStartMove()
{
	PlayerState = PLAYER_STATE_MOVING;	
	StepsTaken++;
}


void PlayerUpdateMove(u8 justSet)
{	
	u8 done = FALSE;

	u8 wasNegative = _xMoveLeft < 0 || _yMoveLeft < 0;

	s16 playerScreenX = (CurrentPlayer->MetaX * 24);
	s16 playerScreenY = (CurrentPlayer->MetaY * 24);

	if (justSet)
	{
		done = TRUE;
	}
	else
	{
		
		if (wasNegative)
		{
			if (_xMoveLeft != 0)
			{
				_xMoveLeft = fix32Add(_xMoveLeft, MovementPerFrame);
			}
			else if (_yMoveLeft != 0)
			{
				_yMoveLeft = fix32Add(_yMoveLeft, MovementPerFrame);
			}
		}
		else
		{
			if (_xMoveLeft != 0)
			{
				_xMoveLeft = fix32Sub(_xMoveLeft, MovementPerFrame);
			}
			else if (_yMoveLeft != 0)
			{
				_yMoveLeft = fix32Sub(_yMoveLeft, MovementPerFrame);
			}
		}

		u8 isNegative = _xMoveLeft < 0 || _yMoveLeft < 0;

		if (wasNegative == isNegative)
		{
			playerScreenX += fix32ToInt(_xMoveLeft);
			playerScreenY += fix32ToInt(_yMoveLeft);
			//KLog_U2(" playerScreenX ", playerScreenX, " playerScreenY ", playerScreenY);

		}
		else
		{
			done = TRUE;
		}
	}





	s16 left = playerScreenX - _midX;
	s16 right = playerScreenX + _midX;


	if (left < 0)
	{
		CameraXOffset = 0;
		_cameraX = 0;
	}
	else if (right > 768)
	{
		_cameraX = 768 - _drawWidth;		
		CameraXOffset = _drawWidth - 768;
		//playerScreenX += (_screenWidth - 768);
	}
	else
	{
		_cameraX = left;
		CameraXOffset = -_cameraX;
		//playerScreenX -= _cameraX;
	}

	playerScreenX += CameraXOffset;


	s16 top = playerScreenY - _midY;
	s16 bottom = playerScreenY + _midY;
	if (top < 0)
	{
		CameraYOffset = 0;
		_cameraY = 0;
		CurrentPlayer->ScreenMetaY = 0;
	}
	else if (bottom > 768)
	{
		_cameraY = 768 - screenHeight;
		CameraYOffset = (screenHeight - 768);
		//playerScreenY += (screenHeight - 768);
	}
	else
	{
		_cameraY = top;
		CameraYOffset = -_cameraY;
		//playerScreenY -= _cameraY;
	}

	playerScreenY += CameraYOffset;

	SetMapCamera(_cameraX, _cameraY);

	CurrentPlayer->ScreenMetaX = _cameraX / 24;
	CurrentPlayer->ScreenMetaY = _cameraY / 24;
	CurrentPlayer->ScreenX = playerScreenX;
	CurrentPlayer->ScreenY = playerScreenY;

	if (done)
	{
		PlayerState = PLAYER_STATE_FINISHED;
	}

	//KLog_U2(" playerScreenX ", playerScreenX, " playerScreenY ", playerScreenY);
}



void PlayerEndMove()
{	
	u8 tile = CurrentMapDataState[MAP_XY_TO_TILE(CurrentPlayer->MetaX, CurrentPlayer->MetaY)];
	

	switch (tile)
	{
		case TILE_TYPE_FORCEFIELD_V:
		case TILE_TYPE_FORCEFIELD_H:
			UpdateTile(CurrentPlayer->MetaX, CurrentPlayer->MetaY, TILE_TYPE_FLOOR); // change to floor
			PlayerState = PLAYER_STATE_TILE_LOGIC;
			break;

		case TILE_TYPE_MAP:
			UpdateTile(CurrentPlayer->MetaX, CurrentPlayer->MetaY, TILE_TYPE_FLOOR); // change to floor
			MapsCollected++;
			PlayerState = PLAYER_STATE_TILE_LOGIC;
			break;

		case TILE_TYPE_XOR:
			UpdateTile(CurrentPlayer->MetaX, CurrentPlayer->MetaY, TILE_TYPE_FLOOR); // change to floor
			MasksCollected++;			
			PlayerState = PLAYER_STATE_TILE_LOGIC;
			break;

		case TILE_TYPE_EXIT:
			PlayerState = PLAYER_STATE_COMPLETE;
			break;

		default:
			PlayerState = PLAYER_STATE_TILE_LOGIC;
			break;
	}	
}


void PlayerMoveDown()
{
	u16 y = CurrentPlayer->MetaY + 1;
	u8 tile = CurrentMapDataState[MAP_XY_TO_TILE(CurrentPlayer->MetaX, y)];
	u8 canMove = FALSE;

	switch (tile)
	{
		case TILE_TYPE_FLOOR:
		case TILE_TYPE_FORCEFIELD_V:
		case TILE_TYPE_MAP:
		case TILE_TYPE_XOR:		
			canMove = TRUE;
			break;

		case TILE_TYPE_EXIT:
			if (MasksCollected == TotalMasks)
			{
				canMove = TRUE;
			}
			break;

		case TILE_TYPE_CHICKEN:
			if (ChickenPushDown(CurrentPlayer->MetaX, y))
			{
				canMove = TRUE;
			}
			break;
	}

	if (canMove)
	{
		CurrentPlayer->MetaY = y;
		_yMoveLeft = -_squareSize;
		_xMoveLeft = 0;
		PlayerStartMove();
	}
}


void PlayerMoveUp()
{
	u16 y = CurrentPlayer->MetaY - 1;
	u8 tile = CurrentMapDataState[MAP_XY_TO_TILE(CurrentPlayer->MetaX, y)];
	u8 canMove = FALSE;

	switch (tile)
	{
		case TILE_TYPE_FLOOR:
		case TILE_TYPE_FORCEFIELD_V:
		case TILE_TYPE_MAP:
		case TILE_TYPE_XOR:		
			canMove = TRUE;
			break;

		case TILE_TYPE_EXIT:
			if (MasksCollected == TotalMasks)
			{
				canMove = TRUE;
			}
			break;

		case TILE_TYPE_CHICKEN:
			if (ChickenPushUp(CurrentPlayer->MetaX, y))
			{
				canMove = TRUE;
			}
			break;
	}


	if (canMove)
	{
		_yMoveLeft = _squareSize;
		_xMoveLeft = 0;
		CurrentPlayer->MetaY = y;
		PlayerStartMove();
	}
}


void PlayerMoveLeft()
{
	u16 x = CurrentPlayer->MetaX - 1;
	u8 tile = CurrentMapDataState[MAP_XY_TO_TILE(x, CurrentPlayer->MetaY)];
	u8 canMove = FALSE;

	switch (tile)
	{
		case TILE_TYPE_FLOOR:
		case TILE_TYPE_FORCEFIELD_H:
		case TILE_TYPE_MAP:
		case TILE_TYPE_XOR:
			canMove = TRUE;
			break;

		case TILE_TYPE_EXIT:
			if (MasksCollected == TotalMasks)
			{
				canMove = TRUE;
			}
			break;

		case TILE_TYPE_FISH:
			if (FishPushLeft(x, CurrentPlayer->MetaY))
			{
				canMove = TRUE;
			}
			break;
	}

	if (canMove)
	{
		_xMoveLeft = _squareSize;
		_yMoveLeft = 0;
		CurrentPlayer->MetaX = x;
		PlayerStartMove();
	}
}


void PlayerMoveRight()
{
	u16 x = CurrentPlayer->MetaX + 1;
	u8 tile = CurrentMapDataState[MAP_XY_TO_TILE(x, CurrentPlayer->MetaY)];
	u8 canMove = FALSE;

	switch (tile)
	{
		case TILE_TYPE_FLOOR:
		case TILE_TYPE_FORCEFIELD_H:
		case TILE_TYPE_MAP:
		case TILE_TYPE_XOR:		
			canMove = TRUE;
			break;

		case TILE_TYPE_EXIT:
			if (MasksCollected == TotalMasks)
			{
				canMove = TRUE;
			}
			break;

		case TILE_TYPE_FISH:
			if (FishPushRight(x, CurrentPlayer->MetaY))
			{
				canMove = TRUE;
			}
			break;
	}

	if (canMove)
	{
		_xMoveLeft = -_squareSize;
		_yMoveLeft = 0;
		CurrentPlayer->MetaX = x;
		PlayerStartMove();
	}
}


void PlayerChange()
{
	StepsTaken++;
	u8 nextId = (CurrentPlayer->Id == 0) ? 1 : 0;

	if (Players[nextId].Alive)
	{
		if (CurrentPlayer->Alive)
		{
			CurrentMapDataState[MAP_XY_TO_TILE(CurrentPlayer->MetaX, CurrentPlayer->MetaY)] = CurrentPlayer->TileId;
		}
		else
		{
			CurrentMapDataState[MAP_XY_TO_TILE(CurrentPlayer->MetaX, CurrentPlayer->MetaY)] = TILE_TYPE_FLOOR;
		}

		OtherPlayer = CurrentPlayer;
		CurrentPlayer = &Players[nextId];
		CurrentMapDataState[MAP_XY_TO_TILE(CurrentPlayer->MetaX, CurrentPlayer->MetaY)] = TILE_TYPE_FLOOR;
	}
	else
	{
		if (!CurrentPlayer->Alive)
		{
			PlayerState = PLAYER_STATE_GAMEOVER;
		}
	}
}


void PlayerKillCurrent()
{
	CurrentPlayer->Alive = PLAYER_ALIVE_REPORT_NO;
	KLog_U1("PlayerKillCurrent ", CurrentPlayer->Id);
}


void PlayerKillOther()
{
	OtherPlayer->Alive = PLAYER_ALIVE_REPORT_NO;
	KLog_U1("PlayerKillOther ", OtherPlayer->Id);

	CurrentMapDataState[MAP_XY_TO_TILE(OtherPlayer->MetaX, OtherPlayer->MetaY)] = TILE_TYPE_FLOOR;
	UpdateTile(OtherPlayer->MetaX, OtherPlayer->MetaY, TILE_TYPE_FLOOR);
}



u8 BothDead()
{
	return Players[0].Alive != PLAYER_ALIVE_YES && Players[1].Alive != PLAYER_ALIVE_YES;
}

