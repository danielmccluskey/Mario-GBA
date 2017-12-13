#include "DM_PrizeBlockManager.h"
#include "PrizeBlockEmpty.h"

enum POWERUPTYPES
{
	MUSHROOM,
	FLOWER,
	STAR,
	COIN
};
s16 PrizeBlockManager::CreateBlock(s32 a_ix, s32 a_iy, PrizeBlockManager* a_PrizeBlockArray, SpriteManager& a_SpriteManager, u16 a_iScrollOffset, bool a_bRightSide)
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
			a_PrizeBlockArray[i].iSpriteID = a_SpriteManager.CreateSprite((u16*)PrizeBlockEmptyTiles, (u16*)PrizeBlockEmptyPal, PrizeBlockEmptyTilesLen, PrizeBlockEmptyPalLen * 2, PrizeBlockTileBlock, PrizeBlockPalb);
			a_PrizeBlockArray[i].ix = a_ix;
			
			a_PrizeBlockArray[i].iy = a_iy;
			a_PrizeBlockArray[i].bActive = true;
			a_SpriteManager.MoveSprite(a_PrizeBlockArray[i].ix, a_PrizeBlockArray[i].iy, a_PrizeBlockArray[i].iSpriteID);
			//a_SpriteManager.HideSprite(a_PrizeBlockArray[i].iSpriteID);

			return a_PrizeBlockArray[i].iSpriteID;
		}

	}
}

void PrizeBlockManager::PlaceEmptyBlock(s32 a_ix, s32 a_iy, SpriteManager& a_SpriteManager)
{

}
void PrizeBlockManager::ChoosePowerUp(s32 a_iTileX, s32 a_iTileY, s32 a_iMapWidth, const unsigned short* a_bgCollisions)
{

}
void PrizeBlockManager::HitBlock(s32 a_ix, s32 a_iy, PrizeBlockManager* a_PrizeBlockArray, SpriteManager& a_SpriteManager, AIManager& a_AIManager, s32 a_iMapWidth, const unsigned short* a_bgCollisions)
{


	//s16 CurrentSpriteID = CreateBlock(a_ix, a_iy, a_PrizeBlockArray, a_SpriteManager, 0);
	
}

void PrizeBlockManager::MoveBlocks(SpriteManager& a_SpriteManager, PrizeBlockManager* a_PrizeBlockArray, s32 a_ix)
{
	for (int i = 0; i < MAX_PRIZEBLOCKS; i++)
	{
		if (a_PrizeBlockArray[i].bActive == true)
		{
			a_PrizeBlockArray[i].ix -= a_ix;
			a_SpriteManager.MoveSprite(a_PrizeBlockArray[i].ix, a_PrizeBlockArray[i].iy, a_PrizeBlockArray[i].iSpriteID);
			if (a_PrizeBlockArray[i].ix <= (-iSpriteWidth))
			{
				a_PrizeBlockArray[i].bActive = false;
				a_SpriteManager.DeleteSprite(a_PrizeBlockArray[i].iSpriteID);
			}
		}
	}

}