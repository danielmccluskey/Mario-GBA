#include "DM_MarioManager.h"
#include "Mario.h"
#include "gba.h"
#include "BG_Collisions.h"
//#include "gba_math.h"

fixed	fixAdder(fixed a_fa, fixed a_fb)
{
	return a_fa + a_fb;
}
s32		fix2inter(fixed a_f)
{
	return a_f / FIX_SCALE;
}

//subtract two fixed point values
fixed	fixSubber(fixed a_fa, fixed a_fb)
{
	return a_fa - a_fb;
}

fixed	int2fixer(s32 a_i)
{
	return a_i << FIX_SHIFT;
}

void MarioManager::CreateMario(SpriteManager& a_SpriteManager)
{
	iSpriteID = a_SpriteManager.SpriteIndex;
	a_SpriteManager.CreateSprite((u16*)marTiles, (u16*)marPal, 256, 24, 4);
	a_SpriteManager.MoveSprite(0, 120, 0);

	iVelocityX = 0;
	iVelocityY = 0;

	iMaxVelocityX = 550;
	iMaxVelocityY = 500;

	ix = 0;
	iy = 120;
}

void MarioManager::MoveMario(s32 a_ix, s32 a_iy, SpriteManager& a_SpriteManager)
{
	if ((iVelocityX < iMaxVelocityX) && (iVelocityX > -iMaxVelocityX))
	{
		iVelocityX = fixAdder(a_ix, iVelocityX);
	}
	if (iVelocityY < iMaxVelocityY)
	{
		iVelocityY = fixAdder(iVelocityY, 10);
		iVelocityY = fixAdder(a_iy, iVelocityY);
	}
}

void MarioManager::UpdateMario(SpriteManager& a_SpriteManager)
{
	iVelocityY = fixAdder(iVelocityY, 30);

	u32 ax = (fix2inter(ix)) >> 3;
	u32 ay = (fix2inter(iy)) >> 3;
	ay = 44 + ay;

	iMapOffset = iMapOffset >> 3;

	//Collision Points to test
	u16 BottomLeft = bgCollision[(((ay + 2) * 424) + ax + iMapOffset)];
	u16 BottomRight = bgCollision[(((ay + 2) * 424) + ax + 2 + iMapOffset)];
	u16 TopLeft = bgCollision[(((ay) * 424) + ax + iMapOffset)];
	u16 TopRight = bgCollision[(((ay) * 424) + ax + 2 + iMapOffset)];

	
	if (iVelocityX > 0 && (TopRight > 0))
	{
		iVelocityX = -32;
	}

	if (iVelocityY < 0 && ((TopLeft > 0 || TopRight > 0)))
	{
		iVelocityY = 8;
	}

	if (BottomLeft > 0 || BottomRight > 0)
	{
		iVelocityY = 0;
		bOnGround = true;
		if (bJump)
		{
			iVelocityY = -1024;
		}
	}
	else
	{
		bOnGround = false;
	}
	bJump = false;

	//iVelocityY = fixAdder(ix, int2fixer(test));

	ix = fixAdder(ix, iVelocityX);
	iy = fixAdder(iy, iVelocityY);
	a_SpriteManager.MoveSprite(fix2inter(ix), fix2inter(iy), iSpriteID);
	if (iVelocityX >= 32)
	{
		iVelocityX = fixSubber(iVelocityX, 32);
	}
	else if (iVelocityX <= -30)
	{
		iVelocityX = fixAdder(iVelocityX, 32);
	}
	else
	{
		iVelocityX = 0;
	}
}