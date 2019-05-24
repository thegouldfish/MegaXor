#include <genesis.h>
#include "GameMaps.h"
#include "Xor.h"


u16 MapMoveCounts[MAP_COUNT] = { MAX_MOVE_COUNT,MAX_MOVE_COUNT,MAX_MOVE_COUNT };

MapDefinition Level1=
{
	"Dots and Waves",
	{
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,1,1,1,1,1,1,1,1,1,1,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
		0,1,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,
		0,1,0,4,3,1,1,1,8,1,7,0,1,1,1,1,1,1,1,1,2,2,2,2,2,1,1,1,1,0,1,0,
		0,4,0,4,1,1,1,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,
		0,1,0,0,0,1,1,1,1,4,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,0,
		0,1,1,1,0,4,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,8,0,0,1,0,1,0,1,0,
		0,0,0,1,0,1,0,3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,0,1,0,
		0,2,2,1,0,1,1,4,0,0,0,0,2,0,0,0,2,2,3,0,0,0,0,0,1,0,1,0,1,0,1,0,
		0,4,3,1,0,1,0,0,3,1,1,1,1,1,4,3,1,4,1,3,4,1,1,0,1,0,1,2,1,0,1,0,
		0,4,3,1,0,1,0,4,3,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,4,0,1,0,1,0,1,0,
		0,4,3,1,0,1,0,1,3,1,0,4,0,4,1,1,1,1,1,1,1,0,1,0,0,0,1,0,1,0,1,0,
		0,4,3,1,0,3,0,1,3,1,0,1,0,0,0,2,2,2,2,0,1,0,1,0,4,0,1,0,1,0,1,0,
		0,3,3,1,0,8,0,1,3,1,1,1,1,1,1,1,1,1,1,0,1,0,1,0,1,0,1,0,1,0,5,0,
		0,0,0,1,0,3,0,1,0,1,0,1,0,4,0,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,
		0,4,0,1,0,3,0,1,0,1,0,1,0,0,0,4,4,3,1,3,1,1,1,3,1,1,1,3,1,3,1,0,
		0,1,0,1,0,1,0,1,0,1,0,4,0,4,0,6,4,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,
		0,1,0,1,0,1,0,1,0,1,0,0,0,1,0,0,0,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,
		0,4,0,1,0,1,0,1,1,1,0,1,1,1,1,1,1,1,1,0,1,0,1,0,1,0,1,0,1,0,1,0,
		0,1,0,1,0,1,0,1,0,0,0,1,0,0,0,0,0,2,0,0,1,0,1,0,1,0,4,0,1,0,1,0,
		0,1,0,1,0,1,0,1,0,4,3,1,0,1,1,1,1,1,1,1,4,0,1,0,1,0,0,0,1,0,1,0,
		0,4,0,1,0,1,0,1,1,4,3,1,0,2,3,2,0,0,0,0,0,0,1,0,1,0,4,0,1,0,1,0,
		0,1,0,1,2,1,0,1,0,4,3,1,2,2,3,3,0,4,1,1,1,1,1,1,1,0,1,0,1,0,1,0,
		0,1,0,0,0,1,0,1,0,0,0,0,0,3,2,2,0,0,0,0,0,0,0,0,1,0,1,0,1,0,1,0,
		0,1,0,4,0,3,0,1,1,1,2,4,0,2,2,3,4,0,4,2,1,1,1,1,1,0,1,0,4,0,1,0,
		0,1,0,4,0,3,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,1,0,0,0,1,0,
		0,1,0,1,0,4,0,4,4,3,3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,4,1,1,0,
		0,1,2,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,1,1,1,0,
		0,1,0,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,4,0,4,1,1,1,1,0,
		0,1,0,0,2,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,0,0,0,0,0,1,1,1,8,2,0,
		0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3,4,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	},
	{ 'D','D','D','D','D','D','D','D','D','D','D','D','D','D','L','L','U','D','D','L','L','R','R','D','R','R','L','D','R','L','L','L','L','L','L','L','L','L','L','L','L','L','L','L','L','L','L','L','L','L','L','L','L','L','L','L','L','L','U','U','U','U','U','U','U','U','U','U','U','U','U','U','U','D','D','D','D','D','D','D','D','D','D','D','D','R','R','U','U','U','D','D','D','L','L','D','D','D','R','R','R','R','R','R','R','R','R','R','R','R','R','R','R','R','R','R','R','R','R','R','R','R','R','R','R','R','R','U','U','U','U','U','U','U','U','U','U','U','U','U','U','U','U','U','U','U','U','U','U','U','U','U','U','U','U','U','L','L','L','L','L','L','L','L','L','L','L','L','L','L','L','L','L','L','L','L','L','L','L','L','L','L','L','L','L','D','D','D','D','D','R','R','D','D','D','U','L','L','D','D','D','D','D','U','U','U','U','U','R','R','D','D','D','D','D','D','D','D','D','D','D','D','D','D','R','R','D','D','D','D','U','U','U','U','U','U','U','U','U','U','U','U','U','U','U','U','U','U','U','U','U','D','D','D','R','R','U','D','L','L','U','U','U','U','L','L','U','D','R','R','R','R','U','R','R','L','D','D','R','R','R','R','R','R','R','R','R','R','R','R','R','R','R','D','D','L','L','L','L','L','L','R','D','D','L','L','R','R','U','U','R','R','R','R','R','R','D','D','D','U','U','U','L','L','R','U','U','R','R','R','D','D','D','D','R','R','D','D','D','D','D','D','D','D','D','D','D','D','D','D','D','U','U','U','U','U','U','U','U','U','U','U','U','U','U','U','L','L','D','D','D','D','D','D','D','D','D','D','U','U','U','U','L','L','U','U','U','D','D','D','D','D','D','D','D','D','D','D','D','D','D','R','R','U','U','U','U','U','D','D','D','D','D','L','L','L','L','L','L','L','L','L','L','L','L','L','L','L','D','D','R','R','R','R','R','R','R','R','R','R','R','R','R','L','L','L','L','L','L','L','L','L','L','L','L','L','U','U','R','R','R','R','R','R','R','R','R','R','R','R','R','U','U','L','L','L','L','L','L','R','R','R','R','R','R','U','U','L','L','L','L','L','L','L','R','R','R','R','R','U','U','U','U','U','U','U','U','U','U','U','D','D','D','D','U','U','U','U','U','U','L','L','R','R','D','D','D','D','D','D','L','L','U','U','U','U','L','L','L','L','L','L','L','R','R','R','R','R','R','R','D','D','D','D','D','D','D','D','D','L','L','L','L','L','L','D','L','R','D','L','D','R','R','D','R','L','L','U','L','U','L','L','U','U','U','U','R','R','U','U','D','D','R','R','R','R','R','U','U','U','U','U','L','L','D','D','D','U','U','D','L','R','U','U','L','L','L','D','U','L','L','L','R','D','D','D','U','U','U','U','U','D','D','L','L','D','D','D','D','D','L','L','D','D','D','D','D','D','R','R','D','D','L','L','R','R','U','U','R','R','L','L','L','L','U','U','U','R','R','D','U','U','D','L','L','U','U','U','U','U','U','U','U','U','U','U','D','D','D','D','D','D','D','D','R','R','U','U','U','U','U','U','U','U','U','R','R','R','R','R','L','L','L','L','L','D','D','D','D','R','R','R','R','R','R','R','D','D','D','L'}
};


