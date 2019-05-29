#include <genesis.h>

#include "Map.h"
#include "StateMapTest.h"
#include "PadHelper.h"

static s16 _cameraX;
static s16 _cameraY;

void StateMapTest_Start()
{
	InitTileSets();
	LoadMap(0);

	_cameraX = 0;
	_cameraY = 0;
}

void StateMapTest_Update()
{
	if (Pad1.Left == PAD_HELD)
	{
		_cameraX--;
	}
	else if (Pad1.Right == PAD_HELD)
	{
		_cameraX++;
	}

	if (Pad1.Up == PAD_HELD)
	{
		_cameraY--;
	}
	else if (Pad1.Down == PAD_HELD)
	{
		_cameraY++;
	}


	if (_cameraX < 0)
	{
		_cameraX = 0;
	}
	else if (_cameraX > 448)
	{
		_cameraX = 448;
	}

	if (_cameraY < 0)
	{
		_cameraY = 0;
	}
	else if (_cameraY > 544)
	{
		_cameraY = 544;
	}

	SetMapCamera(_cameraX, _cameraY);
	UpdatePlans();
}

void StateMapTest_End()
{

}



const SimpleState StateMapTest =
{
	StateMapTest_Start,
	StateMapTest_Update,
	StateMapTest_End,
};