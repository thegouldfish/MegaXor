#ifndef _COMMON_LOGIC_
#define _COMMON_LOGIC_

#include <genesis.h>

s8 FindId(u16 x, u16 y, const ActivePoint* items, const int count);

void DoMovementCommon(u8 x, u8 y, u8 newX, u8 newY, u8 direction, u8 trackedId, ActiveTileItem* activeTile, ActivePoint* items);

u8 UpdateMovementCommon(ActiveTileItem* activeTile);

#endif