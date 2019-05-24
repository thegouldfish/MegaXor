#include <genesis.h>

#include "Xor.h"

#include "StatePlayGame.h"
#include "StateSelectLevel.h"

#include "../res/sound.h"

StateMachine GameMachineState;
Pad Pad1;


int main()
{
	
    SYS_disableInts();
    VDP_setScreenWidth320();	

	// On PAL show the extra pixels
	if (IS_PALSYSTEM)
	{
		VDP_setScreenHeight240();
	}
	else
	{
		VDP_setScreenHeight224();
	}

	VDP_setPlanSize(64, 64);
	

	
	// Using a 64 x 64 plan and a window, so need to setup custom addresses
	VDP_setHScrollTableAddress(0x9400);
	VDP_setSpriteListAddress(0x9800);

	VDP_setWindowAddress(0xA800);

	VDP_setAPlanAddress(0xC000);
	VDP_setBPlanAddress(0xE000);
	

	SPR_init(0, 0, 0);
	//XGM_startPlay(xgm_1);
	
    VDP_setPaletteColors(0, (u16*) palette_black, 64);

	// Set the window location to be on the right hand side of the screen.
	VDP_setWindowHPos(TRUE, 18);
	VDP_setWindowVPos(FALSE, 0);

    SYS_enableInts();

	SetupPad(&Pad1, JOY_1);		

	StateMachineStart(&GameMachineState, &StateLevelSelect);
    while(TRUE)
    {
		UpdatePad(&Pad1);
		StateMachineUpdate(&GameMachineState);

        SPR_update();
        VDP_waitVSync();
		//KLog_F1("fps", getFPS_f());
    }

    return 0;
}