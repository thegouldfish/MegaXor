#include <genesis.h>
#include "StateSelectLevel.h"
#include "../res/gfx.h"

#include "StatePlayGame.h"

#include "StateSelectLevel.h"

#include "PadHelper.h"
#include "GameMaps.h"
#include "Xor.h"

#include "SaveGame.h"

static s8 _previousY = -1;

#define PALETTE_ANIMATION_COUNT 10
static u32 _paletteAnimation[PALETTE_ANIMATION_COUNT];


static u8 _paletteAnimationFrameCount = 3;
static u8 _paletteAnimationFrameTimer = 0;

static s16 _currentPaletteAnimationFrame = 0;
static u8 _paletteAnimatingUp = TRUE;


static u8 _menuOption = 0;


static u8 _highLightColour = 7;



static void SetBackingHighlight(int x, int y, int size)
{
	if (_previousY != -1)
	{
		for (u8 i = 0; i < 28; i++)
		{
			VDP_setTileMapXY(PLAN_B, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, 5), 6 + i, _previousY);
		}
	}


	for (u8 i = 0; i < size; i++)
	{
		VDP_setTileMapXY(PLAN_B, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, 7), x + i, y);
	}

	_previousY = y;
}


static void SetBackingDullLight(int x, int y, int size)
{
	for (u8 i = 0; i < size; i++)
	{
		VDP_setTileMapXY(PLAN_B, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, 4), x + i, y);
	}
}

static void UpdateMenu()
{
	if (_menuOption == 1)
	{
		SetBackingHighlight(7, 19, 7);
	}

	if (_menuOption == 0)
	{
		if (PlayerOptions.SelectedGraphics == 0)
		{
			SetBackingHighlight(7, 7, 8);
		}
		else if (PlayerOptions.SelectedGraphics == 1)
		{
			SetBackingHighlight(20, 7, 5);
		}
		else if (PlayerOptions.SelectedGraphics == 2)
		{
			SetBackingHighlight(7, 8, 7);
		}
	}
	else
	{
		if (PlayerOptions.SelectedGraphics == 0)
		{
			SetBackingDullLight(7, 7, 8);
		}
		else if (PlayerOptions.SelectedGraphics == 1)
		{
			SetBackingDullLight(20, 7, 5);
		}
		else if (PlayerOptions.SelectedGraphics == 2)
		{
			SetBackingDullLight(7, 8, 7);
		}
	}
	

}


void StateOptionsMenu_Start()
{
	SYS_disableInts();

	SPR_reset();
	VDP_resetScreen();

	VDP_setPaletteColors(0, (u16*)palette_black, 64);

	VDP_drawImageEx(PLAN_B, &optionsMenu_Background, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, TILE_USERINDEX), 0, 0, FALSE, TRUE);



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


	VDP_drawText("Graphics set:", 5, 6);
	VDP_drawText("Original", 7, 7);
	VDP_drawText("Sonic", 20, 7);
	VDP_drawText("YouTube", 7, 8);


	VDP_drawText("Reset Save:", 5, 18);
	VDP_drawText("Press A", 7, 19);
	

	SYS_enableInts();


	UpdateMenu();

	u16 palette[16];
	memcpy(&palette[0], optionsMenu_Background.palette->data, 16 * 2);

	palette[_highLightColour] = _paletteAnimation[0];

	ResetPad(&Pad1);

	VDP_drawText("(D-Pad) Change : (C) Return", 7, 26);

	// fade in
	VDP_fadeIn(0, 15, palette, 10, FALSE);
}



void StateOptionsMenu_Update()
{

	if (Pad1.C == PAD_RELEASED)
	{
		WriteSaveOut();
		StateMachineChange(&GameMachineState, &StateLevelSelect);
	}


	if (Pad1.Down == PAD_RELEASED)
	{
		_menuOption++;
		if (_menuOption > 1)
		{
			_menuOption = 0;
		}
		UpdateMenu();
	}
	if (Pad1.Up == PAD_RELEASED)
	{
		_menuOption--;
		if (_menuOption < 0)
		{
			_menuOption = 1;
		}
		UpdateMenu();
	}

	if (_menuOption == 0)
	{
		if (Pad1.Left == PAD_RELEASED)
		{
			PlayerOptions.SelectedGraphics--;
			if (PlayerOptions.SelectedGraphics < 0)
			{
				PlayerOptions.SelectedGraphics = 2;
			}
			UpdateMenu();
		}
		else if (Pad1.Right == PAD_RELEASED)
		{
			PlayerOptions.SelectedGraphics++;
			if (PlayerOptions.SelectedGraphics > 2)
			{
				PlayerOptions.SelectedGraphics = 0;
			}
			UpdateMenu();
		}
	}
	if (_menuOption == 1)
	{
		if (Pad1.A == PAD_RELEASED)
		{
			for (u16 i = 0; i < MAP_COUNT; i++)
			{
				MapMoveCounts[i] = MAX_MOVE_COUNT;
			}

			WriteSaveOut();
			VDP_drawText("Cleared", 7, 11);
		}
	}
}



void StateOptionsMenu_End()
{

}




void StateOptionsMenu_VInt()
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

		VDP_setPaletteColor(_highLightColour, _paletteAnimation[_currentPaletteAnimationFrame]);
	}
}


const SimpleState StateOptionsMenu =
{
	StateOptionsMenu_Start,
	StateOptionsMenu_Update,
	StateOptionsMenu_End,
	StateOptionsMenu_VInt
};