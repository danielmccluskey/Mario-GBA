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
	bool bMoving = false;

	fixed iVelocityX;
	fixed iVelocityY;


	bool bRight = false;
	bool bTop = false;
	bool bBottom = false;
	bool bLeft = false;


	u8 TopLeft;
	u8 BottomLeft;
	u8 TopRight;
	u8 BottomRight;
	u8 AlmostBotRight;
	u8 AlmostBotLeft;

	ParticleManager particleee;
	Fireball sfire[10];

	void CreateMario(SpriteManager& a_SpriteManager);
	void MoveMario(s32 a_ix, s32 a_iy, SpriteManager& a_SpriteManager);
	void SetPos(s32 a_ix, s32 a_iy, SpriteManager& a_SpriteManager);
	void UpdateMario(SpriteManager& a_SpriteManager);
	u16 MapManager(const unsigned short* a_bgCollisionMap, SpriteManager a_SpriteManager);
	void CheckCollisions();
	void TransformMario(s32 a_iMarioType, SpriteManager& a_SpriteManager);
	void UpdateFireBall(SpriteManager& a_SpriteManager);
	void ShootFireBall(SpriteManager& a_SpriteManager);
	void InitFireBall(SpriteManager& a_SpriteManager);
	u16 tile_lookup(u32 x, u32 y, u32 xscroll, u32 yscroll,u16* tilemap, u32 tilemap_w, u32 tilemap_h);

};


#endif // !_MARIOMANAGER_H_
