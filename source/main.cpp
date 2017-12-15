#include "Intellisense.h"
#include "gba.h"

#include "DM_SpriteManager.h"
#include "DM_MarioManager.h"
#include "DM_Tilemanager.h"
#include "DM_AIManager.h"
#include "World1Map_Externs.h"
#include "World1Level1_Externs.h"
#include "World1Level2_Externs.h"
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

enum LEVELUNLOCKS
{
	LVL1X = 8,
	LVL1Y = 1, 
	LVL2X = 16,
	LVL2Y = 1, 
	LVL3X = 0,
	LVL3Y = 0,
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



void CopyCollision(const unsigned short* a_iROMMap, unsigned short* a_iIWRAMMap, s32 a_iLength)
{
	for (int i = 0; i < a_iLength; i++)
	{
		a_iIWRAMMap[i] = a_iROMMap[i];
	}
}

int main()
{
	s32 iGameState = WORLDMAPINIT;
	REG_BGCNT[0] = setBG_Control_Register(0, 0, 0, 0, 15, 0, BG_REG_SIZE_32x32);
	REG_DISPLAYCONTROL = VIDEOMODE_0 | ENABLE_OBJECTS | BGMODE_0 | MAPPINGMODE_1D;

	SpriteManager Spritemanager;
	Spritemanager.InitialiseArray();




	MarioManager oMarioSprite;
	oMarioSprite.CreateMario(Spritemanager);
	

	AIManager aoEnemyArray[MAX_ENEMIES];
	//aoEnemyArray[0].CreateEnemy(Spritemanager, aoEnemyArray, 3, 240, 160);
	
	
	TileManager oTilemanager;
	PrizeBlockManager aoPrizeManager[MAX_PRIZEBLOCKS];
	
	s32 frame = 0;

	s32 iCheapEnemySpawner = 0;
	s32 iCurrentLevel = 0;

	while (1)
	{
		vsync();
		PollKeys();
		
		
		if (iGameState == WORLDMAPINIT)
		{
			oTilemanager.iCurrentMapArray = (unsigned short*)World1MapMap;
			oTilemanager.SetupBG(0, 0, World1MapTilesA, 1024 * 2, World1MapPalette, 512, World1MapMap, 32);
			iGameState = WORLDMAP;
			oMarioSprite.bMapMode = true;
			oMarioSprite.SetPos(24, 40, Spritemanager);
			oTilemanager.ScrollBackGround(false, false, 0);
		}
		if (iGameState == WORLDMAP)
		{
			oTilemanager.AnimateBackground(World1MapTilesA, World1MapTilesB, World1MapTilesC, World1MapTilesD);
			u8 LevelSelected = oMarioSprite.MapManager(World1MapCollision, Spritemanager);

			if (LevelSelected == 1)
			{
				iCurrentLevel = LevelSelected;
				iGameState = LOAD_LEVEL1;
			}
			else if (LevelSelected == 2)
			{
				iCurrentLevel = LevelSelected;
				iGameState = LOAD_LEVEL2;
			}

		}
		if (iGameState == LOAD_LEVEL1)
		{
			oTilemanager.iCurrentMapArray = (unsigned short*)World1Level1Map;
			oTilemanager.SetupBG(2, 12, World1Level1Tiles, 816 * 2, World1Level1Palette, 32, World1Level1Map, 424);
			oTilemanager.SetPos(2, 12, World1Level1Map, 424);			
			CopyCollision(World1Level1Collision, iCurrentCollisionMap, 13568);
			oMarioSprite.apiMarioBGCollision = iCurrentCollisionMap;
			aoEnemyArray[0].iEnemyBGCollision = iCurrentCollisionMap;


			iGameState = GAMEINIT;

		}
		if (iGameState == LOAD_LEVEL2)
		{
			oTilemanager.iCurrentMapArray = (unsigned short*)World1Level2Map;
			oTilemanager.SetupBG(2, 12, World1Level2Tiles, 816 * 2, World1Level2Palette, 32, World1Level2Map, 424);
			oTilemanager.SetPos(2, 12, World1Level2Map, 424);
			CopyCollision(World1Level2Collision, iCurrentCollisionMap, 13568);
			oMarioSprite.apiMarioBGCollision = iCurrentCollisionMap;
			aoEnemyArray[0].iEnemyBGCollision = iCurrentCollisionMap;


			iGameState = GAMEINIT;

		}
		if (iGameState == GAMEINIT)
		{			
			oMarioSprite.SetPos(40, 32, Spritemanager);
			oMarioSprite.MoveMario(100, 0, Spritemanager);
			iGameState = GAME;

		}







		if (iGameState == GAME)
		{
			aoEnemyArray[0].iMapOffsetX = oTilemanager.i_x;
			aoEnemyArray[0].iMapOffsetY = oTilemanager.i_y;
			oMarioSprite.iMapOffsetX = oTilemanager.i_x;
			oMarioSprite.iMapOffsetY = oTilemanager.i_y;
			oMarioSprite.bMoving = false;


			if (fix2int(oMarioSprite.ix) > 40 && keyDown(KEYS::LEFT))
			{
				oMarioSprite.MoveMario(-100, 0, Spritemanager);
				Spritemanager.SetHFlip(true, oMarioSprite.iSpriteID);
			}
			if (fix2int(oMarioSprite.ix) < 160 && keyDown(KEYS::RIGHT))
			{
				oMarioSprite.MoveMario(100, 0, Spritemanager);
				Spritemanager.SetHFlip(false, oMarioSprite.iSpriteID);
			}
			if (keyHit(KEYS::UP))
			{
				oMarioSprite.bJump = true;

			}
			
			if (fix2int(oMarioSprite.ix) <= 4 && oTilemanager.i_x >= 1 && keyDown(KEYS::LEFT))
			{
				oMarioSprite.fpVelocityX = 0;
			}
			if (fix2int(oMarioSprite.ix) >= 160 && keyDown(KEYS::RIGHT) && oTilemanager.scroll_x <= 3392 - 248)
			{
				oMarioSprite.fpVelocityX = 0;
				oTilemanager.right = true;
				if (!((bool*)oMarioSprite.uiAlmostBotRight))
				{
					iCheapEnemySpawner += 2;
					oMarioSprite.bMoving = true;
					aoPrizeManager[0].MoveBlocks(Spritemanager, aoPrizeManager, 2);
					aoEnemyArray[0].ScrollEnemies(Spritemanager, aoEnemyArray, 2);
				}
				

			}

			if (iCheapEnemySpawner >= 250)
			{
				aoEnemyArray[0].CreateEnemy(Spritemanager, aoEnemyArray, qran_range(0, 3), 240, int2fix(100));
				iCheapEnemySpawner = 0;
			}
			
			oMarioSprite.UpdateMario(Spritemanager, aoPrizeManager, oTilemanager.iScrollOffset);



			aoEnemyArray[0].UpdateEnemies(Spritemanager, aoEnemyArray);
			oTilemanager.ScrollBackGround((bool*)oMarioSprite.uiAlmostBotLeft, (bool*)oMarioSprite.uiAlmostBotRight, World1Level1Map);
			

			
			
			u16 bKill = aoEnemyArray[0].CheckSpriteCollision(Spritemanager, aoEnemyArray, oMarioSprite.ix, oMarioSprite.iy, oMarioSprite.iSpriteWidth, oMarioSprite.iSpriteHeight, oMarioSprite.bInvulnerable);
			if (bKill != 15)
			{
				
				if (bKill <= 2 && !oMarioSprite.bInvulnerable)
				{
					oMarioSprite.TransformMario(0, Spritemanager, true);
					oMarioSprite.bInvulnerable = true;
					//aoEnemyArray[0].CreateEnemy(Spritemanager, aoEnemyArray, 0, 240, 00);
				}
				else if (bKill == 3)
				{
					oMarioSprite.TransformMario(1, Spritemanager, false);
				}
				else if (bKill == 4)
				{
					oMarioSprite.TransformMario(2, Spritemanager, false);
				}
			}

			oMarioSprite.CheckFireballCollisions(Spritemanager, aoEnemyArray);
			aoPrizeManager[0].SpawnPowerUp(aoPrizeManager, Spritemanager, aoEnemyArray);

			if (oMarioSprite.bFinished == true || oMarioSprite.bDead == true)
			{
				if (oMarioSprite.bFinished)
				{
					if (iCurrentLevel == 1)
					{
						World1MapCollision[LVL1Y * 32 + LVL1X] = 0x0000;
					}
					else if (iCurrentLevel == 2)
					{
						World1MapCollision[LVL2Y * 32 + LVL2X] = 0x0000;

					}
				}
				oMarioSprite.ResetMario(Spritemanager);
				oMarioSprite.TransformMario(0, Spritemanager, false);
				aoEnemyArray[0].DeleteEnemies(Spritemanager, aoEnemyArray);

				iGameState = WORLDMAPINIT;
			}


			if (keyHit(KEYS::B))
			{
				//oMarioSprite.ShootFireBall(Spritemanager);
				aoEnemyArray[0].CreateEnemy(Spritemanager, aoEnemyArray, 3, 240, 00);
				//aoPrizeManager[0].CreateBlock(oMarioSprite.ix, oMarioSprite.iy+10, aoPrizeManager, Spritemanager, oTilemanager.iScrollOffset, false);

			}
			if (keyHit(KEYS::A))
			{
				//oMarioSprite.ShootFireBall(Spritemanager);
				aoEnemyArray[0].CreateEnemy(Spritemanager, aoEnemyArray, 1, 240, 00);
				//aoPrizeManager[0].CreateBlock(oMarioSprite.ix, oMarioSprite.iy+10, aoPrizeManager, Spritemanager, oTilemanager.iScrollOffset, false);

			}
			if (keyHit(KEYS::DOWN))
			{
				oMarioSprite.ShootFireBall(Spritemanager);
				//aoEnemyArray[0].CreateEnemy(Spritemanager, aoEnemyArray, 3, 240, 00);
				//aoPrizeManager[0].CreateBlock(oMarioSprite.ix, oMarioSprite.iy+10, aoPrizeManager, Spritemanager, oTilemanager.iScrollOffset, false);

			}
			
		}
		
	}
	return 0;
}