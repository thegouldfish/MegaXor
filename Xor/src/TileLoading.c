#include "TileLoading.h"
#include "Map.h"
#include "TileSets.h"

#include "TileDefinitions.h"
#include "Xor.h"

#include "../res/gfx.h"

// Externed Data
LoadedTile LoadedTiles[TILECOUNT];
const u32 blankTiles[9 * 8] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };


void BlankTiles(u16 tileId)
{
	u16 vramLoaction = LoadedTiles[tileId].VramIndex;
	VDP_loadTileData(blankTiles, vramLoaction, 9, DMA);
}


void ReLoadTiles(u16 tileId)
{
	u16 vramLoaction = LoadedTiles[tileId].VramIndex;
	
	LoadTiles(LoadedTiles[tileId].GraphicsDefinition->Tile->tileset, vramLoaction, LoadedTiles[tileId].CurrentAnimationFrame);


}


u16 LoadTiles(TileSet* set, u16 vramLoaction, u8 id)
{
	u16 offset = ((set->numTile / 3) << 3);
	u32* tilePlace = (set->tiles + ((3 << 3) * id));

	for (int i = 0; i < 3; i++)
	{
		VDP_loadTileData(tilePlace, vramLoaction, 3, DMA);

		tilePlace += offset;
		vramLoaction += 3;
	}

	return vramLoaction;
}


void TickTiles()
{
	for (u16 i = 0; i < TILECOUNT -1; i++)
	{
		if (LoadedTiles[i].FrameWaitTime != 0)
		{
			LoadedTiles[i].FrameTimer++;

			if (LoadedTiles[i].FrameTimer >= LoadedTiles[i].FrameWaitTime)
			{
				LoadedTiles[i].FrameTimer = 0;
				LoadedTiles[i].CurrentAnimationFrame++;

				if (LoadedTiles[i].CurrentAnimationFrame >= LoadedTiles[i].GraphicsDefinition->AnimationCount)
				{
					LoadedTiles[i].CurrentAnimationFrame = 0;
				}

				LoadTiles(LoadedTiles[i].GraphicsDefinition->Tile->tileset, LoadedTiles[i].VramIndex, LoadedTiles[i].CurrentAnimationFrame);
			}
		}
	}
}


int LoadDefinition(const TileGraphics* def, u16 tileId, u16 vramIndex)
{
	LoadedTiles[tileId].GraphicsDefinition = def->Definitions[tileId];
	LoadedTiles[tileId].FrameTimer = 0;

	if (IS_PALSYSTEM)
	{
		LoadedTiles[tileId].FrameWaitTime = def->Definitions[tileId]->AnimationWaitFramesPAL;
	}
	else
	{
		LoadedTiles[tileId].FrameWaitTime = def->Definitions[tileId]->AnimationWaitFramesNTSC;
	}

	LoadedTiles[tileId].CurrentAnimationFrame = 0;
	LoadedTiles[tileId].VramIndex = vramIndex;
	
	
	u16 i = 0;
	for (u16 x = 0; x < 3; x++)
	{
		for (u16 y = 0; y < 3; y++)
		{
			LoadedTiles[tileId].TileAttribs[y][x] = TILE_ATTR_FULL(def->Definitions[tileId]->Palette, def->Definitions[tileId]->Priority, FALSE, FALSE, vramIndex + i);
			i++;
		}
	}

	vramIndex = LoadTiles(def->Definitions[tileId]->Tile->tileset, vramIndex, 0);

	return vramIndex;
}




u16 LoadCurrentTileSet()
{
	memset(&GamePalette, 0, 128);

	VramTileIndex = TILE_USERINDEX;
	
	for (int i = 0; i < TILECOUNT - 1; i++)
	{
		VramTileIndex = LoadDefinition(SelectedGraphicsSet, i, VramTileIndex);
	}


	LoadedTiles[TILECOUNT - 1].CurrentAnimationFrame = 0;
	LoadedTiles[TILECOUNT - 1].FrameTimer = 0;
	LoadedTiles[TILECOUNT - 1].FrameWaitTime = 0;
	LoadedTiles[TILECOUNT - 1].GraphicsDefinition = NULL;
	LoadedTiles[TILECOUNT - 1].VramIndex = 0;
	memset(LoadedTiles[TILECOUNT - 1].TileAttribs, 0, 9 * 2);




	for (int i = 0; i < 3; i++)
	{
		u8 index = SelectedGraphicsSet->PaletteIndexes[i];
		memcpy(&GamePalette[i * 16], SelectedGraphicsSet->Definitions[index]->Tile->palette->data, 32);
	}	

	return VramTileIndex;
}

