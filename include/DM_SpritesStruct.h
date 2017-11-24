#ifndef _SPRITES_H_
#define _SPRITES_H_


#include "gba_types.h"


typedef struct Sprite
{
	//s32 i_x = 0, i_y = 0;

	u16 attribute0;
	u16 attribute1;
	u16 attribute2;
	s16 attribute3;

}__attribute__((packed, aligned(4))) Sprite;

typedef struct SpriteInfo
{
	s32 i_x = 0;
	s32 i_y = 0;
}__attribute__((packed, aligned(4))) SpriteInfo;


#endif // !_SPRITES_H_
