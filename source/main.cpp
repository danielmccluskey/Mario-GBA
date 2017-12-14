#include "Intellisense.h"
#include "gba.h"

#include "DM_SpriteManager.h"
#include "DM_MarioManager.h"
#include "DM_TileManager.h"
#include "DM_AIManager.h"
#include "World1Map_Externs.h"
#include "World1Level1_Externs.h"
#include "DM_Enums.h"


enum GAMESTATES
{
	MENUINIT,
	MENU,
	WORLDMAPINIT,
	WORLDMAP,
	GAMEINIT,
	GAME,
	MARIOFLASH,
	LOAD_LEVEL1,
	LOAD_LEVEL2,
	LOAD_LEVEL3,
	LOAD_LEVEL4,
	LOAD_LEVEL5
};

//a small function to set up the background control register with the input parameters passed through as u8's
vu16 setBG_Control_Register(u8 a_priority, u8 a_tileBlockID, u8 a_mosaic, u8 a_colourMode, u8 a_mapBlockID, u8 a_affineWrap, u8 a_bgSize)
{
	vu16 control = (a_priority & BGCNT_PRIORITY_MASK) | ((a_tileBlockID & BGCNT_TBID_MASK) << 2) | ((a_mosaic & BGCNT_MOS_MASK) << 6) | ((a_colourMode & BGCNT_COLOR_MASK) << 7) |
		((a_mapBlockID & BGCNT_TMID_MASK) << 8) | ((a_affineWrap & BGCNT_AFW_MASK) << 13) | ((a_bgSize & BGCNT_SIZE_MASK) << 14);
	return control;
}

void delay(int a_iDelay)
{
	for (int i = 0; i < a_iDelay; i++)
	{

	}
}



int main()
{
	s32 iGameState = WORLDMAPINIT;
	REG_BGCNT[0] = setBG_Control_Register(0, 0, 0, 0, 15, 0, BG_REG_SIZE_32x32);
	REG_DISPLAYCONTROL = VIDEOMODE_0 | ENABLE_OBJECTS | BGMODE_0 | MAPPINGMODE_1D;

	SpriteManager Spritemanager;
	Spritemanager.InitialiseArray();




	MarioManager MarioSprite;
	MarioSprite.CreateMario(Spritemanager);
	

	AIManager EnemyArray[MAX_ENEMIES];
	//EnemyArray[0].CreateEnemy(Spritemanager, EnemyArray, 3, 240, 160);
	
	
	TileManager Tilemanager;
	PrizeBlockManager PrizeManager[MAX_PRIZEBLOCKS];
	
	s32 frame = 0;
	while (1)
	{
		vsync();
		PollKeys();
		
		
		if (iGameState == WORLDMAPINIT)
		{
			Tilemanager.SetupBG(0, 0, World1MapTilesA, 1024 * 2, World1MapPalette, 512, World1MapMap, 32);
			iGameState = WORLDMAP;
			MarioSprite.bMapMode = true;
			MarioSprite.SetPos(24, 40, Spritemanager);
		}
		if (iGameState == WORLDMAP)
		{
			Tilemanager.AnimateBackground(World1MapTilesA, World1MapTilesB, World1MapTilesC, World1MapTilesD);
			u8 LevelSelected = MarioSprite.MapManager(World1MapCollision, Spritemanager);

			if (LevelSelected == 1)
			{
				iGameState = LOAD_LEVEL1;
			}

		}
		if (iGameState == LOAD_LEVEL1)
		{
			Tilemanager.SetupBG(2, 12, World1Level1Tiles, 816 * 2, World1Level1Palette, 32, World1Level1Map, 424);
			Tilemanager.SetPos(2, 12, World1Level1Map, 424);
			iGameState = GAMEINIT;
		}
		if (iGameState == GAMEINIT)
		{			
			MarioSprite.SetPos(40, 32, Spritemanager);
			MarioSprite.MoveMario(100, 0, Spritemanager);
			iGameState = GAME;

		}







		if (iGameState == GAME)
		{
			EnemyArray[0].iMapOffsetX = Tilemanager.i_x;
			EnemyArray[0].iMapOffsetY = Tilemanager.i_y;
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

			}
			
			if (fix2int(MarioSprite.ix) <= 4 && Tilemanager.i_x >= 1 && keyDown(KEYS::LEFT))
			{
				MarioSprite.iVelocityX = 0;
			}
			if (fix2int(MarioSprite.ix) >= 160 && keyDown(KEYS::RIGHT) && Tilemanager.scroll_x <= 3392 - 248)
			{
				MarioSprite.iVelocityX = 0;
				Tilemanager.right = true;
				if (!((bool*)MarioSprite.AlmostBotRight))
				{
					PrizeManager[0].MoveBlocks(Spritemanager, PrizeManager, 2);
					EnemyArray[0].ScrollEnemies(Spritemanager, EnemyArray, 2);
				}
				

			}
			
			MarioSprite.UpdateMario(Spritemanager, PrizeManager, Tilemanager.iScrollOffset);



			EnemyArray[0].UpdateEnemies(Spritemanager, EnemyArray);
			Tilemanager.ScrollBackGround((bool*)MarioSprite.AlmostBotLeft, (bool*)MarioSprite.AlmostBotRight, World1Level1Map);
			

			
			
			u16 bKill = EnemyArray[0].CheckSpriteCollision(Spritemanager, EnemyArray, MarioSprite.ix, MarioSprite.iy, MarioSprite.iSpriteWidth, MarioSprite.iSpriteHeight, MarioSprite.bInvulnerable);
			if (bKill != 15)
			{
				
				if (bKill <= 2 && !MarioSprite.bInvulnerable)
				{
					MarioSprite.TransformMario(0, Spritemanager, true);
					MarioSprite.bInvulnerable = true;
					//EnemyArray[0].CreateEnemy(Spritemanager, EnemyArray, 0, 240, 00);
				}
				else if (bKill == 3)
				{
					MarioSprite.TransformMario(1, Spritemanager, false);
				}
				else if (bKill == 4)
				{
					MarioSprite.TransformMario(2, Spritemanager, false);
				}
			}

			MarioSprite.CheckFireballCollisions(Spritemanager, EnemyArray);
				
			

			


			PrizeManager[0].SpawnPowerUp(PrizeManager, Spritemanager, EnemyArray);


			if (keyHit(KEYS::B))
			{
				//MarioSprite.ShootFireBall(Spritemanager);
				EnemyArray[0].CreateEnemy(Spritemanager, EnemyArray, 3, 240, 00);
				//PrizeManager[0].CreateBlock(MarioSprite.ix, MarioSprite.iy+10, PrizeManager, Spritemanager, Tilemanager.iScrollOffset, false);

			}
			if (keyHit(KEYS::A))
			{
				//MarioSprite.ShootFireBall(Spritemanager);
				EnemyArray[0].CreateEnemy(Spritemanager, EnemyArray, 0, 240, 00);
				//PrizeManager[0].CreateBlock(MarioSprite.ix, MarioSprite.iy+10, PrizeManager, Spritemanager, Tilemanager.iScrollOffset, false);

			}
			if (keyHit(KEYS::DOWN))
			{
				MarioSprite.ShootFireBall(Spritemanager);
				//EnemyArray[0].CreateEnemy(Spritemanager, EnemyArray, 3, 240, 00);
				//PrizeManager[0].CreateBlock(MarioSprite.ix, MarioSprite.iy+10, PrizeManager, Spritemanager, Tilemanager.iScrollOffset, false);

			}
			
		}
		
	}
	return 0;
}