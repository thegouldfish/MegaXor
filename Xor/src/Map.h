#ifndef MAP_
#define MAP_

#include <genesis.h>
#include "GameMaps.h"

typedef struct
{
	const Image* Image;
	u16 TileIndex;
	u8 YLookup[3];
	u8 Palette;

}TileLookupInfo;

const extern MapDefinition* CurrentMap;
extern u8 CurrentMapDataState[MAP_SIZE];

extern u16 GameScreenPalette[64];

extern u16 VramTileIndex;

void InitTileSets();

void LoadMap(u8 index);

void RedrawScreen(u8 startX, u8 startY);

void SetMapCamera(u16 x, u16 y);

void UpdatePlans();

void UpdateTile(u16 metaX, u16 metaY, u16 tileId);

#endif