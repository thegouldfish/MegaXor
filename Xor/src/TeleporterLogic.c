#include "TeleporterLogic.h"
#include "Xor.h"
#include <genesis.h>
#include "Map.h"
#include "TileDefinitions.h"
#include "Players.h"

static Point _teleporterLocations[2];

static s8 _teleportTo = -1;

void SetupTeleporters()
{
	u8 i = 0;
	for (s8 y = 0; y < MAP_HEIGHT; y++)
	{
		for (s8 x = 31; x >= 0; x--)
		{
			if (CurrentMapDataState[MAP_XY_TO_TILE(x, y)] == TILE_TYPE_TELEPORTER)
			{
				_teleporterLocations[i].X = x;
				_teleporterLocations[i].Y = y;
				i++;
			}
		}
	}
	
	_teleportTo = -1;
}

u8 TeleportCurrentPlayer()
{
	if (CurrentPlayer->CanTeleport)
	{
		if (CurrentPlayer->MetaX == _teleporterLocations[0].X && CurrentPlayer->MetaY == _teleporterLocations[0].Y)
		{
			_teleportTo = 1;
		}
		else if (CurrentPlayer->MetaX == _teleporterLocations[1].X && CurrentPlayer->MetaY == _teleporterLocations[1].Y)
		{
			_teleportTo = 0;
		}
		else
		{
			return FALSE;
		}

		return TRUE;
	}

	return FALSE;
}



u8 FinishTeleport(Sprite* player)
{
	if (_teleportTo != -1)
	{
		VDP_fade(0, 47, GamePalette, palette_black, 5, FALSE);


		// test if the square to the right is empty
		u16 teleportX = _teleporterLocations[(u8)_teleportTo].X + 1;
		u16 teleportY = _teleporterLocations[(u8)_teleportTo].Y;

		if (CurrentMapDataState[MAP_XY_TO_TILE(teleportX, teleportY)] != TILE_TYPE_FLOOR)
		{
			// test if square above is empty
			teleportX = _teleporterLocations[(u8)_teleportTo].X;
			teleportY = _teleporterLocations[(u8)_teleportTo].Y - 1;


			if (CurrentMapDataState[MAP_XY_TO_TILE(teleportX, teleportY)] != TILE_TYPE_FLOOR)
			{
				// test if square to the left is empty
				teleportX = _teleporterLocations[(u8)_teleportTo].X - 1;
				teleportY = _teleporterLocations[(u8)_teleportTo].Y;

				if (CurrentMapDataState[MAP_XY_TO_TILE(teleportX, teleportY)] != TILE_TYPE_FLOOR)
				{
					teleportX = _teleporterLocations[(u8)_teleportTo].X;
					teleportY = _teleporterLocations[(u8)_teleportTo].Y + 1;

					if (CurrentMapDataState[MAP_XY_TO_TILE(teleportX, teleportY)] != TILE_TYPE_FLOOR)
					{
						PlayerKillCurrent();
					}
				}
			}
		}


		CurrentPlayer->MetaX = teleportX;
		CurrentPlayer->MetaY = teleportY;

		PlayerUpdateMove(TRUE);
		RedrawScreen(CurrentPlayer->ScreenMetaX, CurrentPlayer->ScreenMetaY);
		SPR_setPosition(player, CurrentPlayer->ScreenX, CurrentPlayer->ScreenY);

		SPR_update();
		VDP_waitVSync();
		VDP_fadeIn(0, (3 * 16) - 1, GamePalette, 5, FALSE);

		CurrentPlayer->CanTeleport = FALSE;
		_teleportTo = -1;
	}

	return TRUE;
}