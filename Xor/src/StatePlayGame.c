#include <genesis.h>

#include "Xor.h"

#include "Map.h"
#include "StatePlayGame.h"
#include "Globals.h"
#include "PadHelper.h"
#include "TileDefinitions.h"

#include "../res/sprite.h"
#include "../res/gfx.h"

#include "Players.h"
#include "FishLogic.h"
#include "ChickenLogic.h"

#include "Globals.h"
#include "StateSelectLevel.h"


#define MOVED_NONE 0
#define MOVED_VERTICALLY 1
#define MOVED_HORIZONTALLY 2



static Sprite* _player;

static Sprite* _masksCollected[2];
static Sprite* _masksTotal[2];
static Sprite* _moves[4];
static Sprite* _playerUI;

static Sprite* _whoops;

static u8 _previousMapCount = 0;
static u16 MapVramIndex = 0;


static u8 _replayMovements;
static char _recordedMovements[MAX_MOVE_COUNT];
static u16 _currentMoveCount;

static u8 _moveType;


static u8 _playerDead;

void SetNumbers(int number, Sprite* sprites[], const int size)
{
	int numbs[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int i = size - 1;
	while (number > 0)
	{
		if (number < 10)
		{
			numbs[i] = number;
		}
		else
		{
			numbs[i] = (number % 10);
		}

		number /= 10;
		i--;
	}



	for (int i = 0; i < 10 && i < size; i++)
	{
		SPR_setFrame(sprites[i], numbs[i]);
	}
}



static void GenerateTiles2()
{
	u16 yStart = 0;
	u16 xStart = 0;

	u32 tileSet[8];

	u16 tileOffset = 0;
	MapVramIndex = VramTileIndex;

	for (u16 tt = 0; tt < 4; tt++)
	{
		for (u16 ss = 0; ss < 4; ss++)
		{
			xStart = ss << 3;
			yStart = tt << 3;
			memset(tileSet, 0, 8 * 4);

			for (u16 y = yStart; y < yStart + 8; y++)
			{
				for (u16 x = xStart; x < xStart + 8; x++)
				{
					u16 i = (y << 5) + x;

					u16 bitshiftOffset = (7 - (x - xStart)) << 2;

					u16 toSet = 15;

					if (CurrentMapDataState[i] == TILE_TYPE_WALL)
					{
						toSet = 7;
					}
					else if (CurrentMapDataState[i] == TILE_TYPE_XOR)
					{
						toSet = 1;
					}


					tileSet[y - yStart] = tileSet[y - yStart] | (toSet << bitshiftOffset);
				}
			}

			VDP_loadTileData((const u32 *)tileSet, VramTileIndex + tileOffset, 1, DMA);
			tileOffset++;
		}
	}
}


void StatePlayGame_Start()
{
	SYS_disableInts();
	SPR_reset();
	VDP_resetScreen();


	VDP_setPaletteColors(0, (u16*)palette_black, 64);


	InitTileSets();
	LoadMap(SelectedLevel);
	
	
	
	_player = SPR_addSprite(&ShieldSprites, 0, 0, TILE_ATTR(PAL2, TRUE, FALSE, FALSE));
	SPR_setFrame(_player, 0);


	_playerUI = SPR_addSprite(&ShieldSprites, 292, 196, TILE_ATTR(PAL2, TRUE, FALSE, FALSE));
	SPR_setFrame(_playerUI, 0);


	_whoops = SPR_addSprite(&sp_whoops,98,98, TILE_ATTR(PAL3, TRUE, FALSE, FALSE));
	SPR_setVisibility(_whoops, HIDDEN);

	_currentMoveCount = 0;

	_masksCollected[0] = SPR_addSprite(&sp_numbers, 296, 68, TILE_ATTR(PAL3, TRUE, FALSE, FALSE));
	_masksCollected[1] = SPR_addSprite(&sp_numbers, 304, 68, TILE_ATTR(PAL3, TRUE, FALSE, FALSE));

	
	_masksTotal[0] = SPR_addSprite(&sp_numbers, 296, 130, TILE_ATTR(PAL3, TRUE, FALSE, FALSE));
	_masksTotal[1] = SPR_addSprite(&sp_numbers, 304, 130, TILE_ATTR(PAL3, TRUE, FALSE, FALSE));


	_moves[0] = SPR_addSprite(&sp_numbers, 288, 172, TILE_ATTR(PAL3, TRUE, FALSE, FALSE));
	_moves[1] = SPR_addSprite(&sp_numbers, 296, 172, TILE_ATTR(PAL3, TRUE, FALSE, FALSE));
	_moves[2] = SPR_addSprite(&sp_numbers, 304, 172, TILE_ATTR(PAL3, TRUE, FALSE, FALSE));
	_moves[3] = SPR_addSprite(&sp_numbers, 312, 172, TILE_ATTR(PAL3, TRUE, FALSE, FALSE));
	

	PlayersSetup();	
	FishSetup();
	ChickenSetup();
	
	VDP_waitVSync();
	RedrawScreen(CurrentPlayer->ScreenMetaX , CurrentPlayer->ScreenMetaY);
	
	PlayerState = PLAYER_STATE_WAITING;


	GenerateTiles2();
	SYS_enableInts();
	ActiveReset();

	UpdatePlans();
	SPR_setPosition(_player, CurrentPlayer->ScreenX, CurrentPlayer->ScreenY);


	SetNumbers(TotalMasks, _masksTotal, 2);
	SetNumbers(MasksCollected, _masksCollected, 2);
	SetNumbers(StepsTaken, _moves, 4);


	SPR_update();
	_replayMovements = FALSE;
	_currentMoveCount = 0;

	if (PlaySolution)
	{
		memcpy(_recordedMovements, GameMaps[SelectedLevel]->Solution, MAX_MOVE_COUNT);

		_replayMovements = TRUE;
	}


	_moveType = MOVED_NONE;

	VDP_fadeIn(0, (4 * 16) - 1, GameScreenPalette, 10, FALSE);
}

u8 _whoopsTimer = 120;

void StatePlayGame_Update()
{
	

	if (_previousMapCount != MapsCollected)
	{
		for (s16 i = 0; i < MapsCollected; i++)
		{
			int mapIndex = MapVramIndex;
			int x = 36;
			int y = 2;

			if (i == 1)
			{
				x = 38;
				mapIndex = MapVramIndex+2;
			}
			else if (i == 2)
			{
				y = 4;
				mapIndex = MapVramIndex + 8;
			}
			else if (i == 3)
			{
				x = 38;
				y = 4;
				mapIndex = MapVramIndex + 10;
			}

			VDP_setTileMapXY(PLAN_WINDOW, TILE_ATTR_FULL(PAL3, FALSE, FALSE, FALSE, mapIndex + 0), x, y);
			VDP_setTileMapXY(PLAN_WINDOW, TILE_ATTR_FULL(PAL3, FALSE, FALSE, FALSE, mapIndex + 1), x+1, y);
			VDP_setTileMapXY(PLAN_WINDOW, TILE_ATTR_FULL(PAL3, FALSE, FALSE, FALSE, mapIndex + 4), x, y+1);
			VDP_setTileMapXY(PLAN_WINDOW, TILE_ATTR_FULL(PAL3, FALSE, FALSE, FALSE, mapIndex + 5), x+1, y+1);			
		}

		_previousMapCount = MapsCollected;
	}
	
	//KLog_U1("Player State ", PlayerState);
	switch (PlayerState)
	{
		
		case PLAYER_STATE_WAITING:
		{	
			_playerDead = FALSE;
			_moveType = MOVED_NONE;
			char replay = 0;
			
			SPR_setVisibility(_whoops, HIDDEN);
			if (_replayMovements)
			{
				replay = _recordedMovements[_currentMoveCount];
			}

			if ((replay == 0 && (Pad1.Left == PAD_PRESSED || Pad1.Left == PAD_HELD)) || replay == 'L')
			{
				PlayerMoveLeft();
				_recordedMovements[_currentMoveCount] = 'L';
				_currentMoveCount++;
				_moveType = MOVED_HORIZONTALLY;
			}
			else if ((replay == 0 && (Pad1.Right == PAD_PRESSED || Pad1.Right == PAD_HELD)) || replay == 'R')
			{
				PlayerMoveRight();
				_recordedMovements[_currentMoveCount] = 'R';
				_currentMoveCount++;
				_moveType = MOVED_HORIZONTALLY;
			}

			if ((replay == 0 && (Pad1.Up == PAD_PRESSED || Pad1.Up == PAD_HELD)) || replay == 'U')
			{
				PlayerMoveUp();
				_recordedMovements[_currentMoveCount] = 'U';
				_currentMoveCount++;
				_moveType = MOVED_VERTICALLY;
			}
			else if ((replay == 0 && (Pad1.Down == PAD_PRESSED || Pad1.Down == PAD_HELD)) || replay == 'D')
			{
				PlayerMoveDown();
				_recordedMovements[_currentMoveCount] = 'D';
				_currentMoveCount++;
				_moveType = MOVED_VERTICALLY;
			}
			else if ((replay == 0 && (Pad1.A == PAD_RELEASED)) || replay == 'x')
			{
				_recordedMovements[_currentMoveCount] = 'x';
				_currentMoveCount++;

				VDP_fadeOut(0, 47, 5, FALSE);
				
				PlayerChange();
				PlayerUpdateMove(TRUE);
				RedrawScreen(CurrentPlayer->ScreenMetaX, CurrentPlayer->ScreenMetaY);
				
				SPR_setFrame(_player, CurrentPlayer->SpriteFrame);
				SPR_setFrame(_playerUI, CurrentPlayer->SpriteFrame);

				SPR_setPosition(_player, CurrentPlayer->ScreenX, CurrentPlayer->ScreenY);
				
				SPR_update();
				VDP_waitVSync();
				VDP_fadeIn(0, (3 * 16) - 1, GameScreenPalette, 5, FALSE);

			}
			break;
		}

		case PLAYER_STATE_MOVING:
		{
			PlayerUpdateMove(FALSE);
			FishUpdateMovement();
			ChickenUpdateMovement();		

			UpdatePlans();
			SPR_setPosition(_player, CurrentPlayer->ScreenX, CurrentPlayer->ScreenY);
			break;
		}


		case PLAYER_STATE_FINISHED:
		{
			PlayerEndMove();
			FishFinishMovement();
			ChickenFinishMovement();
			GenerateTiles2();
			SetNumbers(MasksCollected, _masksCollected, 2);
			SetNumbers(StepsTaken, _moves, 4);
			
			break;
		}

		case PLAYER_STATE_TILE_LOGIC:
		{			
			if (_moveType == MOVED_VERTICALLY)
			{
				if (ChickenUpdateLogic())
				{
					PlayerState = PLAYER_STATE_TILE_UPDATE;
				}
				else if (FishUpdateLogic()) 
				{
					_moveType = MOVED_NONE;
					PlayerState = PLAYER_STATE_TILE_UPDATE;
				}
				else
				{
					if (!_playerDead)
					{
						PlayerState = PLAYER_STATE_WAITING;
					}
					else
					{
						_whoopsTimer = 120;
						PlayerState = PLAYER_STATE_TILE_WHOOPS;
					}
				}				
			}
			else
			{
				if (FishUpdateLogic())
				{
					PlayerState = PLAYER_STATE_TILE_UPDATE;
				}
				else if (ChickenUpdateLogic())
				{
					PlayerState = PLAYER_STATE_TILE_UPDATE;
				}
				else
				{
					if (!_playerDead)
					{
						PlayerState = PLAYER_STATE_WAITING;
					}
					else
					{
						_whoopsTimer = 120;
						PlayerState = PLAYER_STATE_TILE_WHOOPS;
					}
				}
			}
			break;
		}

		case PLAYER_STATE_TILE_UPDATE:
		{
			u8 allDone = TRUE;

			allDone = allDone && FishUpdateMovement();
			allDone = allDone && ChickenUpdateMovement();

			
			if (allDone)
			{
				PlayerState = PLAYER_STATE_TILE_FINISHED;
			}
			break;
		}


		case PLAYER_STATE_TILE_FINISHED:
		{
			FishFinishMovement();
			ChickenFinishMovement();

			if (BothDead())
			{
				KLog("Both dead");
				KLog_U2("current alive: ", CurrentPlayer->Alive, " other player ", OtherPlayer->Alive);
				_whoopsTimer = 120;
				PlayerState = PLAYER_STATE_GAMEOVER;
				SPR_setVisibility(_player, HIDDEN);
				SPR_setFrame(_whoops, 1);
				SPR_setVisibility(_whoops, VISIBLE);
			}
			else if ((CurrentPlayer->Alive == PLAYER_ALIVE_REPORT_NO || OtherPlayer->Alive == PLAYER_ALIVE_REPORT_NO) && !_playerDead)
			{
				// Show Whoops
				SPR_setFrame(_whoops, 0);
				SPR_setVisibility(_whoops, VISIBLE);
				
				if (CurrentPlayer->Alive == PLAYER_ALIVE_REPORT_NO)
				{
					SPR_setVisibility(_player, HIDDEN);
				}

				_playerDead = TRUE;
			}
			else
			{
				PlayerState = PLAYER_STATE_TILE_LOGIC;
			}
			break;
		}



		case PLAYER_STATE_TILE_WHOOPS:
		{
			_whoopsTimer--;

			if (_whoopsTimer == 0)
			{
				if (CurrentPlayer->Alive == PLAYER_ALIVE_REPORT_NO)
				{		
					CurrentPlayer->Alive = PLAYER_ALIVE_NO;


					VDP_fadeOut(0, 47, 5, FALSE);
					SPR_setVisibility(_player, VISIBLE);

					PlayerChange();
					PlayerUpdateMove(TRUE);
					RedrawScreen(CurrentPlayer->ScreenMetaX, CurrentPlayer->ScreenMetaY);

					SPR_setFrame(_player, CurrentPlayer->SpriteFrame);
					SPR_setFrame(_playerUI, CurrentPlayer->SpriteFrame);

					SPR_setPosition(_player, CurrentPlayer->ScreenX, CurrentPlayer->ScreenY);

					SPR_update();
					VDP_waitVSync();
					VDP_fadeIn(0, (3 * 16) - 1, GameScreenPalette, 5, FALSE);
				}


				if (OtherPlayer->Alive == PLAYER_ALIVE_REPORT_NO)
				{
					OtherPlayer->Alive = PLAYER_ALIVE_NO;
				}

				_playerDead = FALSE;
				PlayerState = PLAYER_STATE_TILE_LOGIC;
			}
			break;
		}


		case PLAYER_STATE_COMPLETE:
		{
			MapMoveCounts[SelectedLevel] = StepsTaken;
			StateMachineChange(&GameMachineState, &StateLevelSelect);
			break;
		}

		case PLAYER_STATE_GAMEOVER:
		{			
			_whoopsTimer--;
			if (_whoopsTimer == 0)
			{
				StateMachineChange(&GameMachineState, &StateLevelSelect);
			}
			break;
		}
	}


	//KLog_U1("Ending state ", PlayerState);
}

void StatePlayGame_End()
{
	VDP_fadeOut(0, 47, 20, FALSE);
	VDP_clearPlan(PLAN_WINDOW, TRUE);

}



const SimpleState StatePlayGame =
{
	StatePlayGame_Start,
	StatePlayGame_Update,
	StatePlayGame_End,
};