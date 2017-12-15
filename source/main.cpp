//==============================================================================================================================
// Project: Mario GBA
// File: main.cpp
// Author: Daniel McCluskey
// Date Created: 01/12/17
// Brief: Welcome to Lag-a-Thon 5000.
// Brief: There are a lot of things I wanted to finish such as turtles, title screen and on screen text
// Brief: but I ran out of time.
// Brief: Currently, as the game is, there is a source of lag that I can't locate which is probably particles.
// Brief: 
// Brief: There are 2 levels, to get to level 2, you have to finish level 1.
// Brief: 
// Brief: 
// Last Edited by: (See BitBucket Commits: https://bitbucket.org/Danielmclovin/gba-mario
//==============================================================================================================================
#include "Intellisense.h"
#include "gba.h"

#include "DM_SpriteManager.h"
#include "DM_MarioManager.h"
#include "DM_Tilemanager.h"
#include "DM_AIManager.h"
#include "World1Map_Externs.h"
#include "World1Level1_Externs.h"
#include "World1Level2_Externs.h"

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

//Coordinates for letting the player select the next level on the world map
enum LEVELUNLOCKS
{
	LVL1X = 8,
	LVL1Y = 1,
	LVL2X = 16,
	LVL2Y = 1,
	LVL3X = 0,
	LVL3Y = 0,
};

//Small Function to copy each value from one array to another, used for updating collision maps for player, Arrays must be same size.
//const unsigned short* a_iROMMap = The const map from ROM Memory or Cart memory that you wish to copy
//unsigned short* a_iIWRAMMap = The Map that you wish to copy to, can be edited.
//s32 a_iLength = Length of both maps
void CopyCollision(const unsigned short* a_iROMMap, unsigned short* a_iIWRAMMap, s32 a_iLength)
{
	for (int i = 0; i < a_iLength; i++)
	{
		a_iIWRAMMap[i] = a_iROMMap[i];
	}
}