MapDefinition Level2 =
{
	"Something Fishy",
	{ 
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,4,1,7,1,1,4,0,
		0,0,1,9,9,1,1,0,9,1,0,2,2,9,1,1,1,0,1,1,1,9,1,1,0,0,0,1,1,0,0,0,
		0,3,2,2,9,1,0,4,2,1,0,0,4,0,0,2,3,0,1,1,3,0,0,1,0,1,9,1,1,0,5,0,
		0,3,4,3,9,1,9,0,0,1,0,4,0,1,1,1,1,0,0,1,3,4,0,1,0,1,0,0,0,0,1,0,
		0,2,4,3,9,1,2,1,1,1,0,1,1,1,9,1,1,0,9,1,3,8,0,1,1,1,4,0,0,0,10,0,
		0,4,4,3,2,1,4,0,0,4,0,9,9,9,4,9,9,0,4,1,3,4,0,1,0,1,0,1,1,1,1,0,
		0,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,
		0,1,1,1,1,1,4,4,4,4,0,4,10,4,10,4,10,0,4,1,1,1,1,8,0,1,1,1,1,0,1,0,
		0,0,0,0,0,1,0,0,0,0,0,1,1,1,1,1,1,1,3,1,0,4,0,10,0,3,10,0,1,0,1,0,
		0,9,1,1,1,1,1,9,2,9,0,1,1,4,10,10,10,0,4,1,1,1,1,1,0,3,10,0,3,10,1,0,
		0,2,1,1,1,1,1,9,3,4,0,3,10,10,10,10,10,0,3,1,0,4,0,1,0,4,10,0,1,0,1,0,
		0,1,0,0,0,0,1,9,0,4,0,1,1,4,10,10,10,0,10,10,10,10,10,10,0,3,10,0,1,0,1,0,
		0,1,1,1,9,0,1,2,0,3,0,4,3,10,10,10,10,0,4,1,1,1,1,1,1,1,1,0,1,1,4,0,
		0,1,1,1,9,0,1,4,0,3,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,4,1,1,4,0,1,1,2,4,0,4,2,1,1,1,1,0,10,4,3,0,4,0,4,1,1,1,1,4,10,0,
		0,3,1,1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,4,4,10,0,4,0,4,1,1,1,0,10,10,0,
		0,3,1,1,1,1,1,1,3,9,0,4,0,10,1,0,4,0,4,10,1,1,1,0,4,0,4,1,0,4,4,0,
		0,3,1,1,9,1,1,1,2,2,0,1,1,1,1,3,10,0,10,4,10,0,1,1,3,0,0,10,0,1,1,0,
		0,3,1,1,4,1,0,1,3,4,0,0,0,1,1,1,1,0,4,1,1,1,1,0,0,0,1,1,1,1,1,0,
		0,1,1,1,1,1,0,1,0,0,0,4,1,1,4,10,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
		0,3,1,9,1,9,0,1,9,9,0,0,0,0,0,0,1,0,2,3,0,1,1,1,0,0,10,10,10,10,10,0,
		0,3,1,4,1,4,0,1,9,4,0,2,2,4,1,3,1,0,2,3,0,1,1,1,1,0,4,3,3,3,10,0,
		0,3,1,1,1,1,0,1,9,9,0,1,3,10,3,3,1,0,3,10,10,0,0,1,1,0,0,1,1,4,1,0,
		0,3,1,1,1,9,0,1,9,9,0,4,1,2,3,10,1,0,3,10,10,4,0,1,1,0,4,1,4,10,3,0,
		0,3,0,1,1,4,0,2,4,9,0,10,1,3,2,3,1,0,4,1,1,4,0,1,1,0,0,0,0,0,0,0,
		0,4,0,1,4,1,0,0,4,2,0,3,3,3,3,2,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,
		0,4,0,1,1,1,0,1,1,1,0,3,2,2,2,2,1,8,1,1,1,1,1,1,1,0,4,0,3,10,10,0,
		0,4,0,1,1,9,0,1,1,1,0,0,0,0,0,0,0,0,3,10,10,1,1,1,0,4,0,0,10,10,4,0,
		0,4,0,1,1,4,1,1,1,1,9,1,1,6,1,1,1,1,1,1,1,1,1,0,4,3,10,0,3,10,3,0,
		0,4,0,9,9,9,9,1,1,1,2,1,1,0,1,1,0,4,0,4,0,4,0,4,1,1,1,1,1,1,1,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	},
	{ 'D','D','D','D','D','D','D','D','D','D','L','L','U','U','U','U','U','L','L','L','D','D','D','D','D','L','L','L','L','L','L','L','R','U','U','U','U','D','L','R','R','R','D','U','U','U','R','R','L','L','D','D','D','U','L','L','L','U','U','D','L','L','L','U','D','L','L','U','D','L','L','U','D','D','D','D','D','U','U','U','D','D','R','D','U','U','U','R','D','D','U','R','R','R','D','D','D','D','L','L','L','L','L','R','R','R','D','D','D','D','L','U','L','L','U','D','R','R','D','D','L','L','R','R','R','U','R','U','D','R','U','U','D','D','D','D','D','D','D','D','D','D','L','L','L','L','U','U','U','L','R','U','D','U','R','U','D','L','D','D','D','D','R','R','R','R','R','R','R','R','R','R','R','R','U','R','R','D','U','R','R','R','R','L','L','D','U','R','R','D','D','D','D','L','L','L','L','L','L','L','R','U','D','R','U','U','D','D','R','R','R','R','R','U','U','U','U','L','L','L','L','L','L','U','U','U','U','L','L','U','U','R','R','R','R','R','R','R','R','L','D','U','R','D','D','D','L','L','L','D','L','R','U','R','D','U','L','U','L','R','D','R','R','R','U','U','U','U','U','U','L','D','D','L','L','U','U','L','U','U','L','L','D','D','D','L','L','U','U','U','D','D','L','L','U','L','U','D','L','D','U','R','D','D','D','L','D','R','D','L','R','D','L','D','D','U','U','R','D','R','D','D','R','U','D','L','L','L','R','R','U','U','L','U','U','U','R','R','R','R','R','R','R','U','R','R','L','U','U','U','U','R','R','D','D','R','D','D','D','L','L','L','L','L','L','L','D','D','D','D','D','R','D','D','L','L','D','D','L','D','U','L','L','D','U','L','L','D','U','L','L','L','x','L','L','R','R','R','R','R','L','L','L','D','D','L','L','D','D','R','L','L','L','U','U','U','U','L','L','R','R','D','L','L','L','D','D','R','D','D','U','U','L','L','D','D','L','R','U','U','U','U','U','L','L','L','D','D','D','L','L','L','D','L','L','U','D','R','R','R','D','U','U','R','R','U','U','U','L','L','L','R','D','L','L','D','U','U','L','L','L','D','D','D','D','D','U','U','U','L','L','R','R','D','D','L','L','L','L','D','R','L','U','U','U','L','L','D','L','D','D','L','D','D','R','R','R','R','R','R','R','R','L','L','L','L','D','D','L','L','R','R','R','D','D','D','D','D','R','U','D','R','R','U','U','U','D','D','D','L','L','L','U','U','R','L','U','U','U','R','R','D','R','L','L','L','L','L','L','L','L','R','L','D','D','R','R','D','D','R','L','L','D','D','D','D','D','D','U','L','D','D','D','D','D','D','D','D','D','D','U','U','U','U','U','U','U','U','U','U','U','U','U','U','U','R','D','D','D','D','D','D','D','D','D','R','D','D','D','D','D','R','R','L','R','R','R','R','R','L','L','L','L','L','U','U','D','D','R','R','R','R','D','R','R','R','R','U','L','L','L','L','L','L','L','L','R','R','R','R','U','U','U','U','L','U','U','R','U','R','L','L','U','U','U','U','L','L','D','D','L','L','L','D','D','D','D','R','D','D','D','D','D','R','R','R','R','R','R','L','L','L','L','L','U','U','U','U','R','L','U','U','U','L','R','R','L','U','U','U','R','U','R','R','R','D','R','L','L','D','D','D','D','D','R','D','D','D','D','R','D','R','R','R','R' }
};

