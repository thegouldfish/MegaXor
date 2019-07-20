#include <genesis.h>

#include "Map.h"
#include "StateMoveTest.h"
#include "PadHelper.h"
#include "TileDefinitions.h"

#include "../res/sprite.h"

static s16 _cameraX;
static s16 _cameraY;

static s16 _playerX;
static s16 _playerY;

static s16 _playerScreenX;
static s16 _playerScreenY;


static Sprite* _player;


static void SetPositions()
{
	//KLog_S2("x ", _playerX, " y ", _playerY);

	s16 midX = screenWidth / 2;
	s16 midY = screenHeight / 2;

	//KLog_S2("midx ", midX, " midy ", midY);


	_playerScreenX = (_playerX * 24);
	_playerScreenY = (_playerY * 24);

	s16 left = _playerScreenX - midX;
	s16 right = _playerScreenX + midX;


	//KLog_S4("sx ", _playerScreenX, " sy ", _playerScreenY, " left ",left, " right ", right);
	if (left < 0)
	{
		_cameraX = 0;
	}
	else if (right > 768)
	{
		_cameraX = 768 - screenWidth;
		_playerScreenX += (screenWidth - 768);
	}
	else
	{
		_cameraX = left;
		_playerScreenX -= _cameraX;
	}


	s16 top = _playerScreenY - midY;
	s16 bottom = _playerScreenY + midY;
	if (top < 0)
	{
		_cameraY = 0;
	}
	else if (bottom > 768)
	{
		_cameraY = 768 - screenHeight;
		_playerScreenY += (screenHeight - 768);
	}
	else
	{
		_cameraY = top;
		_playerScreenY -= _cameraY;
	}

	//KLog_S2("camx ", _cameraX, " camy ", _cameraY);

	SetMapCamera(_cameraX, _cameraY);
	SPR_setPosition(_player, _playerScreenX, _playerScreenY);
}


void StateMoveTest_Start()
{
	InitTileSets();
	LoadMap(0);

	
	VDP_setPalette(PAL3, ShieldSprites.palette->data);
	_player = SPR_addSprite(&ShieldSprites, 0, 0, TILE_ATTR(PAL3, TRUE, FALSE, FALSE));
	SPR_setFrame(_player, 0);

	_cameraX = 0;
	_cameraY = 0;

	int i = 0;
	for (int y = 0; y < 32; y++)
	{
		for (int x = 0; x < 32; x++)
		{
			if (CurrentMap->MapData[i] == TILE_TYPE_MAGNUS)
			{
				_playerX = x;
				_playerY = y;
				x = 100;
				y = 100;
			}
			i++;
		}
	}

	RedrawScreen(0, 0);
	SetPositions();
}





void StateMoveTest_Update()
{



	if (Pad1.Left == PAD_PRESSED)
	{
		_playerX--;
	}
	else if (Pad1.Right == PAD_PRESSED)
	{
		_playerX++;
	}

	if (Pad1.Up == PAD_PRESSED)
	{
		_playerY--;
	}
	else if (Pad1.Down == PAD_PRESSED)
	{
		_playerY++;
	}

	SetPositions();
	UpdatePlans();
}

void StateMoveTest_End()
{

}



const SimpleState StateMoveTest =
{
	StateMoveTest_Start,
	StateMoveTest_Update,
	StateMoveTest_End,
};