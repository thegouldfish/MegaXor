#include <genesis.h>

#include "TileLoadTestState.h"
#include "TileLoading.h"
#include "Xor.h"

void TileLoadTestState_Start()
{
	LoadCurrentTileSet();

	VDP_fadeIn(0, (4 * 16) - 1, GamePalette, 10, FALSE);
}

void TileLoadTestState_Update()
{
	TickTiles();
}

void TileLoadTestState_End()
{

}



const SimpleState TileLoadTestState =
{
	TileLoadTestState_Start,
	TileLoadTestState_Update,
	TileLoadTestState_End
};