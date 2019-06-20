#include <genesis.h>

#include "Xor.h"

#include "Map.h"
#include "StatePlayGame.h"
#include "PadHelper.h"
#include "TileDefinitions.h"

#include "../res/sprite.h"
#include "../res/gfx.h"

#include "Players.h"
#include "FishLogic.h"
#include "ChickenLogic.h"
#include "Explosion.h"
#include "BombH.h"
#include "BombV.h"
#include "DollLogic.h"
#include "ForceFields.h"
#include "UI.h"

#include "StateSelectLevel.h"


#define MOVED_NONE 0
#define MOVED_VERTICALLY 1
#define MOVED_HORIZONTALLY 2

#define FADE_COLOUR(x)  (((VDPPALETTE_REDMASK & x) / 2) | ((VDPPALETTE_BLUEMASK & x) / 2) | ((VDPPALETTE_GREENMASK & x) / 2 ))



static Sprite* _player;

static Sprite* _playerUI;

static char _recordedMovements[MAX_MOVE_COUNT];
static u16 _currentMoveCount;

static u8 _moveType;


static u8 _playerDead;


static u8 _whoopsTimer = 120;

static u8 _paused;

static u16 _fadedPallete[64];

void StatePlayGame_Start()
{
	SYS_disableInts();
	SPR_reset();
	VDP_resetScreen();


	VDP_setPaletteColors(0, (u16*)palette_black, 64);

	// Load basic tiles
	InitTileSets();	
	
	_player = SPR_addSprite(&ShieldSprites, 0, 0, TILE_ATTR(PAL2, FALSE, FALSE, FALSE));
	SPR_setFrame(_player, 0);


	_playerUI = SPR_addSprite(&ShieldSprites, 292, 196, TILE_ATTR(PAL2, FALSE, FALSE, FALSE));
	SPR_setFrame(_playerUI, 0);


	_currentMoveCount = 0;

	
	SYS_enableInts();

	LoadMap(SelectedLevel);
	
	PlayersSetup();	

	FishMakeSprite();
	FishSetup();

	ChickenMakeSprite();
	ChickenSetup();

	BombHMakeSprite();
	BombHSetup();

	BombVMakeSprite();
	BombVSetup();

	SetupUI();
	
	DollSetup();
	SetupForceFields();
	ExplosionSetup();
	
	VDP_waitVSync();
	RedrawScreen(CurrentPlayer->ScreenMetaX , CurrentPlayer->ScreenMetaY);
	
	PlayerState = PLAYER_STATE_WAITING;


	

	UpdatePlans();
	SPR_setPosition(_player, CurrentPlayer->ScreenX, CurrentPlayer->ScreenY);


	SPR_update();
	_moveType = MOVED_NONE;
	_currentMoveCount = 0;

	if (GameInteractionState == GAME_INTERACTION_STATE_TEST_REPLAY)
	{
		memcpy(_recordedMovements, GameMaps[SelectedLevel]->Solution, MAX_MOVE_COUNT);
	}


	


	_paused = FALSE;
	

	

	// Set colour zero to black as its nicer to see as a border colour
	GameScreenPalette[0] = 0;
	
	memcpy(_fadedPallete, GameScreenPalette, 64 * 2);

	
	for (int i = 0; i < 48; i++)
	{
		_fadedPallete[i] = FADE_COLOUR(_fadedPallete[i]);
	}
	

	KLog_U1("Free memory: ", MEM_getFree());
	VDP_fadeIn(0, (4 * 16) - 1, GameScreenPalette, 10, FALSE);
}



void ResetGame(bool startReplay)
{
	VDP_fadeOut(0, 63, 10, FALSE);

	LoadMap(SelectedLevel);
	DollSetup();
	SetupForceFields();
	ExplosionSetup();


	

	FishSetup();
	BombHSetup();
	BombVSetup();
	ChickenSetup();


	

	PlayersSetup();

	ResetUI();
	HideUIElements();
	RedrawScreen(CurrentPlayer->ScreenMetaX, CurrentPlayer->ScreenMetaY);
	UpdatePlans();
	SPR_setPosition(_player, CurrentPlayer->ScreenX, CurrentPlayer->ScreenY);
	




	_moveType = MOVED_NONE;
	_currentMoveCount = 0;
	if (startReplay)
	{
		GameInteractionState = GAME_INTERACTION_STATE_USER_REPLAY;
	}
	else
	{
		GameInteractionState = GAME_INTERACTION_STATE_NORMAL;
		memset(_recordedMovements, 0, MAX_MOVE_COUNT);		
	}

	SPR_update();
	VDP_waitVSync();
	
	VDP_fadeIn(0, (4 * 16) - 1, GameScreenPalette, 10, FALSE);
}




