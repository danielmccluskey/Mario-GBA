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
	a_ix = fix2int(a_ix);//Convert X to screen
	if (a_bRightSide)//If it is the right side of the block
	{
		a_ix -= 2;//Move away from right edge to avoid overlap
	}
	else//If on left side of block
	{
		a_ix += 4;//Move away from edge to avoid overlap
	}
	a_iy = fix2int(a_iy);//Convert to screen pos



	/*The following part seems useless but has to be done. For some reason, the compiler would completely ignore the lines if I did not do an operation in between.	
	e.g.
	a_ix = 18;
	a_ix >>=4;
	a_ix <<=4;

	a_ix would equal 18 instead of 16
	*/

	a_ix >>= 4;//Divide by 16
	a_iy >>= 4;//Divide by 16
	a_ix = ((int)(a_ix));//Cast as int to get rid of extra data
	a_iy = ((int)(a_iy));//Cast as int to get rid of extra data
	a_ix <<= 4;//Multiply by 16
	a_iy <<= 4;//Multiply by 16

	a_ix += 16;//Add 16 for alignment
	
	
	a_ix -= a_iScrollOffset+2;//Minus the Scroll offset to align spawned block with world.

	//a_iy = a_iy * 16;//Multiply by 16 to get Tiles real Y Pos.
	
	

	for (int i = 0; i < MAX_PRIZEBLOCKS; i++)
	{
		if (a_PrizeBlockArray[i].bActive == false)
		{
			a_PrizeBlockArray[i].iSpriteID = a_oSpriteManager.CreateSprite((u16*)PrizeBlockEmptyTiles, (u16*)PrizeBlockEmptyPal, PrizeBlockEmptyTilesLen, PrizeBlockEmptyPalLen * 2, PrizeBlockTileBlock, PrizeBlockPalb);//Create sprite and give it its assigned sprite id
			a_PrizeBlockArray[i].ix = a_ix;//Set pos
			a_PrizeBlockArray[i].bNeedsSpawn = true;//Sets flag to spawn a prizeblock
			a_PrizeBlockArray[i].iy = a_iy;//Set pos
			a_PrizeBlockArray[i].bActive = true;//Set active flag
			a_oSpriteManager.MoveSprite(a_PrizeBlockArray[i].ix, a_PrizeBlockArray[i].iy, a_PrizeBlockArray[i].iSpriteID);//Move the sprite
			return a_PrizeBlockArray[i].iSpriteID;
		}

	}
	return -1;
}

void PrizeBlockManager::SpawnPowerUp(PrizeBlockManager* a_PrizeBlockArray, SpriteManager& a_oSpriteManager, AIManager* a_AIManager)
{

	for (int i = 0; i < MAX_PRIZEBLOCKS; i++)//Loop through prizeblock array
	{
		if (a_PrizeBlockArray[i].bActive == true && a_PrizeBlockArray[i].bNeedsSpawn == true)//Find first active block that needs a block spawned
		{
			a_PrizeBlockArray[i].bNeedsSpawn = false;//Reset spawn flag
			s32 iRandomNumber = qran_range(3, 5);//Select random powerup magic number

			a_AIManager[0].CreateEnemy(a_oSpriteManager, a_AIManager, iRandomNumber, a_PrizeBlockArray[i].ix, int2fix(a_PrizeBlockArray[i].iy-16));//Create a powerup
		}
	}
	
	
}

void PrizeBlockManager::MoveBlocks(SpriteManager& a_oSpriteManager, PrizeBlockManager* a_PrizeBlockArray, s32 a_ix)
{
	for (int i = 0; i < MAX_PRIZEBLOCKS; i++)//Loop through array
	{
		if (a_PrizeBlockArray[i].bActive == true)//Find any active blocks
		{
			a_PrizeBlockArray[i].ix -= a_ix;//Move the block left as map scrolls
			a_oSpriteManager.MoveSprite(a_PrizeBlockArray[i].ix, a_PrizeBlockArray[i].iy, a_PrizeBlockArray[i].iSpriteID);//Set the position
			if (a_PrizeBlockArray[i].ix <= (-iSpriteWidth))//If sprite is offscreen
			{
				a_PrizeBlockArray[i].bActive = false;//Deactivate sprite
				a_oSpriteManager.DeleteSprite(a_PrizeBlockArray[i].iSpriteID);//Delete the sprite
			}
		}
	}

}