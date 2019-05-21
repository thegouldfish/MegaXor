#include <genesis.h>
#include "Map.h"
#include "GameMaps.h"
#include "TileDefinitions.h"

#include "../res/gfx.h"
#include "../res/sprite.h"

// Externed Data
u8 CurrentMapDataState[MAP_SIZE];
const MapDefinition* CurrentMap = 0;

u16 GameScreenPalette[64];



u16 VramTileIndex = 0;


// Local variables
static TileLookupInfo TileLookupInfos[4];

static u16 _tileBufferFloor[9];

static s16 _camX;
static s16 _camY;

static s16 _metaTileXStart = 0;
static s16 _planTileXStart = 0;

static s16 _metaTileYStart = 0;
static s16 _planTileYStart = 0;

static s16 ScreenWidth = 38;

static u16 TileAttribLookup[TILECOUNT][3][3];

static u8 _floorTiledId = 1;

void InitTileSets()
{
	SYS_disableInts();

	// Set palette to black
	VDP_setPaletteColors(0, (u16*)palette_black, 63);

	VramTileIndex = TILE_USERINDEX;
	

	VDP_loadTileSet(tile_set1.tileset, VramTileIndex, DMA);
	memcpy(&GameScreenPalette[0], tile_set1.palette->data, 32);
	
	TileLookupInfos[0].Image = &tile_set1;
	TileLookupInfos[0].Palette = 0;
	TileLookupInfos[0].TileIndex = VramTileIndex;
	TileLookupInfos[0].YLookup[0] = 0;
	TileLookupInfos[0].YLookup[1] = 15;
	TileLookupInfos[0].YLookup[2] = 30;

	VramTileIndex += tile_set1.tileset->numTile;

	VDP_loadTileSet(tile_set2.tileset, VramTileIndex, DMA);
	memcpy(&GameScreenPalette[16], tile_set2.palette->data, 32);
	
	TileLookupInfos[1].Image = &tile_set2;
	TileLookupInfos[1].Palette = 1;
	TileLookupInfos[1].TileIndex = VramTileIndex;
	TileLookupInfos[1].YLookup[0] = 0;
	TileLookupInfos[1].YLookup[1] = 12;
	TileLookupInfos[1].YLookup[2] = 24;

	VramTileIndex += tile_set2.tileset->numTile;


	VDP_loadTileSet(tile_set3.tileset, VramTileIndex, DMA);
	memcpy(&GameScreenPalette[32], tile_set3.palette->data, 32);
	
	TileLookupInfos[2].Image = &tile_set3;
	TileLookupInfos[2].Palette = 2;
	TileLookupInfos[2].TileIndex = VramTileIndex;
	TileLookupInfos[2].YLookup[0] = 0;
	TileLookupInfos[2].YLookup[1] = 6;
	TileLookupInfos[2].YLookup[2] = 12;



	VramTileIndex += tile_set3.tileset->numTile;

	
	TileLookupInfo* set1 = &TileLookupInfos[0];
	u16 indx = set1->TileIndex;
	_tileBufferFloor[0] = TILE_ATTR_FULL(set1->Palette, FALSE, FALSE, FALSE, set1->Image->map->tilemap[set1->YLookup[0] + 3 + 0] + indx);
	_tileBufferFloor[1] = TILE_ATTR_FULL(set1->Palette, FALSE, FALSE, FALSE, set1->Image->map->tilemap[set1->YLookup[0] + 3 + 1] + indx);
	_tileBufferFloor[2] = TILE_ATTR_FULL(set1->Palette, FALSE, FALSE, FALSE, set1->Image->map->tilemap[set1->YLookup[0] + 3 + 2] + indx);
	_tileBufferFloor[3] = TILE_ATTR_FULL(set1->Palette, FALSE, FALSE, FALSE, set1->Image->map->tilemap[set1->YLookup[1] + 3 + 0] + indx);
	_tileBufferFloor[4] = TILE_ATTR_FULL(set1->Palette, FALSE, FALSE, FALSE, set1->Image->map->tilemap[set1->YLookup[1] + 3 + 1] + indx);
	_tileBufferFloor[5] = TILE_ATTR_FULL(set1->Palette, FALSE, FALSE, FALSE, set1->Image->map->tilemap[set1->YLookup[1] + 3 + 2] + indx);
	_tileBufferFloor[6] = TILE_ATTR_FULL(set1->Palette, FALSE, FALSE, FALSE, set1->Image->map->tilemap[set1->YLookup[2] + 3 + 0] + indx);
	_tileBufferFloor[7] = TILE_ATTR_FULL(set1->Palette, FALSE, FALSE, FALSE, set1->Image->map->tilemap[set1->YLookup[2] + 3 + 1] + indx);
	_tileBufferFloor[8] = TILE_ATTR_FULL(set1->Palette, FALSE, FALSE, FALSE, set1->Image->map->tilemap[set1->YLookup[2] + 3 + 2] + indx);
	



	// Load the Window
	memcpy(&GameScreenPalette[48], window.palette->data, 32);
	VDP_drawImageEx(PLAN_WINDOW, &window, TILE_ATTR_FULL(PAL3, FALSE, FALSE, FALSE, VramTileIndex), 36, 0, FALSE, DMA);
	VramTileIndex += window.tileset->numTile;

	// Create the Attibute Lookup table
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
				TileAttribLookup[i][y][x] = TILE_ATTR_FULL(set1->Palette, FALSE, FALSE, FALSE, set1->Image->map->tilemap[set1->YLookup[y] + tileNumber + x] + indx);
			}
		}
	}

	SYS_enableInts();
}



