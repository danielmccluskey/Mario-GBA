#ifndef _AIMANAGER_H_
#define _AIMANAGER_H_
#include "gba_math.h"
#include "DM_SpriteManager.h"
#include "DM_ParticleManager.h"
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
	unsigned short* iEnemyBGCollision;

	bool bOnGround = false;
	bool bActive = false;
	bool bAnimate = false;
	bool bDead = false;
	bool bSquish = false;
	bool bDeleteParticles = false;

	s32 iMapOffsetX = 0;
	s32 iMapOffsetY = 0;

	s8 iSpriteType = 0;

	s8 bDirection = -1;
	fixed iVelocityY;

	fixed iMaxVelocityY;

	



	void CreateEnemy(SpriteManager& a_oSpriteManager, AIManager* a_aoEnemyArray, s8 a_iEnemyType, s32 a_ix, s32 a_iy);
	void MoveEnemy();
	void UpdateEnemies(SpriteManager& a_oSpriteManager, AIManager* a_aoEnemyArray);
	void UpdateOffset(AIManager* a_aoEnemyArray, s32 a_iOffsetX, s32 a_iOffsetY);
	void TransformEnemy(s32 a_iMarioType, SpriteManager& a_oSpriteManager);
	u16 tile_lookup(u32 x, u32 y, u32 xscroll, u32 yscroll,	u16* tilemap, u32 tilemap_w, u32 tilemap_h);
	u16 CheckSpriteCollision(SpriteManager& a_oSpriteManager, AIManager* a_AIManager, s32 a_ix, s32 a_iy, s32 a_iSpriteWidth, s32 a_iSpriteHeight, bool a_bMarioInvulnerable);
	void DeleteEnemies(SpriteManager& a_oSpriteManager, AIManager* a_aoEnemyArray);
	void ScrollEnemies(SpriteManager& a_oSpriteManager, AIManager* a_aoEnemyArray, s32 a_ix);

};
#endif // !_AIMANAGER_H_