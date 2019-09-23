#include "SaveGame.h"

#include <sram.h>
#include "Xor.h"
#include "GameMaps.h"

void SaveInit()
{
	SRAM_enable();
}


void WriteSaveOut()
{
	SRAM_writeByte(0, SAVE_MAGIC_NUMBER);
	SRAM_writeByte(1, SAVE_VERSION);
	u16 currentByte = 2;

	for (u16 i = 0; i < MAP_COUNT; i++)
	{
		SRAM_writeWord(currentByte, MapMoveCounts[i]);
		currentByte += 2;
	}

	SRAM_writeByte(currentByte, PlayerOptions.SelectedGraphics);
	currentByte++;

}


u8 ReadSaveIn()
{
	u8 magic = SRAM_readByte(0);
	if (magic == SAVE_MAGIC_NUMBER)
	{
		u8 version = SRAM_readByte(1);

		if (version == SAVE_VERSION)
		{
			u16 currentByte = 2;

			for (u16 i = 0; i < MAP_COUNT; i++)
			{
				u16 moveCount = SRAM_readWord(currentByte);
				MapMoveCounts[i] = moveCount;
				currentByte += 2;
			}

			PlayerOptions.SelectedGraphics = SRAM_readByte(currentByte);
			
			currentByte++;


			return TRUE;
		}
	}


	return FALSE;
}
