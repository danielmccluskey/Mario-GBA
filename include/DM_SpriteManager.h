#ifndef _SPRITEMANAGER_H_
#define _SPRITEMANAGER_H_


#include "gba_types.h"
#include "gba_gfx.h"

class SpriteManager
{
public:
	volatile ObjectAttribute* SpriteArray[128];
	bool OccupiedSprite[128];
	s32 SpriteIndex = 0;
	u16 CreateSprite(u16* a_Tiles, u16* a_Palette, s32 a_TileSize, s32 a_PaletteSize, s32 a_TileBlock, s32 a_PaletteBank);
	void DeleteSprite(s32 a_iSpriteID);
	void LoadTiles(u16* a_Tiles, u16* a_Palette, s32 a_TileSize, s32 a_PaletteSize, s32 a_TileBlock, s32 a_PaletteBank);
	void InitialiseArray();
	void HideSprite(s32 a_SpriteID);
	void ShowSprite(s32 a_SpriteID);
	void SetHFlip(bool a_bFlip, s32 a_SpriteID);
	void SetVFlip(bool a_bFlip, s32 a_SpriteID);
	void SetFrame(s32 a_iframe, s32 a_iSpriteID);
	void MoveSprite(s16 a_ix, s16 a_iy, s32 a_SpriteID);
	void UpdateSprites();
	u16 setSpriteAttr0(u16 a_uiY, u8 a_uiObjectMode, u8 a_uiGfxMode, u8 a_uiMosaic, u8 a_uiColormode, u8 a_uiShape);
	u16 setSpriteAttr1(u16 a_uiX, u8 a_uiAffine, u8 a_uiHoriFlip, u8 a_uiVertFlip, u8 a_uiSize);
	u16 setSpriteAttr2(u32 a_uiTileId, u32 a_uiPalBank, u32 a_uiPriority);

};




#endif // !_SPRITEMANAGER_H_
