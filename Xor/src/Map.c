#include <genesis.h>
#include "Map.h"
#include "GameMaps.h"
#include "TileDefinitions.h"

#include "TileLoading.h"

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

u16 VramTileIndex = 0;


u8 TileIndexOffset = 0;

static s16 _camX;
static s16 _camY;

static s16 _metaTileXStart = 0;
static s16 _planTileXStart = 0;

static s16 _metaTileYStart = 0;
static s16 _planTileYStart = 0;

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


	VramTileIndex = LoadCurrentTileSet();
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


			u8 tileIdA = 0;
			u8 tileIdB = 0;
			if (LoadedTiles[tileId].GraphicsDefinition->Plan == CONST_PLAN_B)
			{
				tileIdA = TILE_TYPE_EMPTY;
				tileIdB = tileId;
			}
			else
			{
				tileIdA = tileId;
				tileIdB = TILE_TYPE_FLOOR;
			}

			for (u8 i = 0; i < 3; i++)
			{
				for (u8 j = 0; j < 3; j++)
				{					
					u8 loc = (p + j) & PLAN_WIDTH_MODUS;

					_screenColumnA[i][loc] = LoadedTiles[tileIdA].TileAttribs[i][j];
					_screenColumnB[i][loc] = LoadedTiles[tileIdB].TileAttribs[i][j];
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

		u8 tileIdA = 0;
		u8 tileIdB = 0;
		if (LoadedTiles[tileId].GraphicsDefinition->Plan == CONST_PLAN_B)
		{
			tileIdA = TILE_TYPE_EMPTY;
			tileIdB = tileId;
		}
		else
		{
			tileIdA = tileId;
			tileIdB = TILE_TYPE_FLOOR;
		}

		for (u8 i = 0; i < 3; i++)
		{
			for (u8 j = 0; j < 3; j++)
			{
				u8 loc = (plany + j) & PLAN_WIDTH_MODUS;
				_screenColumnA[i][loc] = LoadedTiles[tileIdA].TileAttribs[i][j]; 
				_screenColumnB[i][loc] = LoadedTiles[tileIdB].TileAttribs[i][j];
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

		u8 tileIdA = 0;
		u8 tileIdB = 0;
		if (LoadedTiles[tileId].GraphicsDefinition->Plan == CONST_PLAN_B)
		{
			tileIdA = TILE_TYPE_EMPTY;
			tileIdB = tileId;
		}
		else
		{
			tileIdA = tileId;
			tileIdB = TILE_TYPE_FLOOR;
		}


		for (u8 i = 0; i < 3; i++)
		{
			for (u8 j = 0; j < 3; j++)
			{
				u8 loc = (planx + j) & PLAN_HEIGHT_MODUS;

				_screenRowA[i][loc] = LoadedTiles[tileIdA].TileAttribs[j][i]; 
				_screenRowB[i][loc] = LoadedTiles[tileIdB].TileAttribs[j][i];
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

	if (LoadedTiles[tileId].GraphicsDefinition->Plan == CONST_PLAN_B)
	{
		// set planA to transparent
		memset(_TileBufferA, 0, 18);

		//copy the tile attributes to the buffer
		memcpy(_TileBufferB, LoadedTiles[tileId].TileAttribs, 18);
	}
	else
	{
		// Copy the floor into planB, as that will always be its default value
		memcpy(_TileBufferB, LoadedTiles[_floorTiledId].TileAttribs, 18);
		memcpy(_TileBufferA, LoadedTiles[tileId].TileAttribs, 18);
	}


	int f = 0;
	for (int xx = 0; xx < 3; xx++)
	{	
		for (int yy = 0; yy < 3; yy++)
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