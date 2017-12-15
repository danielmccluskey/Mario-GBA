#ifndef _MARIOMANAGER_H_
#define _MARIOMANAGER_H_


#include "gba_math.h"
#include "DM_SpriteManager.h"
#include "DM_ParticleManager.h"
#include "DM_PrizeBlockManager.h"
#include "DM_AIManager.h"


struct Fireball
{
	fixed fx, fy;
	fixed fvx, fvy;
	u16 iSpriteID;
	u16 iFrame = 0;
	u16 iFrameSize = 1;
	u16 iLifeTime = 255;
	bool bActive = false;

};

class MarioManager
{
public:
	s32 ix; s32 iy;

	s32 iSpriteID;

	s32 iFrame = 0;
	s32 iFrameSize = 4;
	s32 iAnimationDelay = 20;



	u8 iSpriteHeight = 16;
	u8 iSpriteWidth = 16;

	s32 iMapOffsetX = 0;
	s32 iMapOffsetY = 0;

	bool bMapMode = false;
	bool bJump = false;
	bool bOnGround = false;
	bool bJustLanded = false;
	bool bMoving = false;
	bool bHitPrizeBlock = false;
	bool bInvulnerable = false;
	bool bDead = false;
	bool bFinished = false;
	u16 iInvulnerableTime = 0;

	fixed iVelocityX;
	fixed iVelocityY;


	bool bRight = false;
	bool bTop = false;
	bool bBottom = false;
	bool bLeft = false;


	u8 TopLeft;
	u8 TopMiddle;
	u8 BottomLeft;
	u8 TopRight;
	u8 BottomRight;
	u8 AlmostBotRight;
	u8 AlmostBotLeft;

	s8 iCurrentType = 0;

	ParticleManager Particlemanager;
	ParticleManager EnemyParticlemanager;
	Fireball sfire[MAX_FIREBALLS];

	unsigned short* iMarioBGCollision;
	

	void CreateMario(SpriteManager& a_SpriteManager);
	void MoveMario(s32 a_ix, s32 a_iy, SpriteManager& a_SpriteManager);
	void ResetMario();
	void SetPos(s32 a_ix, s32 a_iy, SpriteManager& a_SpriteManager);
	void UpdateMario(SpriteManager& a_SpriteManager, PrizeBlockManager* a_PrizeBlockManagerArray, u16 a_iScrollOffset);
	void AnimateMario(SpriteManager& a_SpriteManager);
	void PhysicsHandler();
	void FlashMario(SpriteManager& a_SpriteManager);
	u16 MapManager(const unsigned short* a_bgCollisionMap, SpriteManager a_SpriteManager);
	s32 GrabIndex(u32 x, u32 y, u32 xscroll, u32 yscroll, u16* tilemap, u32 tilemap_w, u32 tilemap_h);
	void CheckCollisions();
	void TransformMario(s32 a_iMarioType, SpriteManager& a_SpriteManager, bool a_bHurtMario);
	void UpdateFireBall(SpriteManager& a_SpriteManager);
	void ShootFireBall(SpriteManager& a_SpriteManager);
	void InitFireBall(SpriteManager& a_SpriteManager);
	void CheckFireballCollisions(SpriteManager& a_SpriteManager, AIManager* a_EnemyArray);

	u16 tile_lookup(u32 x, u32 y, u32 xscroll, u32 yscroll,u16* tilemap, u32 tilemap_w, u32 tilemap_h);

};


#endif // !_MARIOMANAGER_H_
