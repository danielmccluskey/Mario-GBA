#ifndef _AIMANAGER_H_
#define _AIMANAGER_H_
#include "gba_math.h"
#include "DM_SpriteManager.h"
class AIManager
{
public:
	s32 ix; s32 iy;

	s32 iSpriteID = -1;

	s32 iStartingFrame = 0;
	s32 iFrame = 0;
	s32 iFrameSize = 4;
	s32 iAnimationDelay = 20;

	s32 iSpriteHeight = 16;
	s32 iSpriteWidth = 16;


	bool bOnGround = false;
	bool bActive = false;
	bool bAnimate = false;

	s32 iMapOffsetX = 0;
	s32 iMapOffsetY = 0;

	s8 bDirection = -1;
	fixed iVelocityY;

	fixed iMaxVelocityY;

	void CreateEnemy(SpriteManager& a_SpriteManager, AIManager* a_EnemyArray, s8 a_iEnemyType, s32 a_ix, s32 a_iy);
	void MoveEnemy();
	void UpdateEnemies(SpriteManager& a_SpriteManager, AIManager* a_EnemyArray);
	void UpdateOffset(AIManager* a_EnemyArray, s32 a_iOffsetX, s32 a_iOffsetY);
	void TransformEnemy(s32 a_iMarioType, SpriteManager& a_SpriteManager);
	u16 tile_lookup(u32 x, u32 y, u32 xscroll, u32 yscroll,	u16* tilemap, u32 tilemap_w, u32 tilemap_h);
	void DeleteEnemy(AIManager& a_Enemy);
	void ScrollEnemies(SpriteManager& a_SpriteManager, AIManager* a_EnemyArray, s32 a_ix);

};
#endif // !_AIMANAGER_H_