int main()
{
	s32 iGameState = WORLDMAPINIT;//Set current game state to initialise the world map//Set current game state to initialise the world map

	REG_BGCNT[0] = setBG_Control_Register(0, 0, 0, 0, 15, 0, BG_REG_SIZE_32x32);//Set the background control registers
	REG_DISPLAYCONTROL = VIDEOMODE_0 | ENABLE_OBJECTS | BGMODE_0 | MAPPINGMODE_1D;//Set the dispaly registers

	SpriteManager Spritemanager;//Create a sprite manager class member
	Spritemanager.InitialiseArray();//Initialise the sprite array

	MarioManager oMarioSprite;//Create a Mario sprite
	oMarioSprite.CreateMario(Spritemanager);//Initilise the sprite

	AIManager aoEnemyArray[MAX_ENEMIES];//Create an enemy array (Powerups and enemies)

	TileManager oTilemanager;//Create a tile manager class member
	PrizeBlockManager aoPrizeManager[MAX_PRIZEBLOCKS];//Create an array of prize blocks

	s32 iCheapEnemySpawner = 0;//Very cheap way of spawning mobs, basically just spawns them every time the map gets scrolled a certain amount
	s32 iCurrentLevel = 0;//Sets the current level

	while (1)
	{
		vsync();//Waits for vsync
		PollKeys();//Polls the keys

		if (iGameState == WORLDMAPINIT)//if the game state is Initialise worldmap
		{
			oTilemanager.piCurrentMapArray = (unsigned short*)World1MapMap;//Set the current map for tile manager to the World map
			oTilemanager.SetupBG(0, 0, World1MapTilesA, 1024 * 2, World1MapPalette, 512, World1MapMap, 32);//Setup the background
			oMarioSprite.bMapMode = true;//Turn map mode on for mario
			oMarioSprite.SetPos(24, 40, Spritemanager);//Set the position of mario
			oTilemanager.ScrollBackGround(false, false);//Scroll the background once to fix alignment errors

			iGameState = WORLDMAP;//Set the next game state to World map
		}
		if (iGameState == WORLDMAP)//If the game state is world map
		{
			oTilemanager.AnimateBackground(World1MapTilesA, World1MapTilesB, World1MapTilesC, World1MapTilesD);//Animate the background
			u8 LevelSelected = oMarioSprite.MapManager(World1MapCollision, Spritemanager);//Move mario around and check if the user has selected a level

			if (LevelSelected == 1)//If the selected level is 1
			{
				iCurrentLevel = LevelSelected;//Set current level
				iGameState = LOAD_LEVEL1;//Load level
			}
			else if (LevelSelected == 2)//If the selected level is 2
			{
				iCurrentLevel = LevelSelected;//Set current level
				iGameState = LOAD_LEVEL2;//Load level
			}
		}
		if (iGameState == LOAD_LEVEL1)//if Game state is load level 1
		{
			oTilemanager.piCurrentMapArray = (unsigned short*)World1Level1Map;//Set the current map tiles  to level 1 map
			oTilemanager.SetupBG(2, 12, World1Level1Tiles, 816 * 2, World1Level1Palette, 32, World1Level1Map, 424);//Load level 1 map and palette
			oTilemanager.SetPos(2, 12, 424);//Set the position of the map magic numbers
			CopyCollision(World1Level1Collision, iCurrentCollisionMap, 13568);//Copy the collision map from ROM
			oMarioSprite.apiMarioBGCollision = iCurrentCollisionMap;//Set marios collision map to the RAM collision map
			aoEnemyArray[0].iEnemyBGCollision = iCurrentCollisionMap;//Set the enemies collision map to the RAM Collision map

			iGameState = GAMEINIT;//Next game state
		}
		if (iGameState == LOAD_LEVEL2)//if Game state is load level 2
		{
			oTilemanager.piCurrentMapArray = (unsigned short*)World1Level2Map;//Set the current map tiles  to level 2 map
			oTilemanager.SetupBG(2, 12, World1Level2Tiles, 816 * 2, World1Level2Palette, 32, World1Level2Map, 424);//Load level 2 map and palette
			oTilemanager.SetPos(2, 12, 424);//Set the position of the map magic numbers
			CopyCollision(World1Level2Collision, iCurrentCollisionMap, 13568);//Copy the collision map from ROM
			oMarioSprite.apiMarioBGCollision = iCurrentCollisionMap;//Set marios collision map to the RAM collision map
			aoEnemyArray[0].iEnemyBGCollision = iCurrentCollisionMap;//Set the enemies collision map to the RAM Collision map

			iGameState = GAMEINIT;//Next game state
		}
		if (iGameState == GAMEINIT)//If set to initialise game
		{
			oMarioSprite.SetPos(40, 32, Spritemanager);//Set marios position
			oMarioSprite.MoveMario(100, 0, Spritemanager);//Pushes mario slightly forward
			iGameState = GAME;//NExt game state
		}

		if (iGameState == GAME)//If set to run game
		{
			//The following copies the map offsets to the class members for collision.
			aoEnemyArray[0].iMapOffsetX = oTilemanager.i_x;
			aoEnemyArray[0].iMapOffsetY = oTilemanager.i_y;
			oMarioSprite.iMapOffsetX = oTilemanager.i_x;
			oMarioSprite.iMapOffsetY = oTilemanager.i_y;

			oMarioSprite.bMoving = false;//Reset moving flag

			if (fix2int(oMarioSprite.ix) > 40 && keyDown(KEYS::LEFT))//If mario is not at the left edge and is pressing left
			{
				oMarioSprite.MoveMario(-100, 0, Spritemanager);//Move mario using physics
				Spritemanager.SetHFlip(true, oMarioSprite.iSpriteID);//Turn mario around
			}
			if (fix2int(oMarioSprite.ix) < 160 && keyDown(KEYS::RIGHT))//If mario is not at the right edge and is pressing right
			{
				oMarioSprite.MoveMario(100, 0, Spritemanager);//Move mario using physics
				Spritemanager.SetHFlip(false, oMarioSprite.iSpriteID);//Turn mario around
			}
			if (keyHit(KEYS::UP) || keyHit(KEYS::A))//If player presses UP or A
			{
				oMarioSprite.bJump = true;//Set jump flag
			}
			if (keyHit(KEYS::B))//If player presses UP or A
			{
				oMarioSprite.ShootFireBall(Spritemanager);//Shoot a fireball
			}


			if (fix2int(oMarioSprite.ix) <= 4 && oTilemanager.i_x >= 1 && keyDown(KEYS::LEFT))//If mario is at left edge of screen
			{
				oMarioSprite.fpVelocityX = 0;//Dont move mario
			}
			if (fix2int(oMarioSprite.ix) >= 160 && keyDown(KEYS::RIGHT) && oTilemanager.iScroll_X <= 3392 - 248)//If mario is at the end of the map magic numbers
			{
				oMarioSprite.fpVelocityX = 0;//Dont move mario
				oTilemanager.bRight = true;//MAke the map scroll
				if (!(oMarioSprite.uiAlmostBotRight > 0))//If mario is not colliding
				{
					iCheapEnemySpawner += 2;//Add to the enemy spawn timer
					oMarioSprite.bMoving = true;//Set moving flag for animations
					aoPrizeManager[0].MoveBlocks(Spritemanager, aoPrizeManager, 2);//Scroll the Prizeblocks left
					aoEnemyArray[0].ScrollEnemies(Spritemanager, aoEnemyArray, 2);//Scroll all enemies left
				}
			}

			if (iCheapEnemySpawner >= 250)//If the enemy spawner timer reaches 250
			{
				aoEnemyArray[0].CreateEnemy(Spritemanager, aoEnemyArray, qran_range(0, 3), 240, int2fix(100));//Spawn an enemy
				iCheapEnemySpawner = 0;//Reset timer
			}

			oMarioSprite.UpdateMario(Spritemanager, aoPrizeManager, oTilemanager.iScrollOffset);//Update mario

			aoEnemyArray[0].UpdateEnemies(Spritemanager, aoEnemyArray);//Updates enemies
			oTilemanager.ScrollBackGround(oMarioSprite.uiAlmostBotLeft, oMarioSprite.uiAlmostBotRight);//Scrolls the background

			u16 bKill = aoEnemyArray[0].CheckSpriteCollision(Spritemanager, aoEnemyArray, oMarioSprite.ix, oMarioSprite.iy, oMarioSprite.iSpriteWidth, oMarioSprite.iSpriteHeight, oMarioSprite.bInvulnerable);//Checks if mario is colliding with any of the enemies
			if (bKill != 15)//If mario is colliding with something magic numbers
			{
				if (bKill <= 2 && !oMarioSprite.bInvulnerable)//If mario collides with an enemy and is not invulnerable
				{
					oMarioSprite.TransformMario(0, Spritemanager, true);//Hurt mario
					oMarioSprite.bInvulnerable = true;//Set mario to invulnerable
				}
				else if (bKill == 3)//If mario collides with a mushroom
				{
					oMarioSprite.TransformMario(1, Spritemanager, false);//Turn mario into a mushroom mario
				}
				else if (bKill == 4)//If mario collides with a flower
				{
					oMarioSprite.TransformMario(2, Spritemanager, false);//Turn mario into a flower mario
				}
			}

			oMarioSprite.CheckFireballCollisions(Spritemanager, aoEnemyArray);//Checks if any of the fireballs are colliding with enemies
			aoPrizeManager[0].SpawnPowerUp(aoPrizeManager, Spritemanager, aoEnemyArray);//Checks to see if any of the need spawn flags are active and spawns prize blocks

			if (oMarioSprite.bFinished == true || oMarioSprite.bDead == true)//if mario is dead or has finished the level
			{
				if (oMarioSprite.bFinished)//if mario has finished the level
				{
					if (iCurrentLevel == 1)//If it is the first level
					{
						World1MapCollision[LVL1Y * 32 + LVL1X] = 0x0000;//Unlock the level 2 barrier
					}
					else if (iCurrentLevel == 2)//If it is the second level
					{
						World1MapCollision[LVL2Y * 32 + LVL2X] = 0x0000;//Unlock the level 3 barrier
					}
				}
				oMarioSprite.ResetMario(Spritemanager);//Reset mario
				oMarioSprite.TransformMario(0, Spritemanager, false);//Make mario small again!
				aoEnemyArray[0].DeleteEnemies(Spritemanager, aoEnemyArray);//Deletes all of the enemies

				iGameState = WORLDMAPINIT;//Set current game state to initialise the world map
			}
		}
	}
	return 0;
}