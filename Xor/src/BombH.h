#ifndef _BOMB_H_
#define _BOMB_H_

#include <genesis.h>

void BombHMakeSprite();
void BombHSetup();

u8 BombHPushLeft(u16 x, u16 y);
u8 BombHPushRight(u16 x, u16 y);
u8 BombHUpdateMovement();
u8 BombHUpdateLogic();
void BombHFinishMovement();

void TriggerBombH(u16 x, u16 y);



#endif