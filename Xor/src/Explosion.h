#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_
#include <genesis.h>

void ExplosionSetup();

u8 ExplosionUpdate();

u8 ExplosionsNeeded();

void PlaceExplosion(u16 x, u16 y);

#endif