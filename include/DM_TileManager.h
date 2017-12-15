#ifndef _TILES_H_
#define _TILES_H_


#include "gba_types.h"

class TileManager
{
public:

	s32 iScroll_X = 0;
	s32 iScroll_Y = 0;

	s32 iFrame = 0;
	u16 iScrollOffset = 0;

	s32 i_x = 0;
	s32	i_y = 0;

	unsigned short* piCurrentMapArray;

	bool bLeft = false;
	bool bRight = false;

	void SetupBG(s32 a_ix, s32 a_iy, const unsigned short* a_bgTiles, u32 a_uiBGTilesLen, const unsigned short* a_bgPalette, u32 a_uiBGPalLen, const unsigned short* a_bgMap, u32 a_uiBGMapWidth);
	void AnimateBackground(const unsigned short* a_bgTilesA, const unsigned short* a_bgTilesB, const unsigned short* a_bgTilesC, const unsigned short* a_bgTilesD);
	void SetPos(s32 a_ix, s32 a_iy, u32 a_bgMapWidth);
	void ScrollBackGround(u8 a_uiLeftCollide, u8 a_uiRightCollide);
	void AddRow(s32 tx, s32 ty);
};


#endif // !_TILES_H
