#include <genesis.h>
#include "StateSelectLevel.h"
#include "../res/gfx.h"

#include "StatePlayGame.h"

#include "PadHelper.h"
#include "GameMaps.h"
#include "Globals.h"
#include "Xor.h"

static s8 _previousSelection = 0;
static s8 _currentSelection = 0;


void SetBackingHighlight()
{
	for (u8 i = 0; i < 29; i++)
	{
		VDP_setTileMapXY(PLAN_B, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, 13), 6 + i, _previousSelection + 8);
		VDP_setTileMapXY(PLAN_B, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, 1), 6 + i, _currentSelection + 8);
	}
}

void SetNumberCharsEx(u32 number, char* buff, int size, char defaultChar)
{
	for (int i = 0; i < size - 1; i++)
	{
		buff[i] = defaultChar;
	}
	buff[size - 1] = 0;


	int i = size - 2;
	do
	{
		if (number < 10)
		{
			buff[i] = number + 48;
		}
		else
		{
			buff[i] = (number % 10) + 48;
		}

		number /= 10;
		i--;
	} while (number > 0);
}


void StateLevelSelect_Start()
{
	SYS_disableInts();
	VDP_setPaletteColors(0, (u16*)palette_black, 64);

	SPR_reset();
	VDP_resetScreen();
	
	VDP_drawImage(PLAN_B, &levelSelect_Background, 0, 0);

	PlaySolution = FALSE;



	char text[30];
	for (u8 i = 0; i < MAP_COUNT; i++)
	{
		memset(text, (int)' ', 30);
		text[29] = 0;
		SetNumberCharsEx(i, text, 3, '0');
		text[2] = ' ';

		u16 len = strlen(GameMaps[i]->MapName);
		memcpy(&text[3], GameMaps[i]->MapName, len);

		text[22] = '-';
		SetNumberCharsEx(MapMoveCounts[i], &text[24], 5, '0');


		VDP_drawText(text, 6, 8 + i);
	}

	SetBackingHighlight();


	VDP_drawText(Version, 32, 28);

	SYS_enableInts();
}

void StateLevelSelect_Update()
{

	if (Pad1.Down == PAD_RELEASED)
	{		
		_previousSelection = _currentSelection;
		_currentSelection++;
		if (_currentSelection >= MAP_COUNT)
		{
			_currentSelection = 0;
		}

		SetBackingHighlight();
	}
	else if (Pad1.Up == PAD_RELEASED)
	{
		_previousSelection = _currentSelection;
		_currentSelection--;
		if (_currentSelection < 0)
		{
			_currentSelection = MAP_COUNT -1;
		}

		SetBackingHighlight();
	}
	else if (Pad1.START == PAD_RELEASED || Pad1.C == PAD_RELEASED || Pad1.A == PAD_RELEASED)
	{
		if (Pad1.C == PAD_RELEASED)
		{
			PlaySolution = TRUE;
		}

		SelectedLevel = _currentSelection;
		StateMachineChange(&GameMachineState, &StatePlayGame);
	}	
}

void StateLevelSelect_End()
{

}



const SimpleState StateLevelSelect =
{
	StateLevelSelect_Start,
	StateLevelSelect_Update,
	StateLevelSelect_End
};