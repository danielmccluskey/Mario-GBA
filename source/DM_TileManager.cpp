


#include "DM_TileManager.h"

#include "gba.h"
#include <string.h>





s32 MAPWIDTH = 32;
s32 MAPHEIGHT = 32;


//Background Scrollers
vu16* BG0SC_X = (vu16*)0x4000010;
vu16* BG0SC_Y = (vu16*)0x4000012;
vu16* BG1SC_X = (vu16*)0x4000014;
vu16* BG1SC_Y = (vu16*)0x4000016;
vu16* BG2SC_X = (vu16*)0x4000018;
vu16* BG2SC_Y = (vu16*)0x400001a;
vu16* BG3SC_X = (vu16*)0x400001c;
vu16* BG3SC_Y = (vu16*)0x400001e;

#define BG_HOFFSET0			*(vu16*)(REG_BASE+0x0010)		
#define BG_VOFFSET0			*(vu16*)(REG_BASE+0x0012)

s16 xscroll = 0;
u16 ScrollAmount = 2;

#define PALSIZE 256

void TileManager::SetupBG(s32 a_ix, s32 a_iy, const unsigned short* a_bgTiles, u32 a_bgTilesLen, const unsigned short* a_bgPalette, u32 a_bgPalLen, const unsigned short* a_bgMap, u32 a_bgMapWidth)
{
	memcpy(pal_bg_mem, a_bgPalette, a_bgPalLen);
	memcpy(&tile_mem[0][0], a_bgTiles, a_bgTilesLen);
	i_x = a_ix;
	i_y = a_iy;


	scroll_x = a_ix;
	//scroll_y = a_iy;
	MAPWIDTH = a_bgMapWidth;
	SCR_ENTRY *ScreenBlock = se_mem[15], *map = (SCR_ENTRY*)a_bgMap;
	s32 actY = 0;
	for (s32 iy = i_y; iy < i_y + 32; iy++)
	{
		for (s32 ix = i_x; ix < i_x + 32; ix++)
		{
			ScreenBlock[(actY * 32) + ix - i_x] = map[(iy * MAPWIDTH) + ix];
		}
		actY++;

	}



}
void TileManager::SetPos(s32 a_ix, s32 a_iy, const unsigned short* a_bgMap, u32 a_bgMapWidth)
{
	i_x = a_ix;
	i_y = a_iy;
	SCR_ENTRY *ScreenBlock = se_mem[15], *map = (SCR_ENTRY*)a_bgMap;
	s32 actY = 0;
	for (s32 iy = i_y; iy < i_y + 32; iy++)
	{
		for (s32 ix = i_x; ix < i_x + 32; ix++)
		{
			ScreenBlock[(actY * 32) + ix - i_x] = map[(iy * a_bgMapWidth) + ix];
		}
		actY++;

	}
	
}


void TileManager::AnimateBackground(const unsigned short* a_bgTilesA, const unsigned short* a_bgTilesB, const unsigned short* a_bgTilesC, const unsigned short* a_bgTilesD)
{
	switch (iframe)
	{
	case 25:
		memcpy(&tile_mem[0][0], a_bgTilesA, 1024 * 2);

		break;
	case 50:
		memcpy(&tile_mem[0][0], a_bgTilesB, 1024 * 2);
		break;
	case 75:
		memcpy(&tile_mem[0][0], a_bgTilesC, 1024 * 2);
		break;
	case 100:
		memcpy(&tile_mem[0][0], a_bgTilesD, 1024 * 2);
		iframe = 1;
		break;


	}

	iframe++;

}

void TileManager::ScrollBackGround(bool a_bLeftCollide, bool a_bRightCollide, const unsigned short* a_bgMap)
{


	

	if (right && !a_bRightCollide)
	{
		i_x += ScrollAmount;
		scroll_x += ScrollAmount;

		iScrollOffset += 2;
		if (iScrollOffset >= 16)
		{
			iScrollOffset = 0;
		}
	}
	if (left && !a_bLeftCollide)
	{
		i_x -= ScrollAmount;
		scroll_x -= ScrollAmount;
	}

	if ((right && !a_bRightCollide) || (left && !a_bLeftCollide))
	{
		BG_HOFFSET0 = scroll_x;
		BG_VOFFSET0 = scroll_y;
		// Pixel coords
		int vx = scroll_x - 8, vy = scroll_y + 4;
		int bx = i_x, by = i_y;

		// Tile coords
		int tvx = vx >> 3, tvy = vy >> 3;
		int tbx = bx >> 3, tby = by >> 3;

		if (right)		// add on left
			AddRow(tvx, tvy, a_bgMap);
		else if (left)	// add on right
			AddRow(tvx + 31, tvy, a_bgMap);

		if (tvy < tby)		// add on top
			AddCol(tvx, tvy, a_bgMap);
		else if (tvy > tby)	// add on bottom
			AddCol(tvx, tvy + 31, a_bgMap);

		left = false;
		right = false;
	}
	
	
}




void TileManager::AddRow(s32 tx, s32 ty, const unsigned short* a_bgMap)
{	
	int iy, y0 = ty & 31;

	int srcP = MAPWIDTH;
	SCR_ENTRY *ScreenBlocks = se_mem[15], *maps = (SCR_ENTRY*)a_bgMap;
	SCR_ENTRY *srcL;
	SCR_ENTRY *dstL;
	if (right)
	{
		srcL = &maps[i_y*srcP + tx + 32];
		dstL = &ScreenBlocks[y0 * 32 + (tx & 31)];
	}
	else
	{
		srcL = &maps[i_y*srcP + tx - 32];
		dstL = &ScreenBlocks[y0 * 32 + (tx+1 & 31)];
	}
	
	

	for (iy = y0; iy<32; iy++)
	{
		*dstL = *srcL;	dstL += 32;	srcL += srcP;
	}

	dstL -= 1024;

	for (iy = 0; iy<y0; iy++)
	{
		*dstL = *srcL;	dstL += 32;	srcL += srcP;
	}	
}
void TileManager::AddCol(s32 tx, s32 ty, const unsigned short* a_bgMap)
{
	int ix, x0 = tx & 31;

	int srcP = MAPWIDTH;
	SCR_ENTRY *ScreenBlocks = se_mem[15], *maps = (SCR_ENTRY*)a_bgMap;
	SCR_ENTRY *srcL = &maps[ty*srcP + tx];
	SCR_ENTRY *dstL = &ScreenBlocks[(ty & 31) * 32 + x0];

	for (ix = x0; ix<32; ix++)
		*dstL++ = *srcL++;

	dstL -= 32;

	for (ix = 0; ix<x0; ix++)
		*dstL++ = *srcL++;
}