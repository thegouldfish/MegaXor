#include <genesis.h>
#include "Xor.h"
#include "Map.h"
#include "TileDefinitions.h"
#include "CommonLogic.h"



u8 FindId(u16 x, u16 y, const ActivePoint* items, const int count)
{
	for (u8 i = 0; i < count; i++)
	{
		if (items[i].X == x && items[i].Y == y)
		{
			return i;
		}
	}

	return 0;
}


void DoMovementCommon(u8 x, u8 y, u8 newX, u8 newY, u8 direction, u8 trackedId, ActiveTileItem* activeTile, ActivePoint* items)
{
	activeTile->CurrentMetaX = x;
	activeTile->CurrentMetaY = y;

	activeTile->DestinationMetaX = newX;
	activeTile->DestinationMetaY = newY;


	activeTile->DestinationScreenX = newX * 24;
	activeTile->DestinationScreenY = newY * 24;

	activeTile->OnScreen = OnScreen(x, y) || OnScreen(activeTile->DestinationMetaX, activeTile->DestinationMetaY);

	activeTile->MoveRemaining = fix32Add(MovementPerFrame, FIX32(24));
	activeTile->MoveDirection = direction;

	activeTile->IsActive = TRUE;

	CurrentMapDataState[MAP_XY_TO_TILE(x, y)] = TILE_TYPE_FLOOR;
	UpdateMovementCommon(activeTile);

	items[trackedId].Active = TRUE;
	activeTile->ActiveIndex = trackedId;

	if (activeTile->OnScreen)
	{
		UpdateTile(activeTile->CurrentMetaX, activeTile->CurrentMetaY, TILE_TYPE_FLOOR);
	}	
}


u8 UpdateMovementCommon(ActiveTileItem* activeTile)
{
	u8 done = TRUE;

	if (activeTile->IsActive)
	{
		if (activeTile->OnScreen)
		{
			activeTile->MoveRemaining = fix32Sub(activeTile->MoveRemaining, MovementPerFrame);

			if (fix32ToInt(activeTile->MoveRemaining) < 0)
			{
				activeTile->MoveRemaining = 0;
			}
			else
			{
				done = FALSE;
			}

			s16 x = 0;
			s16 y = 0;
			switch (activeTile->MoveDirection)
			{
				case MOVE_DIRECTION_LEFT:
				{
					y = activeTile->DestinationScreenY;
					x = activeTile->DestinationScreenX + fix32ToInt(activeTile->MoveRemaining);
					break;
				}

				case MOVE_DIRECTION_RIGHT:
				{
					y = activeTile->DestinationScreenY;
					x = activeTile->DestinationScreenX - fix32ToInt(activeTile->MoveRemaining);
					break;
				}

				case MOVE_DIRECTION_UP:
				{
					y = activeTile->DestinationScreenY + fix32ToInt(activeTile->MoveRemaining);
					x = activeTile->DestinationScreenX;
					break;
				}

				case MOVE_DIRECTION_DOWN:
				{
					y = activeTile->DestinationScreenY - fix32ToInt(activeTile->MoveRemaining);
					x = activeTile->DestinationScreenX;
					break;
				}

				default:
					break;
			}

			x += CameraXOffset;
			y += CameraYOffset;

			SPR_setPosition(activeTile->ActiveSprite, x, y);
			SPR_setVisibility(activeTile->ActiveSprite, VISIBLE);
			SPR_update();
		}
	}

	return done;
}