#ifndef _TILES_H_
#define _TILES_H_


#include "gba_types.h"


class TileManager
{
public:

	s32 scroll_x = 0;
	s32 scroll_y = 0;

	s32 iframe = 0;

	s32 i_x = 0, i_y = 0;
	bool left = false;
	bool right = false;

	void SetupBG(s32 a_ix, s32 a_iy, const unsigned short* a_bgTiles, u32 a_bgTilesLen, const unsigned short* a_bgPalette, u32 a_bgPalLen, const unsigned short* a_bgMap, u32 a_bgMapWidth);
	void LoadBGPalette();
	void LoadTileMap();
	void AnimateBackground(const unsigned short* a_bgTilesA, const unsigned short* a_bgTilesB, const unsigned short* a_bgTilesC, const unsigned short* a_bgTilesD,
							const unsigned short* a_palA, const unsigned short* a_palB, const unsigned short* a_palC, const unsigned short* a_palD);
	void SetPos(s32 a_ix, s32 a_iy, const unsigned short* a_bgMap, u32 a_bgMapWidth);
	void ScrollBackGround(bool a_bLeftCollide, bool a_bRightCollide);
	void AddRow(s32 tx, s32 ty);
	void AddCol(s32 tx, s32 ty);
};


#endif // !_TILES_H
