#include <genesis.h>
#include "Explosion.h"
#include "Xor.h"
#include "Map.h"
#include "TileDefinitions.h"
#include "Players.h"
#include "UI.h"
#include "SwitchLogic.h"
#include "ChickenLogic.h"
#include "FishLogic.h"

#include "../res/gfx.h"


#define MAX_EXPLOSIONS 10
static Point _explosions[MAX_EXPLOSIONS];

static s16 _explosionTimer = 0;
static s16 _explosionFrameCount = 6;

static s16 _explosionAnimationCount = 5;
static s16 _explosionCurrentFrame = 0;

static s16 _explosionCount = 0;

static u8 _explotionsTriggered = FALSE;

u8 ExplosionsNeeded()
{
	return _explosionCount > 0;
}


void ExplosionSetup()
{
	for (int i = 0; i < MAX_EXPLOSIONS; i++)
	{
		_explosions[i].X = -1;
		_explosions[i].Y = -1;
	}
}


u8 ExplosionUpdate()
{
	if (_explosionCount == 0 || !_explotionsTriggered)
	{
		return TRUE;
	}
	else
	{
		_explosionTimer--;

		if (_explosionTimer == 0)
		{
			_explosionTimer = _explosionFrameCount;
			_explosionCurrentFrame++;

			if (_explosionCurrentFrame == _explosionAnimationCount)
			{				
				for (int i = 0; i < MAX_EXPLOSIONS; i++)
				{
					if (_explosions[i].X != -1)
					{
						UpdateTile(_explosions[i].X, _explosions[i].Y, TILE_TYPE_FLOOR);

						_explosions[i].X = -1;
						_explosions[i].Y = -1;
					}
				}

				
				ExplosionHappenedUI();
				_explosionCount = 0;
				_explotionsTriggered = FALSE;
				return TRUE;
			}
			else
			{				
				u16 index = TILE_USERINDEX + 15;
				u32* tilePtr = tile_explosion.tileset->tiles;
				tilePtr += (_explosionCurrentFrame * 24);
				
				for (int i = 0; i < 3; i++)
				{
					VDP_loadTileData(tilePtr, index, 3, CPU);
					index += TileIndexOffset;
					tilePtr += 120;
				}
			}
		}

		return FALSE;
	}
}





void TriggerExplosions()
{
	if (_explosionCount != 0)
	{
		_explosionTimer = _explosionFrameCount;
		_explosionCurrentFrame = 0;
		_explotionsTriggered = TRUE;

		KLog("Trigger booms");
		for (int i = 0; i < MAX_EXPLOSIONS; i++)
		{

			if (_explosions[i].X != -1)
			{
				u16 x = _explosions[i].X;
				u16 y = _explosions[i].Y;

				u8 tile = CurrentMapDataState[MAP_XY_TO_TILE(x, y)];
				if (CurrentPlayer->MetaX == x && CurrentPlayer->MetaY == y)
				{
					PlayerKillCurrent();
				}
				else if (tile == TILE_TYPE_MAGNUS || tile == TILE_TYPE_QUESTOR)
				{
					PlayerKillOther();
				}

				if (tile == TILE_TYPE_SWITCH)
				{
					SwitchToggle(x, y);
				}
				else if (tile == TILE_TYPE_CHICKEN)
				{
					KillChicken(x, y);
				}
				else if (tile == TILE_TYPE_FISH)
				{
					KillFish(x, y);
				}

				UpdateTile(x, y, TILE_TYPE_EXPLOSION);
			}
		}
	}
}


void PlaceExplosion(u16 x, u16 y)
{
	_explosionCount++;
	for (int i = 0; i < MAX_EXPLOSIONS; i++)
	{
		if (_explosions[i].X == -1)
		{
			_explosions[i].X = x;
			_explosions[i].Y = y;

			break;
		}
	}
}

