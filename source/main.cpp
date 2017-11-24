#include "Intellisense.h"
#include "gba.h"

#include <string.h>
#include "DM_SpriteManager.h"
#include "DM_MarioManager.h"
#include "DM_TileManager.h"


//a small function to set up the background control register with the input parameters passed through as u8's
vu16 setBG_Control_Register(u8 a_priority, u8 a_tileBlockID, u8 a_mosaic, u8 a_colourMode, u8 a_mapBlockID, u8 a_affineWrap, u8 a_bgSize)
{
	vu16 control = (a_priority & BGCNT_PRIORITY_MASK) | ((a_tileBlockID & BGCNT_TBID_MASK) << 2) | ((a_mosaic & BGCNT_MOS_MASK) << 6) | ((a_colourMode & BGCNT_COLOR_MASK) << 7) |
		((a_mapBlockID & BGCNT_TMID_MASK) << 8) | ((a_affineWrap & BGCNT_AFW_MASK) << 13) | ((a_bgSize & BGCNT_SIZE_MASK) << 14);
	return control;
}


int main()
{

	REG_BGCNT[0] = setBG_Control_Register(0, 0, 0, 0, 15, 0, BG_REG_SIZE_32x32);
	REG_DISPLAYCONTROL = VIDEOMODE_0 | ENABLE_OBJECTS | BGMODE_0 | MAPPINGMODE_1D;

	SpriteManager Spritemanager;
	Spritemanager.InitialiseArray();




	MarioManager MarioSprite;
	MarioSprite.CreateMario(Spritemanager);
	

	TileManager Tilemanager;
	
	Tilemanager.SetupBG(0,0);
	Tilemanager.SetPos(0,44);
	
	s32 frame = 0;
	while (1)
	{
		vsync();
		PollKeys();
		MarioSprite.iMapOffset = Tilemanager.i_x;
		if (fix2int(MarioSprite.ix) > 40 && keyDown(KEYS::LEFT))
		{
			MarioSprite.MoveMario(-100, 0, Spritemanager);
			Spritemanager.SetHFlip(true, MarioSprite.iSpriteID);
		}
		if (fix2int(MarioSprite.ix) < 160 && keyDown(KEYS::RIGHT))
		{
			MarioSprite.MoveMario(100, 0, Spritemanager);
			Spritemanager.SetHFlip(false, MarioSprite.iSpriteID);
		}
		if (keyDown(KEYS::UP))
		{
			MarioSprite.bJump = true;
		}
		if (keyDown(KEYS::DOWN))
		{			
			MarioSprite.MoveMario(int2fix(0), int2fix(1), Spritemanager);			
		}		
		if (fix2int(MarioSprite.ix) <= 80 && Tilemanager.i_x >= 1 && keyDown(KEYS::LEFT))
		{
			Tilemanager.i_x -= 2;
			MarioSprite.iVelocityX = int2fix(0);
			Tilemanager.scroll_x-=2;
			Tilemanager.left = true;

		}
		if (fix2int(MarioSprite.ix) >= 160 && keyDown(KEYS::RIGHT) && Tilemanager.scroll_x <= 3392-248)// Tilemanager.i_x <= 424 - 31 &&
		{
			MarioSprite.iVelocityX = int2fix(0);
			Tilemanager.i_x += 2;
			Tilemanager.scroll_x+=2;
			Tilemanager.right = true;
		}
		MarioSprite.UpdateMario(Spritemanager);

		Tilemanager.ScrollBackGround();
	}//loop forever

	return 0;
}