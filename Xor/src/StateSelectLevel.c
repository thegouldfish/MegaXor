#include <genesis.h>
#include "StateSelectLevel.h"
#include "../res/gfx.h"

#include "StatePlayGame.h"

#include "PadHelper.h"
#include "GameMaps.h"
#include "Xor.h"

static s8 _previousSelection = 0;
static s8 _currentSelection = 0;

#define PALETTE_ANIMATION_COUNT 10
static u32 _paletteAnimation[PALETTE_ANIMATION_COUNT];


static u8 _paletteAnimationFrameCount = 3;
static u8 _paletteAnimationFrameTimer = 0;

static s16 _currentPaletteAnimationFrame = 0;
static u8 _paletteAnimatingUp = TRUE;


void SetBackingHighlight()
{
	for (u8 i = 0; i < 28; i++)
	{
		VDP_setTileMapXY(PLAN_B, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, 13), 6 + i, _previousSelection + 8);
		VDP_setTileMapXY(PLAN_B, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, 1) , 6 + i, _currentSelection  + 8);
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

	SPR_reset();
	VDP_resetScreen();

	VDP_setPaletteColors(0, (u16*)palette_black, 64);


	VDP_drawImageEx(PLAN_B, &levelSelect_Background, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, TILE_USERINDEX), 0, 0, FALSE, TRUE);

	GameInteractionState = GAME_INTERACTION_STATE_NORMAL;


	// Generate the colour shades from dark blue to light
	// This is used for the highlight palette animation
	fix32 start = FIX32(0x000072);
	fix32 end = FIX32(0x0000FF);
	fix32 range = fix32Sub(end, start);
	fix32 div = fix32Div(range, FIX32(PALETTE_ANIMATION_COUNT));
	
	for (int i = 0; i < PALETTE_ANIMATION_COUNT; i++)
	{
		_paletteAnimation[i] = RGB24_TO_VDPCOLOR(fix32ToInt(start));
		start = fix32Add(start, div);
	}

	// Draw the menu text to the screen
	char text[30];
	for (u8 i = 0; i < MAP_COUNT; i++)
	{
		memset(text, (int)' ', 30);
		text[29] = 0;
		SetNumberCharsEx(i + 1, text, 3, '0');
		text[2] = ' ';

		u16 len = strlen(GameMaps[i]->MapName);
		memcpy(&text[3], GameMaps[i]->MapName, len);

		text[22] = '-';
		SetNumberCharsEx(MapMoveCounts[i], &text[24], 5, '0');


		VDP_drawText(text, 6, 8 + i);
	}

	SetBackingHighlight();


	VDP_drawText(Version, 32, 26);

	SYS_enableInts();



	u16 palette[16];
	memcpy(&palette[0], levelSelect_Background.palette->data, 16 * 2);

	ResetPad(&Pad1);


	// fade in
	VDP_fadeIn(0, 15, palette, 10, FALSE);
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
			GameInteractionState = GAME_INTERACTION_STATE_TEST_REPLAY;
		}

		SelectedLevel = _currentSelection;
		StateMachineChange(&GameMachineState, &StatePlayGame);
	}	
}



void StateLevelSelect_End()
{

}




void StateLevelSelect_VInt()
{
	_paletteAnimationFrameTimer++;

	if (_paletteAnimationFrameTimer > _paletteAnimationFrameCount)
	{
		if (_paletteAnimatingUp)
		{
			_currentPaletteAnimationFrame++;
			if (_currentPaletteAnimationFrame >= PALETTE_ANIMATION_COUNT)
			{
				_currentPaletteAnimationFrame = PALETTE_ANIMATION_COUNT - 2;
				_paletteAnimatingUp = FALSE;
			}
		}
		else
		{
			_currentPaletteAnimationFrame--;
			if (_currentPaletteAnimationFrame < 0)
			{
				_currentPaletteAnimationFrame = 1;
				_paletteAnimatingUp = TRUE;
			}
		}

		_paletteAnimationFrameTimer = 0;

		VDP_setPaletteColor(1, _paletteAnimation[_currentPaletteAnimationFrame]);
	}
}


const SimpleState StateLevelSelect =
{
	StateLevelSelect_Start,
	StateLevelSelect_Update,
	StateLevelSelect_End,
	StateLevelSelect_VInt
};