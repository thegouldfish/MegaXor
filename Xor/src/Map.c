#include <genesis.h>
#include "Map.h"
#include "GameMaps.h"
#include "TileDefinitions.h"

#include "../res/gfx.h"
#include "../res/sprite.h"


// defines
#define PLAN_WIDTH 64
#define PLAN_WIDTH_BYTESIZE 128
#define PLAN_WIDTH_MODUS 63

#define PLAN_HEIGHT 64
#define PLAN_HEIGHT_BYTESIZE 128
#define PLAN_HEIGHT_MODUS 63


#define MULTIPLY_BY_32(x)  (x << 5)




// Externed Data
u8 CurrentMapDataState[MAP_SIZE];
const MapDefinition* CurrentMap = 0;

u16 GameScreenPalette[64];
u16 VramTileIndex = 0;


u8 TileIndexOffset = 0;

// Local variables
static TileLookupInfo TileLookupInfos[4];

static s16 _camX;
static s16 _camY;

static s16 _metaTileXStart = 0;
static s16 _planTileXStart = 0;

static s16 _metaTileYStart = 0;
static s16 _planTileYStart = 0;

static u16 TileAttribLookup[TILECOUNT][3][3];

static u8 _floorTiledId = 1;


// Buffers for use with DAM transfers
static u16 _TileBufferB[9];
static u16 _TileBufferA[9];


static u16 _screenColumnB[3][PLAN_HEIGHT];
static u16 _screenColumnA[3][PLAN_HEIGHT];

static u16 _screenRowB[3][PLAN_WIDTH];
static u16 _screenRowA[3][PLAN_WIDTH];




void InitTileSets()
{
	// Set palette to black
	VDP_setPaletteColors(0, (u16*)palette_black, 63);

	VramTileIndex = TILE_USERINDEX;
	
	// Load the tilesets and setup one of the look up tables
	VDP_loadTileSet(tile_set1.tileset, VramTileIndex, DMA);
	memcpy(&GameScreenPalette[0], tile_set1.palette->data, 32);
	
	TileLookupInfos[0].Image = &tile_set1;
	TileLookupInfos[0].Palette = 0;
	TileLookupInfos[0].TileIndex = VramTileIndex;
	TileLookupInfos[0].YLookup[0] = 0;
	TileLookupInfos[0].YLookup[1] = tile_set1.map->w;
	TileLookupInfos[0].YLookup[2] = TileLookupInfos[0].YLookup[1] * 2;

	VramTileIndex += tile_set1.tileset->numTile;

	TileIndexOffset = tile_set1.map->w;

	VDP_loadTileSet(tile_set2.tileset, VramTileIndex, DMA);
	memcpy(&GameScreenPalette[16], tile_set2.palette->data, 32);
	
	TileLookupInfos[1].Image = &tile_set2;
	TileLookupInfos[1].Palette = 1;
	TileLookupInfos[1].TileIndex = VramTileIndex;
	TileLookupInfos[1].YLookup[0] = 0;
	TileLookupInfos[1].YLookup[1] = tile_set2.map->w;
	TileLookupInfos[1].YLookup[2] = TileLookupInfos[1].YLookup[1] * 2;

	VramTileIndex += tile_set2.tileset->numTile;


	VDP_loadTileSet(tile_set3.tileset, VramTileIndex, DMA);
	memcpy(&GameScreenPalette[32], tile_set3.palette->data, 32);
	
	TileLookupInfos[2].Image = &tile_set3;
	TileLookupInfos[2].Palette = 2;
	TileLookupInfos[2].TileIndex = VramTileIndex;
	TileLookupInfos[2].YLookup[0] = 0;
	TileLookupInfos[2].YLookup[1] = tile_set3.map->w;
	TileLookupInfos[2].YLookup[2] = TileLookupInfos[2].YLookup[1] * 2;



	VramTileIndex += tile_set3.tileset->numTile;



	// Create the tile attributes for each of the metatiles and put them into the lookup table
	for (int i = 0; i < TILECOUNT; i++)
	{
		const TileDefinition* tileToDraw = Tiles[i];
		TileLookupInfo* set1 = &TileLookupInfos[tileToDraw->TileSet];
		u16 indx = set1->TileIndex;
		u16 tileNumber = tileToDraw->TileId;

		for (int y = 0; y < 3; y++)
		{
			for (int x = 0; x < 3; x++)
			{
				TileAttribLookup[i][y][x] = TILE_ATTR_FULL(set1->Palette, tileToDraw->Priority , FALSE, FALSE, set1->Image->map->tilemap[set1->YLookup[y] + tileNumber + x] + indx);
			}
		}
	}
}


