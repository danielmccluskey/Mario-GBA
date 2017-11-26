#include "DM_SpriteManager.h"
#include "gba_reg.h"
#include <string.h>
#include "gba_gfx.h"


void SpriteManager::CreateSprite(u16* a_Tiles, u16* a_Palette, s32 a_TileSize, s32 a_PaletteSize, s32 a_TileBlock)
{
	memcpy(pal_sp_mem, a_Palette, a_PaletteSize);
	memcpy(&tile_mem[a_TileBlock][0], a_Tiles, a_TileSize);


	//SpriteInformation[SpriteIndex]->i_x = 50;
	//SpriteInformation[SpriteIndex]->i_y = 50;


	SpriteArray[SpriteIndex] = &MEM_OAM[0];

	SpriteArray[SpriteIndex]->attr0 = setSpriteAttr0(0, 0, 0, 0, A0_4BPP, A0_SQUARE);
	SpriteArray[SpriteIndex]->attr1 = setSpriteAttr1(0, 0, 0, 0, 1);
	SpriteArray[SpriteIndex]->attr2 = 0;



	SpriteIndex++;

}

void SpriteManager::InitialiseArray()
{
	for (int i = 0; i < 128; ++i)
	{
		SpriteArray[i] = &MEM_OAM[i];

		SpriteArray[i]->attr0 = setSpriteAttr0(0, 2, 0, 0, A0_4BPP, A0_SQUARE);
		SpriteArray[i]->attr1 = setSpriteAttr1(0, 0, 0, 0, 1);
		SpriteArray[i]->attr2 = 0;
	}
}


void SpriteManager::SetHFlip(bool a_bFlip, s32 a_SpriteID)
{
	if (a_bFlip)
	{
		SpriteArray[a_SpriteID]->attr1 |= 0x1000;
	}
	else
	{
		SpriteArray[a_SpriteID]->attr1 &= 0xefff;
	}
}
void SpriteManager::SetVFlip(bool a_bFlip, s32 a_SpriteID)
{
	if (a_bFlip)
	{
		SpriteArray[a_SpriteID]->attr1 |= 0x2000;
	}
	else
	{
		SpriteArray[a_SpriteID]->attr1 &= 0xdfff;
	}
}
void SpriteManager::SetFrame(s32 a_iframe, s32 a_iSpriteID)
{

	SpriteArray[a_iSpriteID]->attr2 &= 0xfc00;
	SpriteArray[a_iSpriteID]->attr2 |= (a_iframe & 0x03ff);
}
void SpriteManager::MoveSprite(s16 a_ix, s16 a_iy, s32 a_SpriteID)
{
	SpriteArray[a_SpriteID]->attr0 = (SpriteArray[a_SpriteID]->attr0 & 0xFF00) | (a_iy & 0x00FF);
	SpriteArray[a_SpriteID]->attr1 = (SpriteArray[a_SpriteID]->attr1 & 0xFE00) | (a_ix & 0x01FF);

	//SpriteInformation[a_SpriteID]->i_y = a_iy;
	//SpriteInformation[a_SpriteID]->i_x = a_ix;
}

u16 SpriteManager::setSpriteAttr0(u16 a_y, u8 a_objectMode, u8 a_gfxMode, u8 a_mosaic, u8 a_colormode, u8 a_shape)
{
	u16 attrib0 = a_y & A0_YMASK |
		((a_objectMode & A0_MODE_MASK) << 8) |
		((a_gfxMode & 0x3) << 10) |
		((a_mosaic & 1) << 12) |
		((a_colormode & 1) << 13) |
		((a_shape & 0x3) << 14);
	return attrib0;
}
u16 SpriteManager::setSpriteAttr1(u16 a_x, u8 a_affine, u8 a_horiFlip, u8 a_vertFlip, u8 a_size)
{
	u16 attrib1 = a_x & A0_YMASK |
		((a_affine & 0x3) << 9) |
		((a_horiFlip & 0x3) << 12) |
		((a_vertFlip & 0x1) << 13) |
		((a_size & 0x1) << 14);
	return attrib1;
}
u16 SpriteManager::setSpriteAttr2(u32 a_tileId, u32 a_palBank, u32 a_priority)
{
	u16 attrib2 = (((a_tileId) & 0x3FF) |
		(((a_palBank) & 15) << 12) |
		(((a_priority) & 3) << 10));
	return attrib2;
}



void SpriteManager::UpdateSprites()
{
	//memcpy((u16*)MEM_OAM, (u16*)SpriteArray, 128);
}