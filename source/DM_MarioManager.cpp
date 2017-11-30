#include "DM_MarioManager.h"
#include "Mario_Small.h"
#include "Mario_Tall.h"
#include "gba.h"
#include "BG_Collisions.h"
#include "gba_math.h"


enum MARIOTYPES
{
	NORMAL,
	TALL,
	FIRE
};

enum MarioFrames
{
	STAND,
	WALK1,
	WALK2,
	WALK3,
	WALK4,
	JUMP,
	DEATH,
	SWIM1
};

void MarioManager::CreateMario(SpriteManager& a_SpriteManager)
{
	iSpriteID=	a_SpriteManager.CreateSprite((u16*)Mario_SmallTiles, (u16*)Mario_SmallPal, Mario_SmallTilesLen, Mario_SmallPalLen*3, 4);
	a_SpriteManager.MoveSprite(0, 120, iSpriteID);

	iVelocityX = 0;
	iVelocityY = 0;

	iMaxVelocityX = 550;
	iMaxVelocityY = 500;

	ix = 0;
	iy = 120;

	
	particleee.InitArray(a_SpriteManager);
	InitFireBall(a_SpriteManager);
	//particleee.DeleteArray(a_SpriteManager);
}

void MarioManager::TransformMario(s32 a_iMarioType, SpriteManager& a_SpriteManager)
{
	switch (a_iMarioType)
	{
	case NORMAL:
	{
		iSpriteHeight = 16;
		iFrameSize = 4;
		a_SpriteManager.LoadTiles((u16*)Mario_SmallTiles, (u16*)Mario_SmallPal, Mario_SmallTilesLen, Mario_SmallPalLen * 3, 4);
		a_SpriteManager.SpriteArray[iSpriteID]->attr0 = a_SpriteManager.setSpriteAttr0(iy, 0, 0, 0, A0_4BPP, A0_SQUARE);

		break;
	}
	case TALL:
	{
		iSpriteHeight = 32;
		iFrameSize = 8;
		a_SpriteManager.LoadTiles((u16*)Mario_TallTiles, (u16*)Mario_TallPal, Mario_TallTilesLen, Mario_TallPalLen * 3, 4);
		a_SpriteManager.SpriteArray[iSpriteID]->attr0 = a_SpriteManager.setSpriteAttr0(iy, 0, 0, 0, A0_4BPP, A0_TALL);
		a_SpriteManager.SpriteArray[iSpriteID]->attr1 = a_SpriteManager.setSpriteAttr1(0, 1, 0, 0, A1_SIZE_2);
		break;
	}
	case FIRE:
	{
		break;
	}




	}

}

void MarioManager::MoveMario(s32 a_ix, s32 a_iy, SpriteManager& a_SpriteManager)
{
	if ((iVelocityX < iMaxVelocityX) && (iVelocityX > -iMaxVelocityX))
	{
		iVelocityX = fixAdd(a_ix, iVelocityX);
	}
	if (iVelocityY < iMaxVelocityY)
	{
		iVelocityY = fixAdd(iVelocityY, 10);
		iVelocityY = fixAdd(a_iy, iVelocityY);
	}
}
u16 tile_lookup(u32 x, u32 y, u32 xscroll, u32 yscroll,
	u16* tilemap, u32 tilemap_w, u32 tilemap_h) {

	x += xscroll;
	y += yscroll;

	/* convert from screen coordinates to tile coordinates */
	x >>= 3;
	y >>= 3;

	//y = 44 + y;

	/* account for wraparound */
	while (x >= tilemap_w) {
		x -= tilemap_w;
	}
	while (y >= tilemap_h) {
		y -= tilemap_h;
	}
	while (x < 0) {
		x += tilemap_w;
	}
	while (y < 0) {
		y += tilemap_h;
	}

	/* lookup this tile from the map */
	s32 index = y * tilemap_w + x;

	/* return the tile */
	return tilemap[index];
}

