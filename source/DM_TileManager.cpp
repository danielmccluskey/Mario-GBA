//==============================================================================================================================
// Project: Mario GBA
// File: DM_TileManager.cpp
// Author: Daniel McCluskey
// Date Created: 01/12/17
// Brief: This is the cpp file for .
// Last Edited by: (See BitBucket Commits: https://bitbucket.org/Danielmclovin/gba-mario
//==============================================================================================================================


#include "DM_Tilemanager.h"

#include "gba.h"
#include <string.h>//Memcpy


s32 MAPWIDTH = 32;//Map width in tiles
s32 MAPHEIGHT = 32;//Map height in tiles

#define SCREENBLOCK_WIDTH 32//Screenblock width in tiles
#define SCREENBLOCK_HEIGHT 32//Screenblock height in tiles
#define TILE_COLLISION_TILE 0//The collision tile for all maps
u16 iScrollAmount = 2;//How much to scroll the map by

void TileManager::SetupBG(s32 a_ix, s32 a_iy, const unsigned short* a_bgTiles, u32 a_uiBGTilesLen, const unsigned short* a_bgPalette, u32 a_uiBGPalLen, const unsigned short* a_bgMap, u32 a_uiBGMapWidth)
{
	memcpy(pal_bg_mem, a_bgPalette, a_uiBGPalLen);//Copy the palette to memory
	memcpy(&tile_mem[0][0], a_bgTiles, a_uiBGTilesLen);//Copy the tiles to memory
	i_x = a_ix;//Set the pos of the background
	i_y = a_iy;//Set the pos of the background
	iScroll_X = a_ix;//Set the scroll value of the background
	BG_HOFFSET0 = iScroll_X;//Set the scroll offsets
	BG_VOFFSET0 = 0;//Set the scroll offsets
	iScrollOffset = 0;//Reset scroll offset for prize block placing
	MAPWIDTH = a_uiBGMapWidth;//Sets map width in tiles
	SCR_ENTRY *psScreenBlock = se_mem[15];//Creates a "Viewport" of the screen block. Contains 1024 members, perfect for a screen block since it is 32x32
	SCR_ENTRY *psMapPort = (SCR_ENTRY*)piCurrentMapArray;//Creates another viewport of the background map.
	s32 actY = 0;//Value to make screenblock y separate from map y

	for (s32 iy = i_y; iy < i_y + SCREENBLOCK_WIDTH; iy++)//Loop through the screenblock Y
	{
		for (s32 ix = i_x; ix < i_x + SCREENBLOCK_WIDTH; ix++)//Loop through the screenblock X
		{
			psScreenBlock[(actY * SCREENBLOCK_WIDTH) + ix - i_x] = psMapPort[(iy * MAPWIDTH) + ix];//Set the current position in the screen block to the current map tile.
		}
		actY++;//Increment screenblock row

	}



}
void TileManager::SetPos(s32 a_ix, s32 a_iy, u32 a_bgMapWidth)
{
	i_x = a_ix;//Copy pos
	i_y = a_iy;//Copy pos
	SCR_ENTRY *psScreenBlock = se_mem[15];//Creates a "Viewport" of the screen block. Contains 1024 members, perfect for a screen block since it is 32x32
	SCR_ENTRY *psMapPort = (SCR_ENTRY*)piCurrentMapArray;//Creates another viewport of the background map.
	s32 actY = 0;//Value to make screenblock y separate from map y
	for (s32 iy = i_y; iy < i_y + SCREENBLOCK_WIDTH; iy++)//Loop through the screenblock Y
	{
		for (s32 ix = i_x; ix < i_x + SCREENBLOCK_WIDTH; ix++)//Loop through the screenblock X
		{
			psScreenBlock[(actY * SCREENBLOCK_WIDTH) + ix - i_x] = psMapPort[(iy * a_bgMapWidth) + ix];//Set the current position in the screen block to the current map tile.
		}
		actY++;//Increment screenblock row

	}
	
}


void TileManager::AnimateBackground(const unsigned short* a_bgTilesA, const unsigned short* a_bgTilesB, const unsigned short* a_bgTilesC, const unsigned short* a_bgTilesD)
{

	//Simple function to animate the world map.
	//Just replaces the currently used tiles with the next set depending on iFrame
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
	iFrame++;//Increment iFrame

}