u16 _TileBufferB[9];
u16 _TileBufferA[9];

void SetTile(u16 tileX, u16 tileY, u16 planX, u16 planY)
{

	int mapDataIndex = (tileY << 5) + tileX;
	int tileId = CurrentMapDataState[mapDataIndex];

	const TileDefinition* tileToDraw = Tiles[tileId];
	const TileLookupInfo* tileLookup = &TileLookupInfos[tileToDraw->TileSet];
	const Image* image = tileLookup->Image;


	// Copy the floor into planB, as that will always be its default value
	memcpy(_TileBufferB, _tileBufferFloor, 18);

	// set planA to empty to start with
	memset(_TileBufferA, 0, 18);

	u16 tileNumber = tileToDraw->TileId;
	u16 indx = TileLookupInfos[tileToDraw->TileSet].TileIndex;
	if (tileToDraw->Plan == TILE_PLAN_B)
	{
		
		_TileBufferB[0] = TILE_ATTR_FULL(tileLookup->Palette, FALSE, FALSE, FALSE, image->map->tilemap[tileLookup->YLookup[0] + tileNumber + 0] + indx);
		_TileBufferB[1] = TILE_ATTR_FULL(tileLookup->Palette, FALSE, FALSE, FALSE, image->map->tilemap[tileLookup->YLookup[0] + tileNumber + 1] + indx);
		_TileBufferB[2] = TILE_ATTR_FULL(tileLookup->Palette, FALSE, FALSE, FALSE, image->map->tilemap[tileLookup->YLookup[0] + tileNumber + 2] + indx);
		_TileBufferB[3] = TILE_ATTR_FULL(tileLookup->Palette, FALSE, FALSE, FALSE, image->map->tilemap[tileLookup->YLookup[1] + tileNumber + 0] + indx);
		_TileBufferB[4] = TILE_ATTR_FULL(tileLookup->Palette, FALSE, FALSE, FALSE, image->map->tilemap[tileLookup->YLookup[1] + tileNumber + 1] + indx);
		_TileBufferB[5] = TILE_ATTR_FULL(tileLookup->Palette, FALSE, FALSE, FALSE, image->map->tilemap[tileLookup->YLookup[1] + tileNumber + 2] + indx);
		_TileBufferB[6] = TILE_ATTR_FULL(tileLookup->Palette, FALSE, FALSE, FALSE, image->map->tilemap[tileLookup->YLookup[2] + tileNumber + 0] + indx);
		_TileBufferB[7] = TILE_ATTR_FULL(tileLookup->Palette, FALSE, FALSE, FALSE, image->map->tilemap[tileLookup->YLookup[2] + tileNumber + 1] + indx);
		_TileBufferB[8] = TILE_ATTR_FULL(tileLookup->Palette, FALSE, FALSE, FALSE, image->map->tilemap[tileLookup->YLookup[2] + tileNumber + 2] + indx);
	}
	else if (tileToDraw->Plan == TILE_PLAN_A)
	{
		_TileBufferA[0] = TILE_ATTR_FULL(tileLookup->Palette, FALSE, FALSE, FALSE, image->map->tilemap[tileLookup->YLookup[0] + tileNumber + 0] + indx);
		_TileBufferA[1] = TILE_ATTR_FULL(tileLookup->Palette, FALSE, FALSE, FALSE, image->map->tilemap[tileLookup->YLookup[0] + tileNumber + 1] + indx);
		_TileBufferA[2] = TILE_ATTR_FULL(tileLookup->Palette, FALSE, FALSE, FALSE, image->map->tilemap[tileLookup->YLookup[0] + tileNumber + 2] + indx);
		_TileBufferA[3] = TILE_ATTR_FULL(tileLookup->Palette, FALSE, FALSE, FALSE, image->map->tilemap[tileLookup->YLookup[1] + tileNumber + 0] + indx);
		_TileBufferA[4] = TILE_ATTR_FULL(tileLookup->Palette, FALSE, FALSE, FALSE, image->map->tilemap[tileLookup->YLookup[1] + tileNumber + 1] + indx);
		_TileBufferA[5] = TILE_ATTR_FULL(tileLookup->Palette, FALSE, FALSE, FALSE, image->map->tilemap[tileLookup->YLookup[1] + tileNumber + 2] + indx);
		_TileBufferA[6] = TILE_ATTR_FULL(tileLookup->Palette, FALSE, FALSE, FALSE, image->map->tilemap[tileLookup->YLookup[2] + tileNumber + 0] + indx);
		_TileBufferA[7] = TILE_ATTR_FULL(tileLookup->Palette, FALSE, FALSE, FALSE, image->map->tilemap[tileLookup->YLookup[2] + tileNumber + 1] + indx);
		_TileBufferA[8] = TILE_ATTR_FULL(tileLookup->Palette, FALSE, FALSE, FALSE, image->map->tilemap[tileLookup->YLookup[2] + tileNumber + 2] + indx);
	}

	

//	VDP_setTileMapDataRect(PLAN_A, _TileBufferA,  planX % 64, planY % 64, 3, 3);
//	VDP_setTileMapDataRect(PLAN_B, _TileBufferB, planX % 64, planY % 64, 3, 3);

	
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

#define PLAN_WIDTH 64
#define PLAN_WIDTH_BYTESIZE 128
#define PLAN_WIDTH_MODUS 63

#define PLAN_HEIGHT 64
#define PLAN_HEIGHT_BYTESIZE 128
#define PLAN_HEIGHT_MODUS 63


#define MULTIPLY_BY_32(x)  (x << 5)


static u16 _screenColumnB[3][PLAN_HEIGHT];
static u16 _screenColumnA[3][PLAN_HEIGHT];

static u16 _screenRowB[3][PLAN_WIDTH];
static u16 _screenRowA[3][PLAN_WIDTH];


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


void LoadMap(u8 index)
{
	CurrentMap = GameMaps[index];
	memcpy(CurrentMapDataState, CurrentMap->MapData, MAP_SIZE);
}


void RedrawScreen_old(u8 startX, u8 startY)
{
	return;
	KLog_U2("startX  ",startX, "  startY  ",startY);
	// 14 x 10
	// 3 x 3

	_metaTileXStart = startX;
	_planTileXStart = startX * 3;

	_metaTileYStart = startY;
	_planTileYStart = startY * 3;

	int planx = startX * 3;
	int plany = startY * 3;
	for (int x = 0; x < 15; x++)
	{
		for (int y = 0; y < 11; y++)
		{
			SetTile(x + startX, y + startY, planx, plany);
			plany += 3;
		}

		plany = startY * 3;
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





void UpdatePlans()
{
	s16 camMetaTileX1 = _camX / 24;
	s16 camMetaTileY1 = _camY / 24;

	if (camMetaTileX1 > _metaTileXStart && camMetaTileX1 < 32)
	{
		s16 yy = _planTileYStart;
		for (u8 y = 0; y < 11; y++)
		{
			SetTile(camMetaTileX1 + 14, camMetaTileY1 + y, _planTileXStart + 45, yy);
			yy += 3;
		}

		_metaTileXStart++;
		_planTileXStart += 3;
	}
	if (camMetaTileX1 < _metaTileXStart && camMetaTileX1 > -1)
	{
		s16 yy = _planTileYStart;
		for (u8 y = 0; y < 11; y++)
		{
			SetTile(camMetaTileX1, camMetaTileY1 + y, _planTileXStart -3, yy);
			yy += 3;
		}

		_metaTileXStart--;
		_planTileXStart -= 3;
	}



	if (camMetaTileY1 > _metaTileYStart && camMetaTileY1 < 32)
	{
		s16 xx = _planTileXStart;
		for (u8 x = 0; x < 15; x++)
		{
			SetTile(camMetaTileX1 + x, camMetaTileY1 + 10, xx, _planTileYStart + 33);
			xx += 3;
		}

		_metaTileYStart++;
		_planTileYStart += 3;
	}
	if (camMetaTileY1 < _metaTileYStart)
	{
		s16 xx = _planTileXStart;
		for (u8 x = 0; x < 15; x++)
		{
			SetTile(camMetaTileX1 + x, camMetaTileY1, xx, _planTileYStart - 3);
			xx += 3;
		}

		_metaTileYStart--;
		_planTileYStart -= 3;
	}
}


void UpdateTile(u16 metaX, u16 metaY,  u16 tileId)
{
	CurrentMapDataState[MAP_XY_TO_TILE(metaX, metaY)] = tileId;
	

	s16 offSetX = metaX - _metaTileXStart;
	s16 offSetY = metaY - _metaTileYStart;

	//KLog_U4("offsetX ", offSetX, " offsetY ", offSetY, " _planTileXStart ", _planTileXStart ," _planTileYStart ", _planTileYStart);

	s16 planX = ((offSetX) * 3) + _planTileXStart;
	s16 planY = ((offSetY) * 3) + _planTileYStart;


	//KLog_U4("Update Tile ", metaX, " y ", metaY, "  planX ",planX, " planY ", planY);
	SetTile(metaX, metaY, planX, planY);
}