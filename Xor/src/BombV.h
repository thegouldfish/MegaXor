#ifndef _BOMB_V_
#define _BOMB_V_

#include <genesis.h>

void BombVSetup();

u8 BombVPushUp(u16 x, u16 y);
u8 BombVPushDown(u16 x, u16 y);


u8 BombVUpdateMovement();
u8 BombVUpdateLogic();
void BombVFinishMovement();

void TriggerBombV(u16 x, u16 y);



#endif