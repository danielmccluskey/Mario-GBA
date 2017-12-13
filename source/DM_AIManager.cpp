#include "DM_AIManager.h"
#include "EnemySprites.h"
#include "World1Level1_Externs.h"
#include "Powerups.h"

enum EnemyTypes
{
	GOOMBA,
	GREENTURTLE,
	REDTURTLE,
	MUSHROOM,
	FLOWER,
	STAR
};
enum ENEMYPHYSICS
{
	WALKSPEED = 32,
	JUMPHEIGHT = -1024,
	GRAVITY = 32,
	PUSHBACK = 8,
	ALIGNMASK = ~0x7ff,
	STOPPED = 0,
	COLLISIONTILE = 0,
	MAXXVELOCITY = 550,
	MAXYVELOCITY = 550,
	QUESTIONRANGEA = 2,
	QUESTIONRANGEB = 5
};

void AIManager::CreateEnemy(SpriteManager& a_SpriteManager, AIManager* a_EnemyArray, s8 a_iEnemyType, s32 a_ix, s32 a_iy)
{
	for (int i = 0; i < MAX_ENEMIES; i++)
	{
		if (a_EnemyArray[i].bActive == false)
		{
			switch (a_iEnemyType)
			{
			case GOOMBA:
				a_EnemyArray[i].iStartingFrame = EnemyTileBlock;
				a_EnemyArray[i].iSpriteID = a_SpriteManager.CreateSprite((u16*)EnemySpritesTiles, (u16*)EnemySpritesPal, EnemySpritesTilesLen, EnemySpritesPalLen, EnemyTileBlock, EnemyPalb);
				break;
			case GREENTURTLE:
				a_EnemyArray[i].iStartingFrame = EnemyTileBlock;
				a_EnemyArray[i].iSpriteID = a_SpriteManager.CreateSprite((u16*)EnemySpritesTiles, (u16*)EnemySpritesPal, EnemySpritesTilesLen, EnemySpritesPalLen, EnemyTileBlock, EnemyPalb);
				break;
			case REDTURTLE:
				a_EnemyArray[i].iStartingFrame = EnemyTileBlock;
				a_EnemyArray[i].iSpriteID = a_SpriteManager.CreateSprite((u16*)EnemySpritesTiles, (u16*)EnemySpritesPal, EnemySpritesTilesLen, EnemySpritesPalLen, EnemyTileBlock, EnemyPalb);
				break;
			case MUSHROOM:
				a_EnemyArray[i].iStartingFrame = PowerupsTileBlock;
				a_EnemyArray[i].iSpriteID = a_SpriteManager.CreateSprite((u16*)PowerupsTiles, (u16*)PowerupsPal, PowerupsTilesLen, PowerupsPalLen, PowerupsTileBlock, PowerupsPalb);
				break;

			}
			
			a_EnemyArray[i].bDirection = -1;
			a_EnemyArray[i].iVelocityY = 0;
			a_EnemyArray[i].iMaxVelocityY = 550;
			a_EnemyArray[i].ix = a_ix;
			a_EnemyArray[i].iy = a_iy;
			a_EnemyArray[i].bActive = true;
			a_SpriteManager.MoveSprite(a_EnemyArray[i].ix, a_EnemyArray[i].iy, a_EnemyArray[i].iSpriteID);
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

u16 AIManager::tile_lookup(u32 x, u32 y, u32 xscroll, u32 yscroll, u16* tilemap, u32 tilemap_w, u32 tilemap_h)
{
	x += xscroll;
	y += yscroll * 8;
	x >>= 3;
	y >>= 3;
	s32 index = y * tilemap_w + x;
	return tilemap[index];
}

void AIManager::UpdateEnemies(SpriteManager& a_SpriteManager, AIManager* a_EnemyArray)
{
	s32 iTileTest = 0;
	iFrame++;
	if (iFrame == 2)
	{
		iFrame = 0;
	}
	
	for (int i = 0; i < MAX_ENEMIES; i++)
	{
		if (a_EnemyArray[i].bActive == true)
		{

			s32 iTileX = a_EnemyArray[i].ix;//     Took me 4 hours to figure out it wasnt a fixed variable kill me>>fix2int(a_EnemyArray[i].ix);
			s32 iTileY = fix2int(a_EnemyArray[i].iy);


			u8 Bottom = tile_lookup(iTileX, iTileY + iSpriteHeight, iMapOffsetX,
				iMapOffsetY, (u16*)World1Level1Collision, 424, 32);
			u8 Left = tile_lookup(iTileX-4, iTileY + 8, iMapOffsetX,
				iMapOffsetY, (u16*)World1Level1Collision, 424, 32);
			u8 Right = tile_lookup(iTileX+ iSpriteWidth+4, iTileY, iMapOffsetX,
				iMapOffsetY, (u16*)World1Level1Collision, 424, 32);

			u8 BottomRight = tile_lookup(iTileX + iSpriteWidth, iTileY + iSpriteHeight, iMapOffsetX,
				iMapOffsetY, (u16*)World1Level1Collision, 424, 32);

			if (Left > COLLISIONTILE)
			{
				a_EnemyArray[i].bDirection = 1;

			}
			if (Right > COLLISIONTILE)
			{
				a_EnemyArray[i].bDirection = -1;

			}
			if (Bottom > COLLISIONTILE || BottomRight > COLLISIONTILE)
			{
				a_EnemyArray[i].iy &= ALIGNMASK;
				a_EnemyArray[i].bOnGround = true;
			}
			else
			{
				a_EnemyArray[i].bOnGround = false;
				a_EnemyArray[i].iVelocityY = fixAdd(a_EnemyArray[i].iVelocityY, GRAVITY);
				a_EnemyArray[i].iy = fixAdd(a_EnemyArray[i].iy, a_EnemyArray[i].iVelocityY);
			}

			a_EnemyArray[i].ix += a_EnemyArray[i].bDirection;

			a_SpriteManager.MoveSprite(a_EnemyArray[i].ix, fix2int(a_EnemyArray[i].iy), a_EnemyArray[i].iSpriteID);


			if (a_EnemyArray[i].ix <= 1)
			{
				DeleteEnemy(a_EnemyArray[i]);
				a_SpriteManager.DeleteSprite(a_EnemyArray[i].iSpriteID);
			}

			if (a_EnemyArray[i].bAnimate)
			{
				a_SpriteManager.SetFrame(a_EnemyArray[i].iStartingFrame + (iFrame*iFrameSize), a_EnemyArray[i].iSpriteID);

			}
			
			
			

		}


	}

	
	


}

void AIManager::ScrollEnemies(SpriteManager& a_SpriteManager, AIManager* a_EnemyArray, s32 a_ix)
{
	for (int i = 0; i < MAX_ENEMIES; i++)
	{
		if (a_EnemyArray[i].bActive == true)
		{
			a_EnemyArray[i].ix -= a_ix;
			a_SpriteManager.MoveSprite(a_EnemyArray[i].ix, a_EnemyArray[i].iy, a_EnemyArray[i].iSpriteID);
			if (a_EnemyArray[i].ix <= (-iSpriteWidth))
			{
				a_EnemyArray[i].bActive = false;
				a_SpriteManager.DeleteSprite(a_EnemyArray[i].iSpriteID);
			}
		}
	}

}

void AIManager::DeleteEnemy(AIManager& a_Enemy)
{
	a_Enemy.bActive = false;
}