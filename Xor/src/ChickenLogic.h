#ifndef _CHICKEN_LOGIC_H_
#define _CHICKEN_LOGIC_H_

#include <genesis.h>

void ChickenMakeSprite();
void ChickenSetup();

u8 ChickenPushUp(u16 x, u16 y);
u8 ChickenPushDown(u16 x, u16 y);
u8 ChickenUpdateMovement();
u8 ChickenUpdateLogic();
void ChickenFinishMovement();


#endif