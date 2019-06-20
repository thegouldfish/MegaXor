#include <genesis.h>
#include "UI.h"
#include "Map.h"
#include "TileDefinitions.h"
#include "Xor.h"
#include "Players.h"

#include "../res/gfx.h"
#include "../res/sprite.h"



static u16 _miniMapVramIndex = 0;
static s16 _previousMapCount = 0;
static s16 _previousMasksCollected = 0;
static s16 _previousStepsTaken = 0;

static u8 _explosionHappened;

static Sprite* _masksCollected[2];
static Sprite* _masksTotal[2];
static Sprite* _moves[4];

static Sprite* _upperUIElement;
static Sprite* _middleUIElement;
static Sprite* _lowerUIElement;

static void SetNumbers(int number, Sprite* sprites[], const int size)
{
	int numbs[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int i = size - 1;
	while (number > 0)
	{
		if (number < 10)
		{
			numbs[i] = number;
		}
		else
		{
			numbs[i] = (number % 10);
		}

		number /= 10;
		i--;
	}



	for (int i = 0; i < 10 && i < size; i++)
	{
		SPR_setFrame(sprites[i], numbs[i]);
	}
}


// The minimap is made up of 32 x 32 pixels
// aka 4 x 4 tiles.
// this code will at the current state of the map data then set 1 pixel for each "map tile".
// which basically means bitshifting lots of colour data into the tile.
static void GenerateMiniMap()
{
	u16 yStart = 0;
	u16 xStart = 0;

	u32 tileSet[8];

	u16 tileOffset = 0;

	for (u16 miniMapTileY = 0; miniMapTileY < 4; miniMapTileY++)
	{
		for (u16 miniMapTileX = 0; miniMapTileX < 4; miniMapTileX++)
		{
			xStart = miniMapTileX << 3;
			yStart = miniMapTileY << 3;
			memset(tileSet, 0, 8 * 4);

			for (u16 y = yStart; y < yStart + 8; y++)
			{
				for (u16 x = xStart; x < xStart + 8; x++)
				{
					u16 i = (y << 5) + x;

					u16 bitshiftOffset = (7 - (x - xStart)) << 2;

					u16 toSet = 15;

					if (CurrentMapDataState[i] == TILE_TYPE_WALL)
					{
						toSet = 7;
					}
					else if (CurrentMapDataState[i] == TILE_TYPE_XOR)
					{
						toSet = 1;
					}


					tileSet[y - yStart] = tileSet[y - yStart] | (toSet << bitshiftOffset);
				}
			}

			VDP_loadTileData((const u32 *)tileSet, _miniMapVramIndex + tileOffset, 1, DMA);
			tileOffset++;
		}
	}
}


static u16 _windowVramIndex = 0;
void SetupUI()
{
	_masksCollected[0] = SPR_addSprite(&sp_numbers, 296, 68, TILE_ATTR(PAL3, FALSE, FALSE, FALSE));
	_masksCollected[1] = SPR_addSprite(&sp_numbers, 304, 68, TILE_ATTR(PAL3, FALSE, FALSE, FALSE));


	_masksTotal[0] = SPR_addSprite(&sp_numbers, 296, 130, TILE_ATTR(PAL3, FALSE, FALSE, FALSE));
	_masksTotal[1] = SPR_addSprite(&sp_numbers, 304, 130, TILE_ATTR(PAL3, FALSE, FALSE, FALSE));


	_moves[0] = SPR_addSprite(&sp_numbers, 288, 172, TILE_ATTR(PAL3, FALSE, FALSE, FALSE));
	_moves[1] = SPR_addSprite(&sp_numbers, 296, 172, TILE_ATTR(PAL3, FALSE, FALSE, FALSE));
	_moves[2] = SPR_addSprite(&sp_numbers, 304, 172, TILE_ATTR(PAL3, FALSE, FALSE, FALSE));
	_moves[3] = SPR_addSprite(&sp_numbers, 312, 172, TILE_ATTR(PAL3, FALSE, FALSE, FALSE));


	// Load the Window
	memcpy(&GameScreenPalette[48], window.palette->data, 32);
	_windowVramIndex = VramTileIndex;
	VramTileIndex += window.tileset->numTile;


	_miniMapVramIndex = VramTileIndex;
	GenerateMiniMap();
	VramTileIndex += 16;


	_upperUIElement  = SPR_addSprite(&ui_elements_sprites,  96, 80, TILE_ATTR(PAL3, TRUE, FALSE, FALSE));
	_middleUIElement = SPR_addSprite(&ui_elements_sprites,  96, 96, TILE_ATTR(PAL3, TRUE, FALSE, FALSE));
	_lowerUIElement  = SPR_addSprite(&ui_elements_sprites,  96, 112, TILE_ATTR(PAL3, TRUE, FALSE, FALSE));


	SPR_setVisibility(_upperUIElement, HIDDEN);
	SPR_setVisibility(_middleUIElement, HIDDEN);
	SPR_setVisibility(_lowerUIElement, HIDDEN);
	ResetUI();
}


void ResetUI()
{
	_previousMapCount = 0;
	_previousMasksCollected = 0;
	_previousStepsTaken = 0;


	SetNumbers(TotalMasks, _masksTotal, 2);


	SetNumbers(MasksCollected, _masksCollected, 2);
	SetNumbers(StepsTaken, _moves, 4);

	VDP_drawImageEx(PLAN_WINDOW, &window, TILE_ATTR_FULL(PAL3, FALSE, FALSE, FALSE, _windowVramIndex), 36, 0, FALSE, DMA);
}



void UpdateUI()
{
	if (_previousMapCount != MapsCollected)
	{
		for (s16 i = 0; i < MapsCollected; i++)
		{
			int mapIndex = _miniMapVramIndex;
			int x = 36;
			int y = 2;

			if (i == 1)
			{
				x = 38;
				mapIndex = _miniMapVramIndex + 2;
			}
			else if (i == 2)
			{
				y = 4;
				mapIndex = _miniMapVramIndex + 8;
			}
			else if (i == 3)
			{
				x = 38;
				y = 4;
				mapIndex = _miniMapVramIndex + 10;
			}

			VDP_setTileMapXY(PLAN_WINDOW, TILE_ATTR_FULL(PAL3, FALSE, FALSE, FALSE, mapIndex + 0), x, y);
			VDP_setTileMapXY(PLAN_WINDOW, TILE_ATTR_FULL(PAL3, FALSE, FALSE, FALSE, mapIndex + 1), x + 1, y);
			VDP_setTileMapXY(PLAN_WINDOW, TILE_ATTR_FULL(PAL3, FALSE, FALSE, FALSE, mapIndex + 4), x, y + 1);
			VDP_setTileMapXY(PLAN_WINDOW, TILE_ATTR_FULL(PAL3, FALSE, FALSE, FALSE, mapIndex + 5), x + 1, y + 1);
		}

		_previousMapCount = MapsCollected;
	}

	if (_previousMasksCollected != MasksCollected || _explosionHappened)
	{
		SetNumbers(MasksCollected, _masksCollected, 2);
		GenerateMiniMap();
		_previousMasksCollected = MasksCollected;
		_explosionHappened = FALSE;
	}

	if (_previousStepsTaken != StepsTaken)
	{
		SetNumbers(StepsTaken, _moves, 4);
		_previousStepsTaken = StepsTaken;
	}
}


void ExplosionHappenedUI()
{
	_explosionHappened = TRUE;
}


void ShowWhoopsUI()
{
	
	SPR_setFrame(_middleUIElement, 3);
	SPR_setVisibility(_middleUIElement, VISIBLE);			
}

void ShowGameOverUI()
{
	SPR_setFrame(_upperUIElement, 1);	
	SPR_setVisibility(_upperUIElement, VISIBLE);	
	SPR_setVisibility(_lowerUIElement, VISIBLE);
}

void ShowPausedUI()
{
	SPR_setFrame(_upperUIElement, 2);
	SPR_setVisibility(_upperUIElement, VISIBLE);
	SPR_setVisibility(_lowerUIElement, VISIBLE);
}

void HideUIElements()
{
	SPR_setVisibility(_upperUIElement, HIDDEN);
	SPR_setVisibility(_middleUIElement, HIDDEN);
	SPR_setVisibility(_lowerUIElement, HIDDEN);
}