void LoadMap(u8 index)
{
	CurrentMap = GameMaps[index];
	memcpy(CurrentMapDataState, CurrentMap->MapData, MAP_SIZE);
}









void RedrawScreen(u8 startX, u8 startY)
{
	_metaTileXStart = startX;
	_planTileXStart = startX * 3;

	_metaTileYStart = startY;
	_planTileYStart = startY * 3;

	u16 planx = _planTileXStart;
	u16 plany = _planTileYStart;
	
	for (u8 x = 0; x < 15; x++)
	{
		u16 tileX = x + startX;
		u16 p = plany;
		
		for (u8 y = 0; y < 11; y++)
		{		
			u16 tileY = y + startY;
			

			u16 mapDataIndex = MULTIPLY_BY_32(tileY) + tileX;
			u8 tileId = CurrentMapDataState[mapDataIndex];

			const TileDefinition* tileToDraw = Tiles[tileId];

			for (u8 i = 0; i < 3; i++)
			{
				for (u8 j = 0; j < 3; j++)
				{					
					u8 loc = (p + j) & PLAN_WIDTH_MODUS;

					if (tileToDraw->Plan == TILE_PLAN_B)
					{
						_screenColumnA[i][loc] = 0;
						_screenColumnB[i][loc] = TileAttribLookup[tileId][j][i];
					}
					else
					{
						_screenColumnA[i][loc] = TileAttribLookup[tileId][j][i];
						_screenColumnB[i][loc] = TileAttribLookup[_floorTiledId][j][i];
					}
				}
			}
			p += 3;
		}


		DMA_doDma(DMA_VRAM, (u32)_screenColumnB[0], VDP_PLAN_B + (((planx)     & PLAN_WIDTH_MODUS) << 1), PLAN_HEIGHT, PLAN_WIDTH_BYTESIZE);
		DMA_doDma(DMA_VRAM, (u32)_screenColumnB[1], VDP_PLAN_B + (((planx + 1) & PLAN_WIDTH_MODUS) << 1), PLAN_HEIGHT, PLAN_WIDTH_BYTESIZE);
		DMA_doDma(DMA_VRAM, (u32)_screenColumnB[2], VDP_PLAN_B + (((planx + 2) & PLAN_WIDTH_MODUS) << 1), PLAN_HEIGHT, PLAN_WIDTH_BYTESIZE);


		DMA_doDma(DMA_VRAM, (u32)_screenColumnA[0], VDP_PLAN_A + (((planx)     & PLAN_WIDTH_MODUS) << 1), PLAN_HEIGHT, PLAN_WIDTH_BYTESIZE);
		DMA_doDma(DMA_VRAM, (u32)_screenColumnA[1], VDP_PLAN_A + (((planx + 1) & PLAN_WIDTH_MODUS) << 1), PLAN_HEIGHT, PLAN_WIDTH_BYTESIZE);
		DMA_doDma(DMA_VRAM, (u32)_screenColumnA[2], VDP_PLAN_A + (((planx + 2) & PLAN_WIDTH_MODUS) << 1), PLAN_HEIGHT, PLAN_WIDTH_BYTESIZE);

		planx += 3;
	}	
}



void SetMapCamera(u16 x, u16 y)
{
	_camX = x;
	_camY = y;

	VDP_setHorizontalScroll(PLAN_A, -x);
	VDP_setVerticalScroll(PLAN_A, y);

	VDP_setHorizontalScroll(PLAN_B, -x);
	VDP_setVerticalScroll(PLAN_B, y);	
}


