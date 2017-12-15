#include "DM_PrizeBlockManager.h"
#include "PrizeBlockEmpty.h"

#include "DM_Enums.h"
#include "gba_mathUtil.h"
enum POWERUPTYPES
{
	POWERUP_TYPES_MUSHROOM,
	POWERUP_TYPES_FLOWER,
	POWERUP_TYPES_STAR,
	POWERUP_TYPES_COIN
};
s16 PrizeBlockManager::CreateBlock(s32 a_ix, s32 a_iy, PrizeBlockManager* a_PrizeBlockArray, SpriteManager& a_oSpriteManager, u16 a_iScrollOffset, bool a_bRightSide)
{
	a_ix = fix2int(a_ix);
	if (a_bRightSide)
	{
		a_ix -= 2;
	}
	else
	{
		a_ix += 4;
	}
	a_iy = fix2int(a_iy);
	a_ix >>= 4;
	a_iy >>= 4;
	a_ix = ((int)(a_ix));
	a_iy = ((int)(a_iy));
	a_ix <<= 4;
	a_iy <<= 4;

	a_ix += 16;
	
	
	a_ix -= a_iScrollOffset+2;//Minus the Scroll offset to align spawned block with world.

	//a_iy = a_iy * 16;//Multiply by 16 to get Tiles real Y Pos.
	
	

	for (int i = 0; i < MAX_PRIZEBLOCKS; i++)
	{
		if (a_PrizeBlockArray[i].bActive == false)
		{
			a_PrizeBlockArray[i].iSpriteID = a_oSpriteManager.CreateSprite((u16*)PrizeBlockEmptyTiles, (u16*)PrizeBlockEmptyPal, PrizeBlockEmptyTilesLen, PrizeBlockEmptyPalLen * 2, PrizeBlockTileBlock, PrizeBlockPalb);
			a_PrizeBlockArray[i].ix = a_ix;
			a_PrizeBlockArray[i].bNeedsSpawn = true;
			a_PrizeBlockArray[i].iy = a_iy;
			a_PrizeBlockArray[i].bActive = true;
			a_oSpriteManager.MoveSprite(a_PrizeBlockArray[i].ix, a_PrizeBlockArray[i].iy, a_PrizeBlockArray[i].iSpriteID);
			//a_oSpriteManager.HideSprite(a_PrizeBlockArray[i].iSpriteID);

			return a_PrizeBlockArray[i].iSpriteID;
		}

	}
	return -1;
}

void PrizeBlockManager::SpawnPowerUp(PrizeBlockManager* a_PrizeBlockArray, SpriteManager& a_oSpriteManager, AIManager* a_AIManager)
{

	for (int i = 0; i < MAX_PRIZEBLOCKS; i++)
	{
		if (a_PrizeBlockArray[i].bActive == true && a_PrizeBlockArray[i].bNeedsSpawn == true)
		{
			a_PrizeBlockArray[i].bNeedsSpawn = false;
			s32 iRandomNumber = qran_range(3, 5);

			a_AIManager[0].CreateEnemy(a_oSpriteManager, a_AIManager, iRandomNumber, a_PrizeBlockArray[i].ix, int2fix(a_PrizeBlockArray[i].iy-16));
		}
	}
	
	
}

void PrizeBlockManager::MoveBlocks(SpriteManager& a_oSpriteManager, PrizeBlockManager* a_PrizeBlockArray, s32 a_ix)
{
	for (int i = 0; i < MAX_PRIZEBLOCKS; i++)
	{
		if (a_PrizeBlockArray[i].bActive == true)
		{
			a_PrizeBlockArray[i].ix -= a_ix;
			a_oSpriteManager.MoveSprite(a_PrizeBlockArray[i].ix, a_PrizeBlockArray[i].iy, a_PrizeBlockArray[i].iSpriteID);
			if (a_PrizeBlockArray[i].ix <= (-iSpriteWidth))
			{
				a_PrizeBlockArray[i].bActive = false;
				a_oSpriteManager.DeleteSprite(a_PrizeBlockArray[i].iSpriteID);
			}
		}
	}

}