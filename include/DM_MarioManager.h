#ifndef _MARIOMANAGER_H_
#define _MARIOMANAGER_H_


#include "gba_math.h"
#include "DM_SpriteManager.h"
#include "DM_ParticleManager.h"


struct Fireball
{
	fixed fx, fy;
	fixed fvx, fvy;
	u16 iSpriteID;
	u16 iLifeTime = 255;

};

class MarioManager
{
public:
	s32 ix; s32 iy;
	s32 last_ix; s32 last_iy;

	s32 iSpriteID;

	s32 iFrame = 0;
	s32 iFrameSize = 4;
	s32 iAnimationDelay = 20;

	s32 iSpriteHeight = 16;
	s32 iSpriteWidth = 16;

	s32 iMapOffset = 0;

	bool bJump = false;
	bool bOnGround = false;

	fixed iVelocityX;
	fixed iVelocityY;

	fixed iMaxVelocityX;
	fixed iMaxVelocityY;

	ParticleManager particleee;
	Fireball sfire;

	void CreateMario(SpriteManager& a_SpriteManager);
	void MoveMario(s32 a_ix, s32 a_iy, SpriteManager& a_SpriteManager);
	void UpdateMario(SpriteManager& a_SpriteManager);
	void TransformMario(s32 a_iMarioType, SpriteManager& a_SpriteManager);
	void ShootFireBall(SpriteManager& a_SpriteManager);
	void InitFireBall(SpriteManager& a_SpriteManager);

};


#endif // !_MARIOMANAGER_H_
