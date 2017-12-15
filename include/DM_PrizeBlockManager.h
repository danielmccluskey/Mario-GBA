#ifndef _PRIZEBLOCKMANAGER_H_
#define _PRIZEBLOCKMANAGER_H_


#include "gba_types.h"
#include "gba_math.h"
#include "DM_SpriteManager.h"
#include "DM_AIManager.h"



class PrizeBlockManager
{
public:
	s32 ix, iy;
	u16 iSpriteID;
	bool bActive = false;
	bool bNeedsSpawn = false;
	u16 iSpriteWidth = 16;

	~PrizeBlockManager() {};

	s16 CreateBlock(s32 a_ix, s32 a_iy, PrizeBlockManager* a_PrizeBlockArray, SpriteManager& a_oSpriteManager, u16 a_iScrollOffset, bool a_bRightSide);
	void PlaceEmptyBlock(s32 a_ix, s32 a_iy, SpriteManager& a_oSpriteManager);
	void ChoosePowerUp(s32 a_iTileX, s32 a_iTileY, s32 a_iMapWidth, const unsigned short* a_bgCollisions);
	void SpawnPowerUp(PrizeBlockManager* a_PrizeBlockArray, SpriteManager& a_oSpriteManager, AIManager* a_AIManager);
	void MoveBlocks(SpriteManager& a_oSpriteManager, PrizeBlockManager* a_PrizeBlockArray, s32 a_ix);
};


#endif // !_PRIZEBLOCKMANAGER_H_