void DrawColumn(u16 startX, u16 startY)
{
	u16 plany = startY * 3;
	u16 planx = startX * 3;

	for (u8 y = 0; y < 11; y++)
	{
		u16 tileY = y + startY;

		u16 mapDataIndex = MULTIPLY_BY_32(tileY) + startX;
		u8 tileId = CurrentMapDataState[mapDataIndex];

		const TileDefinition* tileToDraw = Tiles[tileId];

		for (u8 i = 0; i < 3; i++)
		{
			for (u8 j = 0; j < 3; j++)
			{
				u8 loc = (plany + j) & PLAN_WIDTH_MODUS;

				if (tileToDraw->Plan == TILE_PLAN_B)
				{
					_screenColumnA[i][loc] = 0;
					_screenColumnB[i][loc] = TileAttribLookup[tileId][j][i];
				}
				else
				{
					_screenColumnA[i][loc] = TileAttribLookup[tileId][j][i];
					_screenColumnB[i][loc] = TileAttribLookup[_floorTiledId][j][i];
				}
			}
		}
		plany += 3;
	}


	DMA_doDma(DMA_VRAM, (u32)_screenColumnB[2],    VDP_PLAN_B + (((planx + 2) & PLAN_WIDTH_MODUS) << 1), PLAN_HEIGHT, PLAN_WIDTH_BYTESIZE);
	DMA_queueDma(DMA_VRAM, (u32)_screenColumnB[1], VDP_PLAN_B + (((planx + 1) & PLAN_WIDTH_MODUS) << 1), PLAN_HEIGHT, PLAN_WIDTH_BYTESIZE);
	DMA_queueDma(DMA_VRAM, (u32)_screenColumnB[0], VDP_PLAN_B + (((planx    ) & PLAN_WIDTH_MODUS) << 1), PLAN_HEIGHT, PLAN_WIDTH_BYTESIZE);



	DMA_doDma(DMA_VRAM, (u32)_screenColumnA[2],    VDP_PLAN_A + (((planx + 2) & PLAN_WIDTH_MODUS) << 1), PLAN_HEIGHT, PLAN_WIDTH_BYTESIZE);
	DMA_queueDma(DMA_VRAM, (u32)_screenColumnA[1], VDP_PLAN_A + (((planx + 1) & PLAN_WIDTH_MODUS) << 1), PLAN_HEIGHT, PLAN_WIDTH_BYTESIZE);
	DMA_queueDma(DMA_VRAM, (u32)_screenColumnA[0], VDP_PLAN_A + (((planx    ) & PLAN_WIDTH_MODUS) << 1), PLAN_HEIGHT, PLAN_WIDTH_BYTESIZE);
}



void DrawRow(u16 startX, u16 startY)
{
	u16 plany = startY * 3;
	u16 planx = startX * 3;

	u16 tileYoffest = MULTIPLY_BY_32(startY);

	for (u8 x = 0; x < 15; x++)
	{
		u16 tilex = x + startX;

		u16 mapDataIndex = tileYoffest + tilex;
		u8 tileId = CurrentMapDataState[mapDataIndex];

		const TileDefinition* tileToDraw = Tiles[tileId];

		for (u8 i = 0; i < 3; i++)
		{
			for (u8 j = 0; j < 3; j++)
			{
				u8 loc = (planx + j) & PLAN_HEIGHT_MODUS;

				if (tileToDraw->Plan == TILE_PLAN_B)
				{
					_screenRowA[i][loc] = 0;
					_screenRowB[i][loc] = TileAttribLookup[tileId][i][j];
				}
				else
				{
					_screenRowA[i][loc] = TileAttribLookup[tileId][i][j];
					_screenRowB[i][loc] = TileAttribLookup[_floorTiledId][i][j];
				}
			}
		}
		planx += 3;
	}


	DMA_doDma(DMA_VRAM,    (u32)_screenRowB[2], VDP_PLAN_B + (((((plany + 2) & PLAN_HEIGHT_MODUS) << 6)) << 1), PLAN_WIDTH, 2);
	DMA_queueDma(DMA_VRAM, (u32)_screenRowB[1], VDP_PLAN_B + (((((plany + 1) & PLAN_HEIGHT_MODUS) << 6)) << 1), PLAN_WIDTH, 2);
	DMA_queueDma(DMA_VRAM, (u32)_screenRowB[0], VDP_PLAN_B + (((((plany    ) & PLAN_HEIGHT_MODUS) << 6)) << 1), PLAN_WIDTH, 2);

	DMA_doDma(DMA_VRAM,    (u32)_screenRowA[2], VDP_PLAN_A + (((((plany + 2) & PLAN_HEIGHT_MODUS) << 6)) << 1), PLAN_WIDTH, 2);
	DMA_queueDma(DMA_VRAM, (u32)_screenRowA[1], VDP_PLAN_A + (((((plany + 1) & PLAN_HEIGHT_MODUS) << 6)) << 1), PLAN_WIDTH, 2);
	DMA_queueDma(DMA_VRAM, (u32)_screenRowA[0], VDP_PLAN_A + (((((plany    ) & PLAN_HEIGHT_MODUS) << 6)) << 1), PLAN_WIDTH, 2);
}


