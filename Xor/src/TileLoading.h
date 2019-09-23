#ifndef _TILE_LOADING_H_
#define _TILE_LOADING_H_
#include <genesis.h>
#include "TileSets.h"
#include "TileDefinitions.h"

typedef struct
{
	const GraphicsDefinition* GraphicsDefinition;
	
	
	u16 CurrentAnimationFrame;

	u16 FrameWaitTime;
	u16 FrameTimer;

	u16 VramIndex;
	u16 TileAttribs[3][3];

}LoadedTile;



u16 LoadCurrentTileSet();

void BlankTiles(u16 tileId);
void ReLoadTiles(u16 tileId);

u16 LoadTiles(TileSet* set, u16 vramLoaction, u8 id);

void TickTiles();

extern LoadedTile LoadedTiles[TILECOUNT];

#endif