void UpdatePauseScreen(bool canReturn)
{
	if (Pad1.A == PAD_RELEASED)
	{
		if (GameInteractionState != GAME_INTERACTION_STATE_TEST_REPLAY)
		{
			GameInteractionState = GAME_INTERACTION_STATE_USER_REPLAY;
		}

		_currentMoveCount = 0;
		ResetGame(TRUE);
		_paused = FALSE;
		return;
	}
	else if (Pad1.B == PAD_RELEASED)
	{
		StepsTaken = MAX_MOVE_COUNT;

		StateMachineChange(&GameMachineState, &StateLevelSelect);
		return;
	}
	else if (Pad1.C == PAD_RELEASED)
	{
		GameInteractionState = GAME_INTERACTION_STATE_NORMAL;
		_currentMoveCount = 0;
		ResetGame(FALSE);
		_paused = FALSE;
		return;
	}	

	if (canReturn)
	{
		if (Pad1.START == PAD_RELEASED)
		{
			HideUIElements();
			VDP_fade(0, 63, _fadedPallete, GameScreenPalette, 10, FALSE);
			_paused = FALSE;
		}
	}
}



void UpdateGameLogic()
{
	if (Pad1.START == PAD_RELEASED)
	{
		ShowPausedUI();
		VDP_fade(0, 63, GameScreenPalette , _fadedPallete, 20, TRUE);
		_paused = TRUE;
		return;
	}
	if (GameInteractionState == GAME_INTERACTION_STATE_USER_REPLAY)
	{
		if (Pad1.A == PAD_RELEASED)
		{
			GameInteractionState = GAME_INTERACTION_STATE_NORMAL;

			// Clear the rest of the movements.
			memset(&_recordedMovements[_currentMoveCount], 0, MAX_MOVE_COUNT - _currentMoveCount);			
		}
	}

	UpdateForceFieldH();
	UpdateForceFieldV();

	UpdateUI();

	switch (PlayerState)
	{

		case PLAYER_STATE_WAITING:
		{
			_playerDead = FALSE;
			_moveType = MOVED_NONE;
			char action = 0;

			HideUIElements();

			if (GameInteractionState == GAME_INTERACTION_STATE_NORMAL)
			{
				if (Pad1.Left == PAD_PRESSED || Pad1.Left == PAD_HELD)
				{
					action = 'L';
				}
				else if (Pad1.Right == PAD_PRESSED || Pad1.Right == PAD_HELD)
				{
					action = 'R';
				}
				else if (Pad1.Up == PAD_PRESSED || Pad1.Up == PAD_HELD)
				{
					action = 'U';
				}
				else if (Pad1.Down == PAD_PRESSED || Pad1.Down == PAD_HELD)
				{
					action = 'D';
				}
				else if (Pad1.A == PAD_RELEASED)
				{
					action = 'x';
				}
			}
			else if (GameInteractionState == GAME_INTERACTION_STATE_USER_REPLAY || GameInteractionState == GAME_INTERACTION_STATE_TEST_REPLAY)
			{
				action = _recordedMovements[_currentMoveCount];
			}


			if (action != 0)
			{
				_recordedMovements[_currentMoveCount] = action;
				_currentMoveCount++;
			}
			else
			{
				GameInteractionState = GAME_INTERACTION_STATE_NORMAL;
			}

			if (action == 'L')
			{
				PlayerMoveLeft();
				_moveType = MOVED_HORIZONTALLY;
			}
			else if (action == 'R')
			{
				PlayerMoveRight();
				_moveType = MOVED_HORIZONTALLY;
			}
			else if (action == 'U')
			{
				PlayerMoveUp();
				_moveType = MOVED_VERTICALLY;
			}
			else if (action == 'D')
			{
				PlayerMoveDown();
				_moveType = MOVED_VERTICALLY;
			}
			else if (action == 'x')
			{
				VDP_fade(0, 47, GameScreenPalette, palette_black, 5, FALSE);

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
			BombHUpdateMovement();
			BombVUpdateMovement();
			DollUpdateMovement();

			UpdatePlans();
			SPR_setPosition(_player, CurrentPlayer->ScreenX, CurrentPlayer->ScreenY);
			break;
		}


		case PLAYER_STATE_FINISHED:
		{
			PlayerEndMove();
			FishFinishMovement();
			ChickenFinishMovement();
			BombVFinishMovement();
			BombHFinishMovement();
			DollFinishMovement();

			break;
		}

		case PLAYER_STATE_TILE_LOGIC:
		{
			if (_moveType == MOVED_VERTICALLY)
			{
				if (BombVUpdateLogic())
				{
					KLog("MOVED_VERTICALLY - BombVUpdateLogic TRUE");
					PlayerState = PLAYER_STATE_TILE_UPDATE;
				}
				else if (BombHUpdateLogic())
				{
					KLog("MOVED_VERTICALLY - BombHUpdateLogic TRUE");
					PlayerState = PLAYER_STATE_TILE_UPDATE;
					_moveType = MOVED_NONE;
				}
				else if (ChickenUpdateLogic())
				{
					PlayerState = PLAYER_STATE_TILE_UPDATE;
				}
				else if (FishUpdateLogic())
				{
					_moveType = MOVED_NONE;
					PlayerState = PLAYER_STATE_TILE_UPDATE;
				}
				else if (DollUpdateLogic())
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
			else
			{
				if (BombHUpdateLogic())
				{
					KLog("MOVED_HORIZONTALLY - BombHUpdateLogic TRUE");
					PlayerState = PLAYER_STATE_TILE_UPDATE;
				}
				else if (BombVUpdateLogic())
				{
					KLog("MOVED_HORIZONTALLY - BombVUpdateLogic TRUE");
					PlayerState = PLAYER_STATE_TILE_UPDATE;
					_moveType = MOVED_NONE;
				}
				else if (FishUpdateLogic())
				{
					PlayerState = PLAYER_STATE_TILE_UPDATE;
				}
				else if (ChickenUpdateLogic())
				{
					PlayerState = PLAYER_STATE_TILE_UPDATE;
				}
				else if (DollUpdateLogic())
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


			if (ExplosionsNeeded())
			{
				PlayerState = PLAYER_STATE_TILE_UPDATE;
			}

			break;
		}

		case PLAYER_STATE_TILE_UPDATE:
		{
			KLog("PLAYER_STATE_TILE_UPDATE");
			u8 allDone = TRUE;

			allDone = allDone && FishUpdateMovement();
			allDone = allDone && ChickenUpdateMovement();
			allDone = allDone && BombHUpdateMovement();
			allDone = allDone && BombVUpdateMovement();
			allDone = allDone && DollUpdateMovement();

			allDone = allDone && ExplosionUpdate();

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
			BombVFinishMovement();
			BombHFinishMovement();
			DollFinishMovement();



			if (BothDead() || StepsTaken >= MAX_MOVE_COUNT)
			{
				_whoopsTimer = 120;
				PlayerState = PLAYER_STATE_GAMEOVER;
				SPR_setVisibility(_player, HIDDEN);
				ShowGameOverUI();
			}
			else if ((CurrentPlayer->Alive == PLAYER_ALIVE_REPORT_NO || OtherPlayer->Alive == PLAYER_ALIVE_REPORT_NO) && !_playerDead)
			{
				ShowWhoopsUI();

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

					VDP_fade(0, 47, GameScreenPalette, palette_black, 5, FALSE);
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
			UpdatePauseScreen(FALSE);
			break;
		}
	}


}









void StatePlayGame_Update()
{
	if (_paused)
	{
		UpdatePauseScreen(TRUE);
	}
	else
	{
		UpdateGameLogic();
	}
}

void StatePlayGame_End()
{
	VDP_fade(0, 63, _fadedPallete, palette_black, 20, FALSE);
	
	VDP_setPaletteColors(0, (u16*)palette_black, 64);
	VDP_clearPlan(PLAN_WINDOW, TRUE);
}



const SimpleState StatePlayGame =
{
	StatePlayGame_Start,
	StatePlayGame_Update,
	StatePlayGame_End,
};