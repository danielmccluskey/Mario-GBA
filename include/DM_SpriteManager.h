//==============================================================================================================================
// Project: Mario GBA
// File: DM_SpriteManager.h
// Author: Daniel McCluskey
// Date Created: 01/12/17
// Brief: This is the header file that handles all sprite spawning and manages them.
// Last Edited by: (See BitBucket Commits: https://bitbucket.org/Danielmclovin/gba-mario
//==============================================================================================================================
#ifndef _SPRITEMANAGER_H_
#define _SPRITEMANAGER_H_

#include "gba_types.h"
#include "gba_gfx.h"

class SpriteManager
{
public:

	//==============================================================================================================================
	//Bools
	//==============================================================================================================================
	bool OccupiedSprite[128];//Array of bools to show which sprites are currently being used

	//==============================================================================================================================
	//Ints
	//==============================================================================================================================
	s32 SpriteIndex = 0;//Current sprite index

	//==============================================================================================================================
	//Unsigned ints
	//==============================================================================================================================
	
	// @brief	Function to create a sprite, returns assigned SpriteID
	// @param	u16* a_Tiles = The tiles for the sprite
	// @param	u16* a_Palette = The sprites palette
	// @param	s32 a_TileSize = The size of the tiles array
	// @param	s32 a_PaletteSize = The size of the palette array
	// @param 	s32 a_TileBlock = Where to place them in OAM tile memory
	// @param 	s32 a_PaletteBank = Where to place the palette in Sprite palette memory
	u16 CreateSprite(u16* a_Tiles, u16* a_Palette, s32 a_TileSize, s32 a_PaletteSize, s32 a_TileBlock, s32 a_PaletteBank);

	// @brief	Function to set attr0
	// @param	u16 a_uiY = Set the Y position of the sprite
	// @param	u8 a_uiObjectMode = Set the object mode
	// @param	u8 a_uiGfxMode = Set the GFX mode
	// @param 	u8 a_uiMosaic = Set the Mosaic mode
	// @param 	u8 a_uiColormode = Set colour mode 
	// @param 	u8 a_uiShape = Set the shape of the sprite
	u16 setSpriteAttr0(u16 a_uiY, u8 a_uiObjectMode, u8 a_uiGfxMode, u8 a_uiMosaic, u8 a_uiColormode, u8 a_uiShape);

	// @brief	Function to set attr1
	// @param	u16 a_uiX = Set XPos of the sprite
	// @param	u8 a_uiAffine = Set affine flag
	// @param	u8 a_uiHoriFlip = Horizontal flip
	// @param 	u8 a_uiVertFlip = Vertical flip
	// @param 	u8 a_uiSize = Set the size of the sprite
	u16 setSpriteAttr1(u16 a_uiX, u8 a_uiAffine, u8 a_uiHoriFlip, u8 a_uiVertFlip, u8 a_uiSize);

	// @brief	Function to set attr2
	// @param	u32 a_uiTileId = Set what tiles the sprite should display
	// @param	u32 a_uiPalBank = Set what palette the sprite should use
	// @param	u32 a_uiPriority = Set the priority of the sprite
	u16 setSpriteAttr2(u32 a_uiTileId, u32 a_uiPalBank, u32 a_uiPriority);

	//==============================================================================================================================
	//Voids
	//==============================================================================================================================
	
	// @brief	Function to delete a sprite
	// @param	s32 a_iSpriteID = The assigned ID of the sprite
	void DeleteSprite(s32 a_iSpriteID);

	// @brief	Function to hide a sprite
	// @param	s32 a_iSpriteID = The assigned ID of the sprite
	void HideSprite(s32 a_SpriteID);

	// @brief	Function to initilise the sprite array
	void InitialiseArray();

	// @brief	Function to get a collision tile from an array based off the input
	// @param	u16* a_Tiles = The tiles for the sprite
	// @param	u16* a_Palette = The sprites palette
	// @param	s32 a_TileSize = The size of the tiles array
	// @param	s32 a_PaletteSize = The size of the palette array
	// @param 	s32 a_TileBlock = Where to place them in OAM tile memory
	// @param 	s32 a_PaletteBank = Where to place the palette in Sprite palette memory
	void LoadTiles(u16* a_Tiles, u16* a_Palette, s32 a_TileSize, s32 a_PaletteSize, s32 a_TileBlock, s32 a_PaletteBank);

	// @brief	Function to move a sprite
	// @param	s16 a_ix = X pos to move to
	// @param	s16 a_iy = Y Pos to move to
	// @param	s32 a_iSpriteID = The assigned ID of the sprite
	void MoveSprite(s16 a_ix, s16 a_iy, s32 a_SpriteID);

	// @brief	Function to set the animation frame of a sprite
	// @param	s32 a_iframe = Frame to set
	// @param	s32 a_iSpriteID = The assigned ID of the sprite
	void SetFrame(s32 a_iframe, s32 a_iSpriteID);

	// @brief	Function to horizontally flip a sprite
	// @param	bool a_bFlip = Whether to flip the sprite or not
	// @param	s32 a_iSpriteID = The assigned ID of the sprite
	void SetHFlip(bool a_bFlip, s32 a_SpriteID);

	// @brief	Function to vertically flip a sprite
	// @param	bool a_bFlip = Whether to flip the sprite or not
	// @param	s32 a_iSpriteID = The assigned ID of the sprite
	void SetVFlip(bool a_bFlip, s32 a_SpriteID);

	// @brief	Function to unhide a sprite
	// @param	s32 a_iSpriteID = The assigned ID of the sprite
	void ShowSprite(s32 a_SpriteID);

	//==============================================================================================================================
	//Structure/Class Members
	//==============================================================================================================================
	volatile ObjectAttribute* SpriteArray[128];//The array of sprites
};

#endif // !_SPRITEMANAGER_H_
