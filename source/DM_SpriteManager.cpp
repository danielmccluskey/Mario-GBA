
//==============================================================================================================================
// Project: Mario GBA
// File: DM_SpriteManager.cpp
// Author: Daniel McCluskey
// Date Created: 01/12/17
// Brief: This is the cpp file that handles all sprite spawning and manages them.
// Last Edited by: (See BitBucket Commits: https://bitbucket.org/Danielmclovin/gba-mario
//==============================================================================================================================
#include "DM_SpriteManager.h"
#include "gba_reg.h"
#include <string.h>
#include "gba_gfx.h"


u16 SpriteManager::CreateSprite(u16* a_Tiles, u16* a_Palette, s32 a_TileSize, s32 a_PaletteSize, s32 a_TileBlock, s32 a_PaletteBank)
{
	for (int i = 0; i < 128; i++)//Loop through sprite array
	{
		if (OccupiedSprite[i] == false)//Find first unoccupied sprite
		{
			OccupiedSprite[i] = true;//Set sprite to occupied
			SpriteIndex = i;//Set index to that number
			break;
		}
	}

	LoadTiles(a_Tiles, a_Palette, a_TileSize, a_PaletteSize *2, a_TileBlock, a_PaletteBank);//Load the given tile from the arguments

	SpriteArray[SpriteIndex] = &MEM_OAM[SpriteIndex];//Set the sprite array to the MEM_OAM address of that sprite

	SpriteArray[SpriteIndex]->attr0 = setSpriteAttr0(0, 0, 0, 0, A0_4BPP, A0_SQUARE);//Set initial attributes
	SpriteArray[SpriteIndex]->attr1 = setSpriteAttr1(0, 0, 0, 0, A1_SIZE_1);//Set initial attributes
	SpriteArray[SpriteIndex]->attr2 = setSpriteAttr2(a_TileBlock, a_PaletteBank, 0);//Set Tile bank and palette bank.



	return SpriteIndex;//Return the sprite id
}

void SpriteManager::DeleteSprite(s32 a_iSpriteID)
{
	OccupiedSprite[a_iSpriteID] = false;//Set occupied position to false
	HideSprite(a_iSpriteID);//Hide the sprite
}

void SpriteManager::LoadTiles(u16* a_Tiles, u16* a_Palette, s32 a_TileSize, s32 a_PaletteSize, s32 a_TileBlock, s32 a_PaletteBank)
{
	memcpy(palette_sp_block_address(a_PaletteBank), a_Palette, a_PaletteSize);//Copy palette to given address
	memcpy(sprite_tile_block_address(a_TileBlock), a_Tiles, a_TileSize);//Copy tiles to given address
}

void SpriteManager::InitialiseArray()
{
	for (int i = 0; i < 128; ++i)//Loop through all 128 sprites
	{
		SpriteArray[i] = &MEM_OAM[i];//Set their address to the correct MEM_OAM location

		SpriteArray[i]->attr0 = setSpriteAttr0(0, 2, 0, 0, A0_4BPP, A0_SQUARE);//Set initial attributes
		SpriteArray[i]->attr1 = setSpriteAttr1(0, 0, 0, 0, 1);//Set initial attributes
		SpriteArray[i]->attr2 = setSpriteAttr2(0, 0, 0);//Set initial attributes
	}
}

void SpriteManager::HideSprite(s32 a_SpriteID)
{
	SpriteArray[a_SpriteID]->attr0 = (SpriteArray[a_SpriteID]->attr0 & A0_MODE_MASK) | (A0_MODE_HIDE << 8);//Hides the sprite by directly affecting the hide bits using a mask
}

//! Unhide an object.
void SpriteManager::ShowSprite(s32 a_SpriteID)
{
	SpriteArray[a_SpriteID]->attr0 = (SpriteArray[a_SpriteID]->attr0 & A0_MODE_MASK) | ((0 & 0x3) << 8);//unhides the sprite by directly affecting the hide bits using a mask
}


void SpriteManager::SetHFlip(bool a_bFlip, s32 a_SpriteID)
{
	if (a_bFlip)
	{
		SpriteArray[a_SpriteID]->attr1 |= 0x1000;//Flips the sprite by directly affecting the flip bits using a mask
	}
	else
	{
		SpriteArray[a_SpriteID]->attr1 &= 0xefff;//Flips the sprite by directly affecting the flip bits using a mask
	}
}
void SpriteManager::SetVFlip(bool a_bFlip, s32 a_SpriteID)
{
	if (a_bFlip)
	{
		SpriteArray[a_SpriteID]->attr1 |= 0x2000;//Flips the sprite by directly affecting the flip bits using a mask
	}
	else
	{
		SpriteArray[a_SpriteID]->attr1 &= 0xdfff;//Flips the sprite by directly affecting the flip bits using a mask
	}
}
void SpriteManager::SetFrame(s32 a_iframe, s32 a_iSpriteID)
{
	SpriteArray[a_iSpriteID]->attr2 &= 0xfc00;//Clears the current frame information.
	SpriteArray[a_iSpriteID]->attr2 |= (a_iframe & 0x03ff);//Sets the new frame information by directly accessing the frame using a mask
}
void SpriteManager::MoveSprite(s16 a_ix, s16 a_iy, s32 a_SpriteID)
{
	SpriteArray[a_SpriteID]->attr0 = (SpriteArray[a_SpriteID]->attr0 & 0xFF00) | (a_iy & 0x00FF);//Sets the position of the sprite
	SpriteArray[a_SpriteID]->attr1 = (SpriteArray[a_SpriteID]->attr1 & 0xFE00) | (a_ix & 0x01FF);//Sets the position of the sprite
}

u16 SpriteManager::setSpriteAttr0(u16 a_uiY, u8 a_uiObjectMode, u8 a_uiGfxMode, u8 a_uiMosaic, u8 a_uiColormode, u8 a_uiShape)
{
	u16 attrib0 = ((a_uiY & A0_YMASK) |
		((a_uiObjectMode & A0_MODE_MASK) << 8) |
		((a_uiGfxMode & 0x3) << 10) |
		((a_uiMosaic & 1) << 12) |
		((a_uiColormode & 1) << 13) |
		((a_uiShape & 0x3) << 14));
	return attrib0;
}
u16 SpriteManager::setSpriteAttr1(u16 a_uiX, u8 a_uiAffine, u8 a_uiHoriFlip, u8 a_uiVertFlip, u8 a_uiSize)
{
	u16 attrib1 = ((a_uiX & A1_XMASK) |
		((a_uiAffine & 0x3) << 9) |
		((a_uiHoriFlip & 0x3) << 12) |
		((a_uiVertFlip & 0x1) << 13) |
		((a_uiSize & 0x3) << 14));
	return attrib1;
}
u16 SpriteManager::setSpriteAttr2(u32 a_uiTileId, u32 a_uiPalBank, u32 a_uiPriority)
{
	u16 attrib2 = (((a_uiTileId) & 0x3FF) |
		(((a_uiPalBank) & 15) << 12) |
		(((a_uiPriority) & 3) << 10));
	return attrib2;
}