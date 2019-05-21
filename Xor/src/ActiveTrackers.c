#include <genesis.h>
#include "Xor.h"
#include "ActiveTrackers.h"

#define ACTIVE_TILE_COUNT 16
static Point _activeMovers[ACTIVE_TILE_COUNT];


void ActiveReset()
{
	for (int i = 0; i < ACTIVE_TILE_COUNT; i++)
	{
		_activeMovers[i].X = -1;
		_activeMovers[i].Y = -1;
	}
}

void ActiveSet(u16 x, u16 y)
{
	KLog_U2("set active x ", x, " y ", y);
	for (int i = 0; i < ACTIVE_TILE_COUNT; i++)
	{
		if (_activeMovers[i].X == -1 && _activeMovers[i].Y == -1)
		{
			_activeMovers[i].X = x;
			_activeMovers[i].Y = y;
			return;
		}
	}
}

void ActiveRemove(u16 x, u16 y)
{
	for (int i = 0; i < ACTIVE_TILE_COUNT; i++)
	{
		if (_activeMovers[i].X == x && _activeMovers[i].Y == y)
		{
			_activeMovers[i].X = -1;
			_activeMovers[i].Y = -1;
			return;
		}
	}
}

u8 ActiveEnabled(u16 x, u16 y)
{
	for (int i = 0; i < ACTIVE_TILE_COUNT; i++)
	{
		if (_activeMovers[i].X == x && _activeMovers[i].Y == y)
		{
			return TRUE;
		}
	}

	return FALSE;
}


void ActivePrint()
{
	KLog("Active Movers");
	for (int i = 0; i < ACTIVE_TILE_COUNT; i++)
	{
		KLog_S3("    i ", i, " x ",_activeMovers[i].X, " y ", _activeMovers[i].Y);
	}
}