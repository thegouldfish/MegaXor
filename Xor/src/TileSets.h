#ifndef _TILE_SETS_H_
#define _TILE_SETS_H_

#include <genesis.h>
#include "TileDefinitions.h"


typedef struct
{
	const Image* Tile;
	const SpriteDefinition* Sprite;
	const u8 AnimationCount;	
	const u8 AnimationWaitFramesNTSC;
	const u8 AnimationWaitFramesPAL;
	const u8 Palette;
	const u8 Plan;
	const u8 Priority;
}GraphicsDefinition;

typedef struct
{
	const GraphicsDefinition* Wall;
	const GraphicsDefinition* Ground;
	const GraphicsDefinition* Mask;
	const GraphicsDefinition* HForceField;
	const GraphicsDefinition* VForceField;
	const GraphicsDefinition* Explosion;
	const GraphicsDefinition* Switch;
	const GraphicsDefinition* Teleporter;
	const GraphicsDefinition* Door;
	const GraphicsDefinition* Map;
	const GraphicsDefinition* Fish;
	const GraphicsDefinition* Chicken;
	const GraphicsDefinition* HBomb;
	const GraphicsDefinition* VBomb;
	const GraphicsDefinition* Doll;
	const GraphicsDefinition* Magus;
	const GraphicsDefinition* Questor;

	Palette* Palettes[3];
}GraphicsSet;



typedef struct
{
	const GraphicsDefinition* Definitions[TILECOUNT - 1];
	const char* Name;
	const u8 PaletteIndexes[3];
}TileGraphics;


typedef struct
{
	u8 Num;
	TileGraphics* GraphicSets[];
}GraphicSetCollection;


extern const TileGraphics tileGraphicsSet1;

extern const TileGraphics tileGraphicsSonicSet1;

extern const TileGraphics tileGraphicsYouTubeSet1;

const extern TileGraphics* SelectedGraphicsSet;



#endif
