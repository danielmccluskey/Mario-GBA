#include "DM_AIManager.h"
#include "EnemySprites.h"
#include "BG_Collisions.h"

enum EnemyTypes
{
	GOOMBA,
	GREENTURTLE,
	REDTURTLE
};

void AIManager::CreateEnemy(SpriteManager& a_SpriteManager, AIManager* a_EnemyArray, s8 a_iEnemyType)
{
	for (int i = 0; i < MAX_ENEMIES; i++)
	{
		if (a_EnemyArray[i].bActive == false)
		{
			a_EnemyArray[i].iSpriteID = a_SpriteManager.CreateSprite((u16*)EnemySpritesTiles, (u16*)EnemySpritesPal, EnemySpritesTilesLen, EnemySpritesPalLen * 3, EnemyTileBlock, EnemyPalb);
			a_EnemyArray[i].iVelocityX = 0;
			a_EnemyArray[i].iVelocityY = 0;
			a_EnemyArray[i].iMaxVelocityY = 550;
			a_EnemyArray[i].ix = 240;
			a_EnemyArray[i].iy = 0;
			a_EnemyArray[i].bActive = true;
			a_SpriteManager.MoveSprite(a_EnemyArray[i].ix, a_EnemyArray[i].iy, iSpriteID);
			break;
		}
		
	}

	
	

	
}

void AIManager::UpdateOffset(AIManager* a_EnemyArray, s32 a_iOffsetX, s32 a_iOffsetY)
{
	for (int i = 0; i < MAX_ENEMIES; i++)
	{
		if (a_EnemyArray[i].bActive == true)
		{
			a_EnemyArray[i].iMapOffsetX = a_iOffsetX;
			a_EnemyArray[i].iMapOffsetY = a_iOffsetY;
		}
	}


}

u16 AIManager::tile_lookup(u32 x, u32 y, u32 xscroll, u32 yscroll,
	u16* tilemap, u32 tilemap_w, u32 tilemap_h) {

	x += xscroll;
	y += yscroll*8;

	/* convert from screen coordinates to tile coordinates */
	x >>= 3;
	y >>= 3;

	//y = 44 + y;


	/* lookup this tile from the map */
	s32 index = y * tilemap_w + x;

	/* return the tile */
	return tilemap[index];
}

void AIManager::UpdateEnemies(SpriteManager& a_SpriteManager, AIManager* a_EnemyArray)
{
	s32 iTileTest = 0;

	for (int i = 0; i < MAX_ENEMIES; i++)
	{
		if (a_EnemyArray[i].bActive == true)
		{

			s32 iTileX = a_EnemyArray[i].ix >> 8;
			s32 iTileY = a_EnemyArray[i].iy >> 8;


			u8 Bottom = tile_lookup(iTileX, iTileY + 16, iMapOffsetX,
				iMapOffsetY, (u16*)bgCollision, 424, 64);
			u8 Left = tile_lookup(iTileX, iTileY + 8, iMapOffsetX,
				iMapOffsetY, (u16*)bgCollision, 424, 64);
			u8 Right = tile_lookup(iTileX+ 16, iTileY + 8, iMapOffsetX,
				iMapOffsetY, (u16*)bgCollision, 424, 64);

			u8 BottomRight = tile_lookup(iTileX + iSpriteWidth, iTileY + iSpriteHeight, iMapOffsetX,
				iMapOffsetY, (u16*)bgCollision, 424, 64);

			s32 bDirection = -1;
			if (Left > iTileTest)
			{
				bDirection = 1;

			}
			if (Right > iTileTest)
			{
				bDirection = -1;

			}
			if (Bottom > iTileTest)
			{
				a_EnemyArray[i].iy &= ~0x7ff;
				a_EnemyArray[i].bOnGround = true;
			
			}
			else
			{
				a_EnemyArray[i].bOnGround = false;
				a_EnemyArray[i].iVelocityY = fixAdd(a_EnemyArray[i].iVelocityY, 32);
				a_EnemyArray[i].iy = fixAdd(a_EnemyArray[i].iy, a_EnemyArray[i].iVelocityY);
			}

			a_EnemyArray[i].ix += bDirection;

			a_SpriteManager.MoveSprite(a_EnemyArray[i].ix, fix2int(a_EnemyArray[i].iy), a_EnemyArray[i].iSpriteID);


			if (a_EnemyArray[i].ix <= 1)
			{
				DeleteEnemy(a_EnemyArray[i]);
				a_SpriteManager.DeleteSprite(a_EnemyArray[i].iSpriteID);
			}


		}


	}

	


}

void AIManager::DeleteEnemy(AIManager& a_Enemy)
{
	a_Enemy.bActive = false;
}