#ifndef _MARIOMANAGER_H_
#define _MARIOMANAGER_H_


#include "gba_math.h"
#include "DM_SpriteManager.h"

class MarioManager
{
public:
	s32 ix; s32 iy;
	s32 last_ix; s32 last_iy;

	s32 iSpriteID;

	s32 iMapOffset = 0;

	bool bJump = false;
	bool bOnGround = false;

	fixed iVelocityX;
	fixed iVelocityY;

	fixed iMaxVelocityX;
	fixed iMaxVelocityY;

	void CreateMario(SpriteManager& a_SpriteManager);
	void MoveMario(s32 a_ix, s32 a_iy, SpriteManager& a_SpriteManager);
	void UpdateMario(SpriteManager& a_SpriteManager);

};


#endif // !_MARIOMANAGER_H_
