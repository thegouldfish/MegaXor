#ifndef _PLAYERS_H_
#define _PLAYERS_H_

#include <genesis.h>


#define PLAYER_STATE_WAITING 0
#define PLAYER_STATE_MOVING 1
#define PLAYER_STATE_FINISHED 2
#define PLAYER_STATE_CHANGING 3

#define PLAYER_STATE_TILE_LOGIC 4
#define PLAYER_STATE_TILE_UPDATE 5
#define PLAYER_STATE_TILE_FINISHED 6
#define PLAYER_STATE_TILE_WHOOPS 7

#define PLAYER_STATE_COMPLETE 10
#define PLAYER_STATE_GAMEOVER 11



#define PLAYER_ALIVE_YES 1
#define PLAYER_ALIVE_REPORT_NO 2
#define PLAYER_ALIVE_NO 3

typedef struct
{
	u8 Id;

	u8 TileId;

	Sprite* Sprite;

	u8 SpriteFrame;

	u16 MetaX;
	u16 MetaY;

	u16 ScreenX;
	u16 ScreenY;

	u16 ScreenMetaX;
	u16 ScreenMetaY;

	u8 Alive;

	u8 CanTeleport;
}Player;



extern Player Players[2];
extern Player* CurrentPlayer;
extern Player* OtherPlayer;

extern u8 PlayerState;

extern s16 StepsTaken;
extern s16 MasksCollected;
extern s16 TotalMasks;
extern s16 MapsCollected;



void PlayersSetup();

u8 PlayerMoveDown();
u8 PlayerMoveUp();
u8 PlayerMoveLeft();
u8 PlayerMoveRight();

u8 PlayerChange();


void PlayerStartMove();
void PlayerUpdateMove(u8 justSet);
void PlayerEndMove();

void PlayerKillCurrent();

void PlayerKillOther();

u8 BothDead();

#endif