#include "TileSets.h"

#include <genesis.h>

#include "../res/TileSet1.h"
#include "../res/SonicSet1.h"
#include "../res/YTSet1.h"




// TileSet 1
const GraphicsDefinition wallSet1 =			{ &Wall_Set1_tile,			NULL,					0, 0,  0,  0, CONST_PLAN_B, FALSE};
const GraphicsDefinition groundSet1 =		{ &Ground_Set1_tile,		NULL,					0, 0,  0,  0, CONST_PLAN_B, FALSE };
const GraphicsDefinition maskSet1 =			{ &Mask_Set1_tile,			&Mask_Set1_sprite,		0, 0,  0,  0, CONST_PLAN_A, FALSE };
const GraphicsDefinition switchSet1 =		{ &Switch_Set1_tile,		NULL,					0, 0,  0,  0, CONST_PLAN_A, FALSE };
const GraphicsDefinition hForceFieldSet1 =	{ &HForceField_Set1_tile,	NULL,					4, 16, 13, 0, CONST_PLAN_A, FALSE };
const GraphicsDefinition vForceFieldSet1 =	{ &VForceField_Set1_tile,	NULL,					4, 16, 13, 0, CONST_PLAN_A, FALSE };
const GraphicsDefinition teleporterSet1 =	{ &Teleporter_Set1_tile,	NULL,					4, 16, 13, 0, CONST_PLAN_A, FALSE };
const GraphicsDefinition explosionSet1 =	{ &Explosion_Set1_tile,		NULL,					5, 5,  4,  0, CONST_PLAN_A, TRUE };
const GraphicsDefinition doorSet1 =			{ &Door_Set1_tile,			NULL,					0, 0,  0,  1, CONST_PLAN_A, FALSE };
const GraphicsDefinition mapSet1 =			{ &Map_Set1_tile,			NULL,					0, 0,  0,  1, CONST_PLAN_A, FALSE };
const GraphicsDefinition chickenSet1 =		{ &Chicken_Set1_tile,		&Chicken_Set1_sprite,	0, 0,  0,  1, CONST_PLAN_A, FALSE };
const GraphicsDefinition fishSet1 =			{ &Fish_Set1_tile,			&Fish_Set1_sprite,		0, 0,  0,  1, CONST_PLAN_A, FALSE };
const GraphicsDefinition hBombSet1 =		{ &HBomb_Set1_tile,			&HBomb_Set1_sprite,		0, 0,  0,  1, CONST_PLAN_A, FALSE };
const GraphicsDefinition vBombSet1 =		{ &VBomb_Set1_tile,			&VBomb_Set1_sprite,		0, 0,  0,  1, CONST_PLAN_A, FALSE };
const GraphicsDefinition dollSet1 =			{ &Doll_Set1_tile,			&Doll_Set1_sprite,		0, 0,  0,  1, CONST_PLAN_A, FALSE };
const GraphicsDefinition questorSet1 =		{ &Questor_Set1_tile,		&Questor_Set1_sprite,	0, 0,  0,  2, CONST_PLAN_A, FALSE };
const GraphicsDefinition magusSet1 =		{ &Magus_Set1_tile,			&Magus_Set1_sprite,		0, 0,  0,  2, CONST_PLAN_A, FALSE };

const TileGraphics tileGraphicsSet1 =
{
	{
		&wallSet1,
		&groundSet1,
		&hForceFieldSet1,
		&vForceFieldSet1,
		&maskSet1,
		&magusSet1,
		&doorSet1,
		&questorSet1,
		&mapSet1,
		&fishSet1,
		&chickenSet1,
		&hBombSet1,
		&vBombSet1,
		&explosionSet1,
		&dollSet1,
		&switchSet1,
		&teleporterSet1
	},
	"Tile Set 1",
	{0, 6, 5}
};













