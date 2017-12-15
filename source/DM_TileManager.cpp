


#include "DM_Tilemanager.h"

#include "gba.h"
#include <string.h>
s32 MAPWIDTH = 32;
s32 MAPHEIGHT = 32;

#define SCREENBLOCK_WIDTH 32
#define SCREENBLOCK_HEIGHT 32
#define TILE_COLLISION_TILE 0

s16 xscroll = 0;
u16 ScrollAmount = 2;

void TileManager::SetupBG(s32 a_ix, s32 a_iy, const unsigned short* a_bgTiles, u32 a_uiBGTilesLen, const unsigned short* a_bgPalette, u32 a_uiBGPalLen, const unsigned short* a_bgMap, u32 a_uiBGMapWidth)
{
	memcpy(pal_bg_mem, a_bgPalette, a_uiBGPalLen);
	memcpy(&tile_mem[0][0], a_bgTiles, a_uiBGTilesLen);
	i_x = a_ix;
	i_y = a_iy;
	iScroll_X = a_ix;
	BG_HOFFSET0 = iScroll_X;
	BG_VOFFSET0 = 0;
	iScrollOffset = 0;
	MAPWIDTH = a_uiBGMapWidth;
	SCR_ENTRY *ScreenBlock = se_mem[15], *map = (SCR_ENTRY*)piCurrentMapArray;
	s32 actY = 0;
	for (s32 iy = i_y; iy < i_y + SCREENBLOCK_WIDTH; iy++)
	{
		for (s32 ix = i_x; ix < i_x + SCREENBLOCK_WIDTH; ix++)
		{
			ScreenBlock[(actY * SCREENBLOCK_WIDTH) + ix - i_x] = map[(iy * MAPWIDTH) + ix];
		}
		actY++;

	}



}
void TileManager::SetPos(s32 a_ix, s32 a_iy, u32 a_bgMapWidth)
{
	i_x = a_ix;
	i_y = a_iy;
	SCR_ENTRY *ScreenBlock = se_mem[15], *map = (SCR_ENTRY*)piCurrentMapArray;
	s32 actY = 0;
	for (s32 iy = i_y; iy < i_y + SCREENBLOCK_WIDTH; iy++)
	{
		for (s32 ix = i_x; ix < i_x + SCREENBLOCK_WIDTH; ix++)
		{
			ScreenBlock[(actY * SCREENBLOCK_WIDTH) + ix - i_x] = map[(iy * a_bgMapWidth) + ix];
		}
		actY++;

	}
	
}


void TileManager::AnimateBackground(const unsigned short* a_bgTilesA, const unsigned short* a_bgTilesB, const unsigned short* a_bgTilesC, const unsigned short* a_bgTilesD)
{
	switch (iFrame)
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
		iFrame = 1;
		break;


	}

	iFrame++;

}

void TileManager::ScrollBackGround(u8 a_uiLeftCollide, u8 a_uiRightCollide)
{


	

	if (bRight && !(a_uiRightCollide > TILE_COLLISION_TILE))
	{
		i_x += ScrollAmount;
		iScroll_X += ScrollAmount;

		iScrollOffset += ScrollAmount;
		if (iScrollOffset >= 16)
		{
			iScrollOffset = 0;
		}
	}
	if (bLeft && !(a_uiLeftCollide > TILE_COLLISION_TILE))
	{
		i_x -= ScrollAmount;
		iScroll_X -= ScrollAmount;
	}

	if ((bRight && !(a_uiRightCollide > TILE_COLLISION_TILE)) || (bLeft && !(a_uiLeftCollide > TILE_COLLISION_TILE)))
	{
		BG_HOFFSET0 = iScroll_X;
		BG_VOFFSET0 = iScroll_Y;
		// Pixel coords
		int iViewPortX = iScroll_X - 8, iViewPortY = iScroll_Y + 4;
		// Tile coords
		int iTileViewPortX = iViewPortX >> 3, iTileViewPortY = iViewPortY >> 3;

		if (bRight)		
		{
			AddRow(iTileViewPortX, iTileViewPortY);
		}
			
		else if (bLeft)
		{
			AddRow(iTileViewPortX + 31, iTileViewPortY);
		}
		bLeft = false;
		bRight = false;
	}
	
	
}




void TileManager::AddRow(s32 tx, s32 ty)
{	
	int iy, iEndofViewport = ty & 31;
	SCR_ENTRY *psScreenBlocks = se_mem[15], *piBGMap = (SCR_ENTRY*)piCurrentMapArray;
	SCR_ENTRY *psBGMapSource;
	SCR_ENTRY *psScreenBlockSource;
	if (bRight)
	{
		psBGMapSource = &piBGMap[i_y*MAPWIDTH + tx + 32];
		psScreenBlockSource = &psScreenBlocks[iEndofViewport * SCREENBLOCK_WIDTH + (tx & 31)];
	}
	else
	{
		psBGMapSource = &piBGMap[i_y*MAPWIDTH + tx - 32];
		psScreenBlockSource = &psScreenBlocks[iEndofViewport * SCREENBLOCK_WIDTH + ((tx+1) & 31)];
	}
	
	

	for (iy = iEndofViewport; iy<32; iy++)
	{
		*psScreenBlockSource = *psBGMapSource;	psScreenBlockSource += SCREENBLOCK_WIDTH;	psBGMapSource += MAPWIDTH;
	}

	psScreenBlockSource -= 1024;

	for (iy = 0; iy<iEndofViewport; iy++)
	{
		*psScreenBlockSource = *psBGMapSource;	psScreenBlockSource += SCREENBLOCK_WIDTH;	psBGMapSource += MAPWIDTH;
	}	
}