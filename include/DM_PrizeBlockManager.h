//==============================================================================================================================
// Project: Mario GBA
// File: DM_PrizeBlockManager.h
// Author: Daniel McCluskey
// Date Created: 01/12/17
// Brief: This is the header file that spawns and controls prizeblocks and the power ups they spawn.
// Last Edited by: (See BitBucket Commits: https://bitbucket.org/Danielmclovin/gba-mario
//==============================================================================================================================
#ifndef _PRIZEBLOCKMANAGER_H_
#define _PRIZEBLOCKMANAGER_H_

#include "gba_types.h"
#include "gba_math.h"
#include "DM_SpriteManager.h"
#include "DM_AIManager.h"

class PrizeBlockManager
{
public:

	//==============================================================================================================================
	//Bools
	//==============================================================================================================================
	bool bActive = false;//If the block is active
	bool bNeedsSpawn = false;//If the block needs a sprite spawned above it

	//==============================================================================================================================
	//Ints
	//==============================================================================================================================

	// @brief	Function to create a block at a position
	// @param	s32 a_ix
	// @param	s32 a_iy
	// @param	PrizeBlockManager* a_PrizeBlockArray
	// @param 	SpriteManager& a_oSpriteManager
	// @param 	u16 a_iScrollOffset
	// @param 	bool a_bRightSide
	s16 CreateBlock(s32 a_ix, s32 a_iy, PrizeBlockManager* a_PrizeBlockArray, SpriteManager& a_oSpriteManager, u16 a_iScrollOffset, bool a_bRightSide);
	s32 ix, iy;//Stores position of block

	//==============================================================================================================================
	//Unsigned ints
	//==============================================================================================================================
	u16 iSpriteID;//Sprite id of block
	u16 iSpriteWidth = 16;//Sprite width and height of block

	//==============================================================================================================================
	//Voids
	//==============================================================================================================================

	// @brief	Function to move blocks when the map scrolls
	// @param	SpriteManager& a_oSpriteManager = The global Sprite manager
	// @param	PrizeBlockManager* a_PrizeBlockArray = The block array to look through
	// @param	s32 a_ix = amount to move the blocks
	void MoveBlocks(SpriteManager& a_oSpriteManager, PrizeBlockManager* a_PrizeBlockArray, s32 a_ix);

	// @brief	Function to Spawn a block sprite
	// @param	PrizeBlockManager* a_PrizeBlockArray
	// @param	SpriteManager& a_oSpriteManager
	// @param	AIManager* a_AIManager
	void SpawnPowerUp(PrizeBlockManager* a_PrizeBlockArray, SpriteManager& a_oSpriteManager, AIManager* a_AIManager);
};

#endif // !_PRIZEBLOCKMANAGER_H_
