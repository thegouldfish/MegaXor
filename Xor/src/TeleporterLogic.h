#ifndef TELEPORTER_LOGIC_H_
#define TELEPORTER_LOGIC_H_

#include <genesis.h>


void SetupTeleporters();

u8 TeleportCurrentPlayer();


u8 FinishTeleport(Sprite* player);

#endif