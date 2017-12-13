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
	a_iy = fix2int(a_iy);


	/*
	This little bit below had me stuck for like 4 goddamn hours.
	For some reason, when I divided by 16 then multiplied by 16 straight after, the compiler would just completely skip those lines.
	e.g.
	
	a_ix = 68;

	a_ix >>= 4; //SKIPPED
	a_ix *= 16; //SKIPPED

	a_ix would still equal 68 and not 64.
	The only fix I could find was casting the variable as an int after dividing and then continuing as normal.
	*/
	a_ix >>= 4;//Divide by 16 to get tile X pos.
	if (a_bRightSide)
	{
		a_ix -= 1;//Minus the Scroll offset to align spawned block with world.
	}
	a_iy >>= 4;//Divide by 16 to get tile Y pos.
	a_ix = ((int)(a_ix));//Cast to int to get rid of extra data.
	a_iy = ((int)(a_iy));//Cast to int to get rid of extra data.
	a_ix = a_ix * 16;//Multiply by 16 to get Tiles real X Pos.
	
	
	a_ix -= a_iScrollOffset - 14;//Minus the Scroll offset to align spawned block with world.

	a_iy = a_iy * 16;//Multiply by 16 to get Tiles real Y Pos.
	
	

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