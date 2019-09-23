#ifndef _SAVE_GAME_H_
#define _SAVE_GAME_H_

#include <genesis.h>

#define SAVE_VERSION 1
#define SAVE_MAGIC_NUMBER 77



void SaveInit();

void WriteSaveOut();


u8 ReadSaveIn();




#endif