MapDefinition level3 =
{
	"Chicken Supreme",
	{
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,1,1,1,1,1,1,1,0,9,9,9,4,0,1,1,1,1,1,0,1,4,0,1,4,10,0,4,4,4,9,0,
		0,1,0,1,1,9,0,1,0,4,4,4,4,0,1,0,4,10,1,0,1,9,4,3,10,10,0,1,1,9,10,0,
		0,1,3,1,0,4,10,1,0,4,0,4,10,0,1,0,0,0,1,0,1,2,0,1,1,0,0,1,9,10,1,0,
		0,1,3,1,1,4,4,1,0,4,4,4,10,0,1,0,9,9,1,1,1,0,1,1,1,1,0,4,10,1,1,0,
		0,1,3,1,0,9,0,1,0,1,1,1,1,1,1,0,9,10,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
		0,1,3,1,4,4,9,10,0,0,0,0,0,0,0,0,4,1,1,0,1,0,1,1,9,10,0,1,1,3,10,0,
		0,1,3,1,1,3,10,2,0,0,0,7,1,1,1,0,4,1,1,0,4,0,4,4,10,10,0,4,1,1,1,0,
		0,1,3,1,1,1,1,1,0,0,9,10,0,0,2,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,
		0,1,0,1,1,1,1,1,0,4,4,1,1,0,4,0,4,9,9,1,9,9,9,9,3,10,10,10,4,0,1,0,
		0,1,1,1,1,9,10,1,0,0,0,1,1,0,4,2,9,9,4,1,2,2,2,2,1,3,10,10,10,0,1,0,
		0,3,0,0,0,9,1,1,0,4,4,1,1,0,0,0,9,2,4,1,3,3,4,3,9,1,3,10,10,0,1,0,
		0,3,2,2,0,4,1,1,0,0,0,0,8,8,8,0,4,1,1,2,2,2,3,2,2,1,1,3,10,0,4,0,
		0,0,0,3,0,0,0,1,0,0,0,0,0,0,8,0,0,0,3,0,0,0,0,0,0,3,0,0,0,0,0,0,
		0,4,1,1,9,1,1,1,0,4,4,4,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
		0,4,10,10,10,1,9,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,4,1,9,10,1,9,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
		0,3,10,10,1,1,9,1,0,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,10,10,10,10,10,1,0,
		0,0,1,1,1,1,4,4,0,1,0,1,1,1,1,1,1,0,4,0,1,9,1,1,0,4,1,1,1,9,1,0,
		0,1,1,1,1,0,0,0,0,1,1,1,0,4,9,0,10,0,1,1,1,2,9,10,0,0,1,4,1,9,1,0,
		0,1,9,1,1,4,4,4,0,0,0,0,0,4,10,0,5,0,1,1,1,1,9,1,0,1,1,1,1,9,1,0,
		0,1,0,0,0,0,0,0,0,0,0,0,0,4,9,0,1,0,1,1,4,10,10,10,0,4,0,1,1,9,1,0,
		0,1,1,1,1,1,1,9,0,1,1,1,0,4,10,0,10,0,4,4,0,4,4,4,0,4,4,0,4,9,1,0,
		0,4,0,0,1,1,1,4,0,1,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,
		0,4,0,1,1,1,1,1,0,1,0,0,0,0,0,0,0,0,1,4,10,0,1,1,1,1,1,1,1,1,1,0,
		0,4,0,1,9,10,10,1,0,1,1,1,1,1,1,1,1,1,1,1,1,0,1,9,1,0,1,1,9,1,9,0,
		0,4,0,1,9,9,1,1,0,1,0,10,10,10,10,0,1,1,1,9,10,0,1,4,10,0,1,1,9,4,9,0,
		0,4,0,1,9,0,1,1,4,1,0,3,4,0,3,0,1,1,9,10,3,0,1,0,1,0,1,0,0,0,9,0,
		0,4,0,4,4,0,4,4,0,1,0,4,1,1,1,0,4,10,10,0,0,0,1,1,1,0,3,4,4,4,10,0,
		0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,10,10,0,3,4,6,4,10,0,
		0,4,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,3,4,4,4,10,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	},
	{ 'U','U','L','L','L','D','D','D','D','D','L','L','U','L','L','D','D','D','R','R','D','U','R','R','R','D','D','D','L','L','L','R','U','D','R','R','U','U','U','R','R','R','R','R','U','D','R','D','L','U','L','L','L','D','D','D','U','U','U','L','L','L','L','L','L','L','D','D','L','L','L','D','R','U','U','U','U','U','L','D','D','D','U','U','L','L','L','D','D','D','D','R','L','U','U','U','R','U','U','U','L','L','L','U','U','U','R','R','R','D','R','R','R','L','L','L','U','U','R','U','U','U','U','R','R','U','U','U','U','U','L','L','L','D','L','L','L','D','D','R','R','D','D','L','L','D','D','U','D','R','U','R','U','U','U','L','L','U','U','R','R','R','U','R','U','U','D','U','U','D','L','U','D','R','U','U','U','U','D','R','R','D','D','L','L','D','L','R','R','R','D','D','D','D','D','L','L','R','R','D','D','L','L','D','D','D','D','R','R','L','L','L','L','L','D','L','D','D','D','D','D','D','D','D','D','D','D','R','R','R','R','R','R','R','R','R','R','R','R','R','R','R','R','R','R','R','R','R','R','R','U','U','U','U','L','R','U','U','R','R','R','R','R','R','U','U','U','L','L','D','U','U','L','L','L','D','D','R','L','U','U','R','R','U','U','L','L','R','R','R','R','R','U','U','L','L','L','L','L','L','L','L','L','L','L','R','D','D','D','D','L','L','U','U','D','D','D','D','R','U','U','R','D','U','U','U','R','R','R','D','D','L','L','L','R','R','R','D','D','L','L','R','U','U','U','U','U','U','R','R','R','R','R','R','R','R','D','D','D','D','D','D','D','D','L','L','L','L','D','D','U','U','R','R','R','D','D','U','U','L','L','L','D','D','D','D','D','D','R','R','L','U','U','R','R','D','D','U','x','D','D','R','D','D','L','L','L','R','R','R','U','U','L','D','U','L','L','R','R','R','D','D','D','R','R','D','D','L','L','L','L','L','R','R','R','R','R','R','R','R','R','R','L','U','U','L','L','R','R','U','L','R','U','L','U','L','D','L','L','U','D','R','R','R','R','D','D','U','D','R','R','R','U','R','D','R','R','L','U','R','U','L','L','L','L','L','R','R','R','R','R','U','D','U','R','R','R','R','L','L','L','L','L','L','L','L','U','U','U','U','R','R','R','D','D','U','U','R','R','D','U','L','L','L','D','D','U','U','R','R','R','R','R','R','D','D','L','R','R','R','U','D','D','D','D','D','D','U','U','U','U','U','U','U','L','L','L','U','D','R','R','R','U','U','U','U','L','L','L','R','R','D','D','D','D','L','L','L','L','L','L','L','L','L','L','U','R','U','R','L','U','U','R','D','R','L','L','D','D','D','R','R','R','U','U','U','D','R','U','U','D','D','D','D','L','L','L','L','L','U','L','D','D','D','L','L','U','D','R','R','U','U','U','U','U','U','L','L','D','U','L','L','D','D','D','D','L','L','L','L','U','L','U','U','R','R','L','U','R','R','D','L','L','R','R','L','D','D','x','L' }
};


const MapDefinition* GameMaps[MAP_COUNT] =
{
	&Level1,
	&Level2,
	&level3
};