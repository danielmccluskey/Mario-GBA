#ifndef _SPRITEMANAGER_H_
#define _SPRITEMANAGER_H_


#include "gba_types.h"
#include "gba_gfx.h"

class SpriteManager
{
public:
	s32 testx = 0;
	//SpriteInfo* SpriteInformation[128];
	volatile ObjectAttribute* SpriteArray[128];

	s32 SpriteIndex = 0;
	void CreateSprite(u16* a_Tiles, u16* a_Palette, s32 a_TileSize, s32 a_PaletteSize, s32 a_TileBlock);
	void InitialiseArray();
	void HideSprite();
	void ShowSprite();
	void SetHFlip(bool a_bFlip, s32 a_SpriteID);
	void SetVFlip(bool a_bFlip, s32 a_SpriteID);
	void MoveSprite(s16 a_ix, s16 a_iy, s32 a_SpriteID);
	void UpdateSprites();
	void TranslateSprite(s32 a_ix, s32 a_iy, s32 a_SpriteID);
	u16 setSpriteAttr0(u16 a_y, u8 a_objectMode, u8 a_gfxMode, u8 a_mosaic, u8 a_colormode, u8 a_shape);
	u16 setSpriteAttr1(u16 a_x, u8 a_affine, u8 a_horiFlip, u8 a_vertFlip, u8 a_size);

};




#endif // !_SPRITEMANAGER_H_
