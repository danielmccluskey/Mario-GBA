#ifndef _TILES_H
#include "../include/main.h"

v_uint16* Get_CharacterBlock(uint32 a_block)
{
	return (v_uint16*)(VRAMBASE + (a_block * 0x4000));
}
v_uint16* Get_ScreenBlock(uint32 a_block)
{
	return (v_uint16*)(VRAMBASE + (a_block * 0x800));
}


#endif // !_TILES_H