void MarioManager::UpdateMario(SpriteManager& a_SpriteManager)
{
	s32 iTileTest = 1;

	s32 iTileX = ix >> 8;
	s32 iTileY = iy >> 8;

	u16 TopLeft = tile_lookup(iTileX, iTileY, iMapOffset,
	44*8, (u16*)bgCollision, 424, 64);
	u16 BottomLeft = tile_lookup(iTileX, iTileY +iSpriteHeight, iMapOffset,
		44*8, (u16*)bgCollision, 424, 64);
	u16 TopRight = tile_lookup(iTileX +iSpriteWidth, iTileY, iMapOffset,
		44*8, (u16*)bgCollision, 424, 64);
	u16 BottomRight = tile_lookup(iTileX +iSpriteWidth, iTileY +iSpriteHeight, iMapOffset,
		44*8, (u16*)bgCollision, 424, 64);

	u16 AlmostBotRight = tile_lookup(iTileX + iSpriteWidth, iTileY + iSpriteHeight-2, iMapOffset,
		44 * 8, (u16*)bgCollision, 424, 64);
	u16 AlmostBotLeft = tile_lookup(iTileX, iTileY + iSpriteHeight - 2, iMapOffset,
		44 * 8, (u16*)bgCollision, 424, 64);
	
	if (iVelocityX > 0 && ((TopRight > iTileTest || AlmostBotRight > iTileTest)))
	{
		iVelocityX = 0;
	}
	if (iVelocityX < 0 && ((TopLeft > iTileTest || AlmostBotLeft > iTileTest)))
	{
		iVelocityX = 0;
	}

	a_SpriteManager.SetFrame(0, iSpriteID);

	if (iVelocityX != 0)
	{		
		iFrame += iFrameSize;
		if (iFrame >= 4*iFrameSize)
		{
			iFrame = 0;
		}
		
		
	}

	ix = fixAdd(ix, iVelocityX);
	
	if (iVelocityY < 0 && ((TopLeft > iTileTest || TopRight > iTileTest)))
	{
		iVelocityY = 8;
	}

	if (BottomLeft > iTileTest || BottomRight > iTileTest)
	{
		//iVelocityY = -31;
		iy &= ~0x7ff;
		bOnGround = true;
		if (bJump)
		{
			iFrame = 4;
			iVelocityY = -1024;
			iy = fixAdd(iy, iVelocityY);			
		}		
	}
	else
	{
		bOnGround = false;
		iVelocityY = fixAdd(iVelocityY, 32);
		iy = fixAdd(iy, iVelocityY);		
	}
	bJump = false;
	
	a_SpriteManager.SetFrame(iFrame, iSpriteID);
	a_SpriteManager.MoveSprite(fix2int(ix), fix2int(iy), iSpriteID);
	if (iVelocityX >= 32)
	{
		iVelocityX = fixSub(iVelocityX, 32);
	}
	else if (iVelocityX <= -32)
	{
		iVelocityX = fixAdd(iVelocityX, 32);
	}
	else
	{
		iVelocityX = 0;
		iFrame = 0;
	}


	particleee.bActive = true;
	particleee.SetEmitterPos(ix, iy+iSpriteHeight);
	particleee.UpdateParticleArray(a_SpriteManager);




}


void MarioManager::ShootFireBall(SpriteManager& a_SpriteManager)
{
	
}
void MarioManager::InitFireBall(SpriteManager& a_SpriteManager)
{
	sfire.fx = ix;
	sfire.fy = iy;
	sfire.iSpriteID = a_SpriteManager.CreateSprite((u16*)Mario_SmallTiles, (u16*)Mario_SmallPal, Mario_SmallTilesLen, Mario_SmallPalLen * 3, 4);
	a_SpriteManager.SpriteArray[sfire.iSpriteID]->attr0 = a_SpriteManager.setSpriteAttr0(sfire.fy, 0, 0, 0, A0_4BPP, A0_SQUARE);
	a_SpriteManager.SpriteArray[sfire.iSpriteID]->attr1 = a_SpriteManager.setSpriteAttr1(sfire.fx, 0, 0, 0, A1_SIZE_0);
}