void TileManager::ScrollBackGround(u8 a_uiLeftCollide, u8 a_uiRightCollide)
{
	if (bRight && !(a_uiRightCollide > TILE_COLLISION_TILE))//If asked to move right and mario is not colliding
	{
		i_x += iScrollAmount;//Scroll the map
		iScroll_X += iScrollAmount;//Scroll the map

		iScrollOffset += iScrollAmount;//Add to the offset for the prize block placing
		if (iScrollOffset >= 16)//If it is greater than 2 tiles
		{
			iScrollOffset = 0;//Reset
		}
	}
	if (bLeft && !(a_uiLeftCollide > TILE_COLLISION_TILE))//Not used anymore but left in anyway. If asked to move left and mario is not colliding
	{
		i_x -= iScrollAmount;//Scroll the map backward
		iScroll_X -= iScrollAmount;//Scroll the map backward
	}

	if ((bRight && !(a_uiRightCollide > TILE_COLLISION_TILE)) || (bLeft && !(a_uiLeftCollide > TILE_COLLISION_TILE)))//If asked to move and mario is not colliding
	{


		/*
		I emailed Tonc for help with this as he has multiple examples of using Level streaming
		but has not made an article on his website about it. He told me the basics of how it works
		and pointed me towards a code example he had.		
		*/

		BG_HOFFSET0 = iScroll_X;//Set the scroll offsets
		BG_VOFFSET0 = iScroll_Y;//Set the scroll offsets
		s32 iViewPortX = iScroll_X - 8;//Set the position of the edge of the viewport
		s32	iViewPortY = iScroll_Y + 4;//Set the positio of the edge of the viewport
		s32 iTileViewPortX = iViewPortX >> 3;//Set viewport to tile pos
		s32 iTileViewPortY = iViewPortY >> 3;//Set viewport to tile pos

		if (bRight)//If moving right
		{
			AddRow(iTileViewPortX, iTileViewPortY);//Add row to left of viewport
		}
			
		else if (bLeft)//If moving right
		{
			AddRow(iTileViewPortX + 31, iTileViewPortY);//Add row to right of viewport
		}
		bLeft = false;//Reset flag
		bRight = false;//Reset flag
	}
	
	
}




void TileManager::AddRow(s32 tx, s32 ty)
{	
	s32 iy;//Used for for loops
	s32 iEndofViewport = ty & 31;//"clamps" to 32

	SCR_ENTRY *psScreenBlocks = se_mem[15]; //Creates a "Viewport" of the screen block. Contains 1024 members, perfect for a screen block since it is 32x32
	SCR_ENTRY *piBGMap = (SCR_ENTRY*)piCurrentMapArray; //Creates another viewport of the map tiles
	SCR_ENTRY *psBGMapSource;//Defines another Viewport used further down
	SCR_ENTRY *psScreenBlockSource;//Defines another Viewport used further down

	if (bRight)//If moving to the right
	{
		psBGMapSource = &piBGMap[i_y*MAPWIDTH + tx + 32];//Grab next row of tiles from the Background map
		psScreenBlockSource = &psScreenBlocks[iEndofViewport * SCREENBLOCK_WIDTH + (tx & 31)];//Set the screen block source to offscreen left of the viewport
	}
	else//If moving left (Not used anymore but kept in)
	{
		psBGMapSource = &piBGMap[i_y*MAPWIDTH + tx - 32];//Grab the next row of tiles from the background map
		psScreenBlockSource = &psScreenBlocks[iEndofViewport * SCREENBLOCK_WIDTH + ((tx+1) & 31)];//Set the screenblock source to offscreen right of the viewport
	}	

	for (iy = iEndofViewport; iy<32; iy++)//Loop down the side of the viewport (RIGHT)
	{
		*psScreenBlockSource = *psBGMapSource;//Screenblock source to the current position of the map
		psScreenBlockSource += SCREENBLOCK_WIDTH;//Go to next row of screenblock
		psBGMapSource += MAPWIDTH;//Go to next row of background map
	}

	psScreenBlockSource -= 1024;//Go back to the beginning of the screenblock

	for (iy = 0; iy<iEndofViewport; iy++)//Loop down the side of the viewport (LEft)
	{
		*psScreenBlockSource = *psBGMapSource;//Screenblock source to the current position of the map
		psScreenBlockSource += SCREENBLOCK_WIDTH;//Go to next row of screenblock
		psBGMapSource += MAPWIDTH;//Go to next row of background map
	}	
}