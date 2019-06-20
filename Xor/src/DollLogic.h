#ifndef _DOLL_LOGIC_
#define _DOLL_LOGIC_

#include <genesis.h>

void DollSetup();

u8 DollPushUp(u16 x, u16 y);
u8 DollPushDown(u16 x, u16 y);
u8 DollPushLeft(u16 x, u16 y);
u8 DollPushRight(u16 x, u16 y);


u8 DollUpdateMovement();
u8 DollUpdateLogic();
void DollFinishMovement();

#endif