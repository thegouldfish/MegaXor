#ifndef _FISH_LOGIC_H_
#define _FISH_LOGIC_H_
#include <genesis.h>

#define LOGIC_ID_FISH 1

void FishSetup();
u8 FishPushLeft(u16 x, u16 y);
u8 FishPushRight(u16 x, u16 y);
u8 FishUpdateMovement();
u8 FishUpdateLogic();
void FishFinishMovement();


#endif