const GraphicsDefinition wallSonicSet1 = { &Wall_SonicSet1_tile,			NULL,					0, 0,  0,  0, CONST_PLAN_B, FALSE };
const GraphicsDefinition groundSonicSet1 = { &Ground_SonicSet1_tile,		NULL,					0, 0,  0,  0, CONST_PLAN_B, FALSE };
const GraphicsDefinition maskSonicSet1 = { &Mask_SonicSet1_tile,			&Mask_SonicSet1_sprite,		4, 16,  13,  0, CONST_PLAN_A, FALSE };
const GraphicsDefinition switchSonicSet1 = { &Switch_SonicSet1_tile,		NULL,					4, 10,  7,  0, CONST_PLAN_A, FALSE };
const GraphicsDefinition hForceFieldSonicSet1 = { &HForceField_SonicSet1_tile,	NULL,					4, 16, 13, 0, CONST_PLAN_A, FALSE };
const GraphicsDefinition vForceFieldSonicSet1 = { &VForceField_SonicSet1_tile,	NULL,					4, 16, 13, 0, CONST_PLAN_A, FALSE };
const GraphicsDefinition teleporterSonicSet1 = { &Teleporter_SonicSet1_tile,	NULL,					4, 16, 13, 0, CONST_PLAN_A, FALSE };
const GraphicsDefinition explosionSonicSet1 = { &Explosion_SonicSet1_tile,		NULL,					5, 5,  4,  0, CONST_PLAN_A, TRUE };
const GraphicsDefinition doorSonicSet1 = { &Door_SonicSet1_tile,			NULL,					0, 0,  0,  1, CONST_PLAN_A, FALSE };
const GraphicsDefinition mapSonicSet1 = { &Map_SonicSet1_tile,			NULL,					0, 0,  0,  1, CONST_PLAN_A, FALSE };
const GraphicsDefinition chickenSonicSet1 = { &Chicken_SonicSet1_tile,		&Chicken_SonicSet1_sprite,	0, 0,  0,  1, CONST_PLAN_A, FALSE };
const GraphicsDefinition fishSonicSet1 = { &Fish_SonicSet1_tile,			&Fish_SonicSet1_sprite,		0, 0,  0,  1, CONST_PLAN_A, FALSE };
const GraphicsDefinition hBombSonicSet1 = { &HBomb_SonicSet1_tile,			&HBomb_SonicSet1_sprite,		0, 0,  0,  1, CONST_PLAN_A, FALSE };
const GraphicsDefinition vBombSonicSet1 = { &VBomb_SonicSet1_tile,			&VBomb_SonicSet1_sprite,		0, 0,  0,  1, CONST_PLAN_A, FALSE };
const GraphicsDefinition dollSonicSet1 = { &Doll_SonicSet1_tile,			&Doll_SonicSet1_sprite,		0, 0,  0,  1, CONST_PLAN_A, FALSE };
const GraphicsDefinition magusSonicSet1 = { &Magus_SonicSet1_tile,			&Magus_SonicSet1_sprite,		0, 0,  0,  2, CONST_PLAN_A, FALSE };
const GraphicsDefinition questorSonicSet1 = { &Questor_SonicSet1_tile,		&Questor_SonicSet1_sprite,	0, 0,  0,  2, CONST_PLAN_A, FALSE };


const TileGraphics tileGraphicsSonicSet1 =
{
	{
		&wallSonicSet1,
		&groundSonicSet1,
		&hForceFieldSonicSet1,
		&vForceFieldSonicSet1,
		&maskSonicSet1,
		&magusSonicSet1,
		&doorSonicSet1,
		&questorSonicSet1,
		&mapSonicSet1,
		&fishSonicSet1,
		&chickenSonicSet1,
		&hBombSonicSet1,
		&vBombSonicSet1,
		&explosionSonicSet1,
		&dollSonicSet1,
		&switchSonicSet1,
		&teleporterSonicSet1
	},
	"Sonic Set 1",
	{ 0, 6, 5 }
};





