//==============================================================================================================================
// Project: Mario GBA
// File: DM_TileManager.h
// Author: Daniel McCluskey
// Date Created: 01/12/17
// Brief: This is the header file that controls background scrolling and level streaming.
// Last Edited by: (See BitBucket Commits: https://bitbucket.org/Danielmclovin/gba-mario
//==============================================================================================================================

#ifndef _TILES_H_
#define _TILES_H_

#include "gba_types.h"

class TileManager
{
public:

	//==============================================================================================================================
	//Bools
	//==============================================================================================================================
	bool bLeft = false;//Scroll the map left
	bool bRight = false;//Scroll the map right

	//==============================================================================================================================
	//Ints
	//==============================================================================================================================
	s32 i_x = 0;//Current pos of map
	s32 iFrame = 0;//Current animation frame
	s32 iScroll_X = 0;//Scroll register store since register is write only
	s32 iScroll_Y = 0;//Scroll register store since register is write only
	s32	i_y = 0;//Current pos of map

	//==============================================================================================================================
	//Unsigned ints
	//==============================================================================================================================
	u16 iScrollOffset = 0;//Offset for prize blocks
	unsigned short* piCurrentMapArray;//Current map to draw

	//==============================================================================================================================
	//Voids
	//==============================================================================================================================
	
	// @brief	Function add a row of tiles offscreen (Level streaming)
	// @param	s32 tx = Pos of row to add
	// @param	s32 ty = Pos of row to add
	void AddRow(s32 tx, s32 ty);

	// @brief	Function to animate a background
	// @param	const unsigned short* a_bgTilesA = Animation tiles A
	// @param	const unsigned short* a_bgTilesB = Animation tiles B
	// @param	const unsigned short* a_bgTilesC = Animation tiles C
	// @param 	const unsigned short* a_bgTilesD = Animation tiles D
	void AnimateBackground(const unsigned short* a_bgTilesA, const unsigned short* a_bgTilesB, const unsigned short* a_bgTilesC, const unsigned short* a_bgTilesD);

	// @brief	Function to Scroll the background if mario is not colliding
	// @param	u8 a_uiLeftCollide = Current collision of left side of mario
	// @param	u8 a_uiRightCollide = Current collision of right side of mario
	void ScrollBackGround(u8 a_uiLeftCollide, u8 a_uiRightCollide);

	// @brief	Function to set the background position
	// @param	s32 a_ix = Pos to set
	// @param	s32 a_iy = Pos to set
	// @param	u32 a_bgMapWidth = Width of background
	void SetPos(s32 a_ix, s32 a_iy, u32 a_bgMapWidth);

	// @brief	Function to Setup a background and draw it
	// @param	s32 a_ix = Pos to set
	// @param	s32 a_iy = Pos to set
	// @param	const unsigned short* a_bgTiles = Tiles to load
	// @param 	u32 a_uiBGTilesLen = Length of tiles array
	// @param 	const unsigned short* a_bgPalette = Palette to load
	// @param 	u32 a_uiBGPalLen = Length of palette
	// @param 	const unsigned short* a_bgMap = Map to load
	// @param 	u32 a_uiBGMapWidth = Width of map in tiles
	void SetupBG(s32 a_ix, s32 a_iy, const unsigned short* a_bgTiles, u32 a_uiBGTilesLen, const unsigned short* a_bgPalette, u32 a_uiBGPalLen, const unsigned short* a_bgMap, u32 a_uiBGMapWidth);
};

#endif // !_TILES_H
