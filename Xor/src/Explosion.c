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

#include "TileLoading.h"


#define MAX_EXPLOSIONS 10
static Point _explosions[MAX_EXPLOSIONS];

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
		if (LoadedTiles[TILE_TYPE_EXPLOSION].CurrentAnimationFrame == LoadedTiles[TILE_TYPE_EXPLOSION].GraphicsDefinition->AnimationCount - 1)
		{			
			for (int i = 0; i < MAX_EXPLOSIONS; i++)
			{
				if (_explosions[i].X != -1)
				{
					if (OnScreen(_explosions[i].X, _explosions[i].Y))
					{
						UpdateTile(_explosions[i].X, _explosions[i].Y, TILE_TYPE_FLOOR);
					}
					else
					{
						CurrentMapDataState[MAP_XY_TO_TILE(_explosions[i].X, _explosions[i].Y)] = TILE_TYPE_FLOOR;
					}						
					_explosions[i].X = -1;
					_explosions[i].Y = -1;
				}
			}

				
			ExplosionHappenedUI();
			_explosionCount = 0;
			_explotionsTriggered = FALSE;
			return TRUE;
		}

		return FALSE;
	}
}





void TriggerExplosions()
{
	if (_explosionCount != 0)
	{
		LoadedTiles[TILE_TYPE_EXPLOSION].CurrentAnimationFrame = 0;
		LoadedTiles[TILE_TYPE_EXPLOSION].FrameTimer = 0;

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

				if (OnScreen(x, y))
				{
					UpdateTile(x, y, TILE_TYPE_EXPLOSION);
				}
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