const GraphicsDefinition wallYTSet1 = { &Wall_YTSet1_tile,			NULL,						0, 0,  0,  0, CONST_PLAN_B, FALSE };
const GraphicsDefinition groundYTSet1 = { &Ground_YTSet1_tile,		NULL,						0, 0,  0,  0, CONST_PLAN_B, FALSE };
const GraphicsDefinition maskYTSet1 = { &Mask_YTSet1_tile,			&Mask_YTSet1_sprite,		0, 16,  13,  2, CONST_PLAN_A, FALSE };
const GraphicsDefinition switchYTSet1 = { &Switch_YTSet1_tile,		NULL,						0, 10,  7,  2, CONST_PLAN_A, FALSE };
const GraphicsDefinition hForceFieldYTSet1 = { &HForceField_YTSet1_tile,	NULL,				4, 16, 13, 0, CONST_PLAN_A, FALSE };
const GraphicsDefinition vForceFieldYTSet1 = { &VForceField_YTSet1_tile,	NULL,				4, 16, 13, 0, CONST_PLAN_A, FALSE };
const GraphicsDefinition teleporterYTSet1 = { &Teleporter_YTSet1_tile,	NULL,					4, 16, 13, 0, CONST_PLAN_A, FALSE };
const GraphicsDefinition explosionYTSet1 = { &Explosion_YTSet1_tile,		NULL,				5, 5,  4,  0, CONST_PLAN_A, TRUE };
const GraphicsDefinition doorYTSet1 = { &Door_YTSet1_tile,			NULL,						0, 0,  0,  1, CONST_PLAN_A, FALSE };
const GraphicsDefinition mapYTSet1 = { &Map_YTSet1_tile,			NULL,						0, 0,  0,  0, CONST_PLAN_A, FALSE };
const GraphicsDefinition chickenYTSet1 = { &Chicken_YTSet1_tile,		&Chicken_YTSet1_sprite,	0, 0,  0,  1, CONST_PLAN_A, FALSE };
const GraphicsDefinition fishYTSet1 = { &Fish_YTSet1_tile,			&Fish_YTSet1_sprite,		0, 0,  0,  2, CONST_PLAN_A, FALSE };
const GraphicsDefinition hBombYTSet1 = { &HBomb_YTSet1_tile,			&HBomb_YTSet1_sprite,	0, 0,  0,  1, CONST_PLAN_A, FALSE };
const GraphicsDefinition vBombYTSet1 = { &VBomb_YTSet1_tile,			&VBomb_YTSet1_sprite,	0, 0,  0,  1, CONST_PLAN_A, FALSE };
const GraphicsDefinition dollYTSet1 = { &Doll_YTSet1_tile,			&Doll_YTSet1_sprite,		0, 0,  0,  1, CONST_PLAN_A, FALSE };
const GraphicsDefinition magusYTSet1 = { &Magus_YTSet1_tile,			&Magus_YTSet1_sprite,	0, 0,  0,  1, CONST_PLAN_A, FALSE };
const GraphicsDefinition questorYTSet1 = { &Questor_YTSet1_tile,		&Questor_YTSet1_sprite,	0, 0,  0,  1, CONST_PLAN_A, FALSE };


const TileGraphics tileGraphicsYouTubeSet1 =
{
	{
		&wallYTSet1,
		&groundYTSet1,
		&hForceFieldYTSet1,
		&vForceFieldYTSet1,
		&maskYTSet1,
		&magusYTSet1,
		&doorYTSet1,
		&questorYTSet1,
		&mapYTSet1,
		&fishYTSet1,
		&chickenYTSet1,
		&hBombYTSet1,
		&vBombYTSet1,
		&explosionYTSet1,
		&dollYTSet1,
		&switchYTSet1,
		&teleporterYTSet1
	},
	"Youtube Set 1",
	{ 0, 5, 4 }
};




const TileGraphics* SelectedGraphicsSet = NULL;