void UpdatePlans()
{
	s16 camMetaTileX1 = _camX / 24;
	s16 camMetaTileY1 = _camY / 24;

	if (camMetaTileX1 > _metaTileXStart && camMetaTileX1 < 32)
	{
		// Update Right hand column
		DrawColumn(camMetaTileX1+14, camMetaTileY1);

		_metaTileXStart++;
		_planTileXStart += 3;
	}
	if (camMetaTileX1 < _metaTileXStart && camMetaTileX1 > -1)
	{
		// Update Left hand column
		DrawColumn(camMetaTileX1, camMetaTileY1);

		_metaTileXStart--;
		_planTileXStart -= 3;
	}


	if (camMetaTileY1 > _metaTileYStart && camMetaTileY1 < 32)
	{
		// Update Bottom row
		DrawRow(camMetaTileX1, camMetaTileY1 + 10);
		_metaTileYStart++;
		_planTileYStart += 3;
	}
	if (camMetaTileY1 < _metaTileYStart)
	{
		// udate Top row
		DrawRow(camMetaTileX1, camMetaTileY1);
		_metaTileYStart--;
		_planTileYStart -= 3;
	}
}






void SetTile(u16 tileX, u16 tileY, u16 planX, u16 planY)
{
	int mapDataIndex = (tileY << 5) + tileX;
	int tileId = CurrentMapDataState[mapDataIndex];

	const TileDefinition* tileToDraw = Tiles[tileId];

	if (tileToDraw->Plan == TILE_PLAN_B)
	{
		// set planA to transparent
		memset(_TileBufferA, 0, 18);

		//copy the tile attributes to the buffer
		memcpy(_TileBufferB, TileAttribLookup[tileId], 18);
	}
	else if (tileToDraw->Plan == TILE_PLAN_A)
	{
		// Copy the floor into planB, as that will always be its default value
		memcpy(_TileBufferB, TileAttribLookup[_floorTiledId], 18);
		memcpy(_TileBufferA, TileAttribLookup[tileId], 18);
	}


	int f = 0;
	for (int yy = 0; yy < 3; yy++)
	{
		for (int xx = 0; xx < 3; xx++)
		{
			VDP_setTileMapXY(PLAN_A, _TileBufferA[f], planX + xx, planY + yy);
			VDP_setTileMapXY(PLAN_B, _TileBufferB[f], planX + xx, planY + yy);
			f++;
		}
	}
}

void UpdateTile(u16 metaX, u16 metaY,  u16 tileId)
{
	CurrentMapDataState[MAP_XY_TO_TILE(metaX, metaY)] = tileId;

	s16 offSetX = metaX - _metaTileXStart;
	s16 offSetY = metaY - _metaTileYStart;

	s16 planX = ((offSetX) * 3) + _planTileXStart;
	s16 planY = ((offSetY) * 3) + _planTileYStart;


	SetTile(metaX, metaY, planX, planY);
}

u8 OnScreen(u16 metaX, u16 metaY)
{
	if (metaX >= _metaTileXStart && metaX <= (_metaTileXStart + 14))
	{		
		if (metaY >= _metaTileYStart && metaY <= (_metaTileYStart + 10))
		{
			return TRUE;
		}
	}
	return FALSE;
}