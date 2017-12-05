#include "Intellisense.h"
#include "gba.h"

#include <string.h>
#include "DM_SpriteManager.h"
#include "DM_MarioManager.h"
#include "DM_TileManager.h"
#include "gba_sound.h"
#include "rocket4bpp.h"
#include "DM_AIManager.h"

u32 __snd_rates[12];

//a small function to set up the background control register with the input parameters passed through as u8's
vu16 setBG_Control_Register(u8 a_priority, u8 a_tileBlockID, u8 a_mosaic, u8 a_colourMode, u8 a_mapBlockID, u8 a_affineWrap, u8 a_bgSize)
{
	vu16 control = (a_priority & BGCNT_PRIORITY_MASK) | ((a_tileBlockID & BGCNT_TBID_MASK) << 2) | ((a_mosaic & BGCNT_MOS_MASK) << 6) | ((a_colourMode & BGCNT_COLOR_MASK) << 7) |
		((a_mapBlockID & BGCNT_TMID_MASK) << 8) | ((a_affineWrap & BGCNT_AFW_MASK) << 13) | ((a_bgSize & BGCNT_SIZE_MASK) << 14);
	return control;
}
// Show the octave the next note will be in
void note_prep(int octave)
{
	//tte_printf("#{P:8,%d;cx:0x1000}[  %+2d]", txt_scrolly, octave);
}


// Play a note and show which one was played
void note_play(int note, int octave)
{
	// Clear next top and current rows
	//SBB_CLEAR_ROW(31, (txt_scrolly / 8 - 2) & 31);
	//SBB_CLEAR_ROW(31, txt_scrolly / 8);

	// Display note and scroll
	//tte_printf("#{P:16,%d;cx:0}%-2s%+2d", txt_scrolly, names[note], octave);
	//se_puts(16, txt_scrolly, str, 0);

	//txt_scrolly -= 8;
	//REG_BG0VOFS = txt_scrolly - 8;

	// Play the actual note
	REG_SND1FREQ = SFREQ_RESET | SND_RATE(note, octave, __snd_rates);
}

void delay(unsigned int amount) {
	for (int i = 0; i < amount * 10; i++);
}
void sos()
{
	const u8 lens[6] = { 1,1,4, 1,1,4 };
	//const u8 notes[6] = { 0x02, 0x05, 0x12,  0x02, 0x05, 0x12 };
	int ii;
	for (ii = 0; ii<7400; ii++)
	{
		note_play(aggg[ii+1] & 15, 12);
		//VBlankIntrDelay(8 * lens[ii]);
		delay(1);
	}
}




int main()
{

	REG_BGCNT[0] = setBG_Control_Register(0, 0, 0, 0, 15, 0, BG_REG_SIZE_32x32);
	REG_DISPLAYCONTROL = VIDEOMODE_0 | ENABLE_OBJECTS | BGMODE_0 | MAPPINGMODE_1D;

	SpriteManager Spritemanager;
	Spritemanager.InitialiseArray();




	MarioManager MarioSprite;
	MarioSprite.CreateMario(Spritemanager);

	AIManager EnemyArray[MAX_ENEMIES];
	EnemyArray[0].CreateEnemy(Spritemanager, EnemyArray, 0);
	
	
	int octave = 0;

	// turn sound on
	REG_SNDSTAT = SSTAT_ENABLE;
	// snd1 on left/right ; both full volume
	REG_SNDDMGCNT = SDMG_BUILD_LR(SDMG_SQR1, 7);
	// DMG ratio to 100%
	REG_SNDDSCNT = SDS_DMG100;

	// no sweep
	REG_SND1SWEEP = SSW_OFF;
	// envelope: vol=12, decay, max step time (7) ; 50% duty
	REG_SND1CNT = SSQR_ENV_BUILD(12, 10, 1) | SSQR_DUTY3_4;
	REG_SND1FREQ = 0;
	sos();

	TileManager Tilemanager;	
	Tilemanager.SetupBG(0,0);
	Tilemanager.SetPos(0,44);
	
	s32 frame = 0;
	while (1)
	{
		vsync();
		PollKeys();

		
		EnemyArray[0].UpdateOffset(EnemyArray, Tilemanager.i_x, Tilemanager.i_y);
		MarioSprite.iMapOffsetX = Tilemanager.i_x;
		MarioSprite.iMapOffsetY = Tilemanager.i_y;
		
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
		if (keyHit(KEYS::UP))
		{
			MarioSprite.bJump = true;
			
			note_play(0x12, 0);
		}
		if (keyHit(KEYS::DOWN))
		{			
			MarioSprite.ShootFireBall(Spritemanager);
			EnemyArray[0].CreateEnemy(Spritemanager, EnemyArray, 0);
		}		
		if (fix2int(MarioSprite.ix) <= 4 && Tilemanager.i_x >= 1 && keyDown(KEYS::LEFT))
		{
			MarioSprite.iVelocityX = int2fix(0);
		}
		if (fix2int(MarioSprite.ix) >= 160 && keyDown(KEYS::RIGHT) && Tilemanager.scroll_x <= 3392-248)// Tilemanager.i_x <= 424 - 31 &&
		{
			MarioSprite.iVelocityX = int2fix(0);
			Tilemanager.right = true;
		}
		MarioSprite.UpdateMario(Spritemanager);
		EnemyArray[0].UpdateEnemies(Spritemanager, EnemyArray);
		Tilemanager.ScrollBackGround((bool*)MarioSprite.AlmostBotLeft, (bool*)MarioSprite.AlmostBotRight);

		delay(200);
	}//loop forever

	return 0;
}