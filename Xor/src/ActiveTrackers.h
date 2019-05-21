#ifndef _ACTIVE_TRACKERS_H_
#define _ACTIVE_TRACKERS_H_

void ActiveReset();

void ActiveSet(u16 x, u16 y);

void ActiveRemove(u16 x, u16 y);

u8 ActiveEnabled(u16 x, u16 y);


void ActivePrint();
#endif