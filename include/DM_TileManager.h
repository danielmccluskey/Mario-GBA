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

	void SetupBG(s32 a_ix, s32 a_iy);
	void LoadBGPalette();
	void LoadTileMap();
	void LoadImage();
	void SetPos(s32 a_ix, s32 a_iy);
	void ScrollBackGround(bool a_bLeftCollide, bool a_bRightCollide);
	void AddRow(s32 tx, s32 ty);
	void AddCol(s32 tx, s32 ty);
};


#endif // !_TILES_H
