#ifndef _XOR_H_
#define _XOR_H_

#include <genesis.h>


#include "SimpleState.h"
#include "PadHelper.h"

extern StateMachine GameMachineState;

extern Pad Pad1;

extern u8 SelectedLevel;
extern fix32 MovementPerFrame;


extern u16 CameraXOffset;
extern u16 CameraYOffset;

extern u16 GamePalette[64];

extern char* Version;

#define MOVE_DIRECTION_NONE 0
#define MOVE_DIRECTION_UP 1
#define MOVE_DIRECTION_DOWN 2
#define MOVE_DIRECTION_LEFT 3
#define MOVE_DIRECTION_RIGHT 4


extern u8 GameInteractionState;
#define GAME_INTERACTION_STATE_NORMAL 0
#define GAME_INTERACTION_STATE_USER_REPLAY 1
#define GAME_INTERACTION_STATE_TEST_REPLAY 2

#define MAX_MOVE_COUNT 2000

typedef struct
{
	s16 X;
	s16 Y;
}Point;



typedef struct
{
	s16 X;
	s16 Y;
	u8 Active;
}ActivePoint;


typedef struct
{
	u8 IsActive;

	u8 ActiveIndex;

	u8 OnScreen;

	u16 CurrentMetaX;
	u16 CurrentMetaY;

	u16 DestinationMetaX;
	u16 DestinationMetaY;

	u16 DestinationScreenX;
	u16 DestinationScreenY;


	fix32 MoveRemaining;
	
	u16 MoveDirection;

	Sprite * ActiveSprite;

}ActiveTileItem;



typedef struct
{
	u8 SelectedGraphics;


}Options;


extern Options PlayerOptions;

#endif