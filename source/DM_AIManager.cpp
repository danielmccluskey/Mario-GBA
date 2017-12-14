#include "DM_AIManager.h"
#include "EnemySprites.h"
#include "Powerups.h"
#include "DM_Enums.h"

enum EnemyTypes
{
	GOOMBA,
	GREENTURTLE,
	REDTURTLE,
	MUSHROOM,
	FLOWER,
	STAR,
	GREENSHELL,
	REDSHELL
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
				a_EnemyArray[i].bDirection = -1;
				a_EnemyArray[i].iSpriteID = a_SpriteManager.CreateSprite((u16*)EnemySpritesTiles, (u16*)EnemySpritesPal, EnemySpritesTilesLen, EnemySpritesPalLen, EnemyTileBlock, EnemyPalb);
				break;
			case GREENTURTLE:
				a_EnemyArray[i].iStartingFrame = EnemyTileBlock + 8;
				a_EnemyArray[i].bDirection = -1;
				a_EnemyArray[i].iSpriteID = a_SpriteManager.CreateSprite((u16*)EnemySpritesTiles, (u16*)EnemySpritesPal, EnemySpritesTilesLen, EnemySpritesPalLen, EnemyTileBlock, EnemyPalb);
				break;
			case REDTURTLE:
				a_EnemyArray[i].iStartingFrame = EnemyTileBlock + 8;
				a_EnemyArray[i].bDirection = -1;
				a_EnemyArray[i].iSpriteID = a_SpriteManager.CreateSprite((u16*)EnemySpritesTiles, (u16*)EnemySpritesPal, EnemySpritesTilesLen, EnemySpritesPalLen, EnemyTileBlock, EnemyPalb);
				break;
			case MUSHROOM:
				a_EnemyArray[i].iStartingFrame = PowerupsTileBlock;
				a_EnemyArray[i].bDirection = 1;
				a_EnemyArray[i].iSpriteID = a_SpriteManager.CreateSprite((u16*)PowerupsTiles, (u16*)PowerupsPal, PowerupsTilesLen, PowerupsPalLen, PowerupsTileBlock, PowerupsPalb);
				break;
			case FLOWER:
				a_EnemyArray[i].iStartingFrame = PowerupsTileBlock + 4;
				a_EnemyArray[i].bDirection = 1;
				a_EnemyArray[i].iSpriteID = a_SpriteManager.CreateSprite((u16*)PowerupsTiles, (u16*)PowerupsPal, PowerupsTilesLen, PowerupsPalLen, PowerupsTileBlock, PowerupsPalb);
				break;

			}

			a_EnemyArray[i].iSpriteType = a_iEnemyType;

			a_SpriteManager.SetFrame(a_EnemyArray[i].iStartingFrame, a_EnemyArray[i].iSpriteID);
			
			
			a_EnemyArray[i].iVelocityY = 0;
			a_EnemyArray[i].iMaxVelocityY = 550;
			a_EnemyArray[i].ix = a_ix;
			a_EnemyArray[i].iy = a_iy;
			a_EnemyArray[i].bActive = true;
			a_EnemyArray[i].bDead = false;
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

u16 AIManager::CheckSpriteCollision(SpriteManager& a_SpriteManager, AIManager* a_AIManager, s32 a_ix, s32 a_iy, s32 a_iSpriteWidth, s32 a_iSpriteHeight, bool a_bMarioInvulnerable)
{
	
	int x1Min = fix2int(a_ix);
	int x1Max = fix2int(a_ix) + a_iSpriteWidth;
	int y1Max = fix2int(a_iy) + a_iSpriteHeight;
	int y1Min = fix2int(a_iy);

	
	for (int i = 0; i < MAX_ENEMIES; i++)
	{
		if (a_AIManager[i].bActive == true && a_AIManager[i].bDead == false)
		{
			if (a_bMarioInvulnerable && !(a_AIManager[i].iSpriteType >= MUSHROOM))
			{
				continue;
			}
			int x2Min = a_AIManager[i].ix;
			int x2Max = a_AIManager[i].ix + iSpriteWidth;
			int y2Max = fix2int(a_AIManager[i].iy) + iSpriteHeight;
			int y2Min = fix2int(a_AIManager[i].iy);

			

			if (x1Max < x2Min || x1Min > x2Max)
			{
				continue;
			}
			else if (y1Max < y2Min || y1Min > y2Max)
			{
				continue;
			}
			else if (y2Min + 5 > y1Max)
			{
				a_AIManager[i].bDead = true;
				continue;
			}
			else
			{
				if (a_AIManager[i].iSpriteType >= MUSHROOM)
				{
					a_AIManager[i].bActive = false;
					a_SpriteManager.DeleteSprite(a_AIManager[i].iSpriteID);
				}
				
				return a_AIManager[i].iSpriteType;
			}
		}		
	}
	return 15;
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
				iMapOffsetY, (u16*)iEnemyBGCollision, 424, 32);
			u8 Left = tile_lookup(iTileX - 4, iTileY + 8, iMapOffsetX,
				iMapOffsetY, (u16*)iEnemyBGCollision, 424, 32);
			u8 Right = tile_lookup(iTileX + iSpriteWidth + 4, iTileY, iMapOffsetX,
				iMapOffsetY, (u16*)iEnemyBGCollision, 424, 32);

			u8 BottomRight = tile_lookup(iTileX + iSpriteWidth, iTileY + iSpriteHeight, iMapOffsetX,
				iMapOffsetY, (u16*)iEnemyBGCollision, 424, 32);


			if (a_EnemyArray[i].iSpriteType != FLOWER )
			{
				if (Left > COLLISIONTILE)
				{
					a_EnemyArray[i].bDirection = 1;

				}
				if (Right > COLLISIONTILE)
				{
					a_EnemyArray[i].bDirection = -1;

				}
				a_EnemyArray[i].ix += a_EnemyArray[i].bDirection;
			}

			if ((Bottom > COLLISIONTILE || BottomRight > COLLISIONTILE ) && !(a_EnemyArray[i].bDead))
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



			a_SpriteManager.MoveSprite(a_EnemyArray[i].ix, fix2int(a_EnemyArray[i].iy), a_EnemyArray[i].iSpriteID);




			if (a_EnemyArray[i].ix <= 1 || fix2int(a_EnemyArray[i].iy) >= 160)
			{
				a_EnemyArray[i].bActive = false;
				a_SpriteManager.DeleteSprite(a_EnemyArray[i].iSpriteID);
				a_EnemyArray[i].bDead = false;
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

void AIManager::DeleteEnemies(SpriteManager a_SpriteManager, AIManager* a_EnemyArray)
{
	for (int i = 0; i < MAX_ENEMIES; i++)
	{
		if (a_EnemyArray[i].bActive == true)
		{
			a_EnemyArray[i].bActive = false;
			a_SpriteManager.DeleteSprite(a_EnemyArray[i].iSpriteID);
			a_EnemyArray[i].bDead = false;
		}
	}
}