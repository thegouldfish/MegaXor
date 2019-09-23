#include "Xor.h"

u8 SelectedLevel = 1;

fix32 MovementPerFrame;

u16 CameraXOffset;
u16 CameraYOffset;

u8 GameInteractionState;

u16 GamePalette[64];

Options PlayerOptions;

char* Version = "V0.5";