#include "DM_MarioManager.h"
#include "Mario_Small.h"
#include "Mario_Tall.h"
#include "particles.h"
#include "gba.h"
#include "World1Level1_Externs.h"
#include "gba_math.h"

enum MARIOTYPES
{
	NORMAL,
	TALL,
	FIRE
};

enum MARIOPHYSICS
{
	WALKSPEED = 32,
	JUMPHEIGHT = -1024,
	GRAVITY = 32,
	PUSHBACK = 8,
	ALIGNMASK = ~0x7ff,
	STOPPED = 0,
	COLLISIONTILE = 0,
	MAXXVELOCITY = 550,
	MAXYVELOCITY = 550,
};

enum WorldSelection
{
	BLANK,
	WALL,
	PIN,
	LEVEL1,
	LEVEL2,
	LEVEL3,
	LEVEL4,
	LEVEL5,
	LEVEL6
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


s32 iMapWidth = 424;
s32 iMapHeight = 32;
void MarioManager::CreateMario(SpriteManager& a_SpriteManager)
{
	iSpriteID=	a_SpriteManager.CreateSprite((u16*)Mario_SmallTiles, (u16*)Mario_SmallPal, Mario_SmallTilesLen, Mario_SmallPalLen*3, MarioTileBlock, MarioPalb);
	a_SpriteManager.MoveSprite(32, 120, iSpriteID);

	iVelocityX = 0;
	iVelocityY = 0;
	ix = 32;
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
		a_SpriteManager.LoadTiles((u16*)Mario_SmallTiles, (u16*)Mario_SmallPal, Mario_SmallTilesLen, Mario_SmallPalLen * 3, MarioTileBlock, MarioPalb);
		a_SpriteManager.SpriteArray[iSpriteID]->attr0 = a_SpriteManager.setSpriteAttr0(iy, 0, 0, 0, A0_4BPP, A0_SQUARE);

		break;
	}
	case TALL:
	{
		iSpriteHeight = 32;
		iFrameSize = 8;
		a_SpriteManager.LoadTiles((u16*)Mario_TallTiles, (u16*)Mario_TallPal, Mario_TallTilesLen, Mario_TallPalLen * 3, MarioTileBlock, MarioPalb);
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
	if ((iVelocityX < MAXXVELOCITY) && (iVelocityX > -MAXXVELOCITY))
	{
		iVelocityX = fixAdd(a_ix, iVelocityX);
	}
	if (iVelocityY < MAXYVELOCITY)
	{
		iVelocityY = fixAdd(iVelocityY, 10);
		iVelocityY = fixAdd(a_iy, iVelocityY);
	}
}

void MarioManager::SetPos(s32 a_ix, s32 a_iy, SpriteManager& a_SpriteManager)
{
	ix = a_ix;
	iy = a_iy;
	a_SpriteManager.MoveSprite(ix, iy, iSpriteID);
}

u16 MarioManager::MapManager(const unsigned short* a_bgCollisionMap, SpriteManager a_SpriteManager)
{
	
	s32 i = 0;


	s32 iTilesX = (ix >> 3);// >> 3);
	s32 iTilesY = (iy >> 3);// >> 3);

	s32 Current = a_bgCollisionMap[iTilesY * 32 + iTilesX];
	s32 Right = a_bgCollisionMap[iTilesY * 32 + (iTilesX + 1)];
	s32 Left = a_bgCollisionMap[iTilesY * 32 + (iTilesX-1)];
	s32 Top = a_bgCollisionMap[(iTilesY - 1) * 32 + iTilesX];
	s32 Bottom = a_bgCollisionMap[(iTilesY + 1) * 32 + iTilesX];


	if (keyHit(KEYS::RIGHT) && bMoving == false)
	{
		if (Right != WALL && bRight == false)
		{
			bRight = true;
			bMoving = true;

			for (i = 0; i < 16; i++)
			{
				SetPos(ix + 1, iy, a_SpriteManager);
			}
		}
	}
	if (keyHit(KEYS::LEFT) && bMoving == false)
	{
		if (Left != WALL && bLeft == false)
		{
			bLeft = true;
			bMoving = true;

			for (i = 0; i < 16; i++)
			{
				SetPos(ix - 1, iy, a_SpriteManager);
			}
		}
	}
	if (keyHit(KEYS::UP) && bMoving == false)
	{
		if (Top != WALL && bTop == false)
		{
			bTop = true;
			bMoving = true;

			for (i = 0; i < 16; i++)
			{
				SetPos(ix, iy - 1, a_SpriteManager);
			}
		}
	}
	if (keyHit(KEYS::DOWN) && bMoving == false)
	{
		if (Bottom != WALL && bBottom == false)
		{
			bBottom = true;
			bMoving = true;
			for (i = 0; i < 16; i++)
			{
				SetPos(ix, iy + 1, a_SpriteManager);
			}
		}
	}


	if (bRight)
	{
		if (Right >= PIN)
		{
			bRight = false;
			bMoving = false;

			for (i = 0; i < 8; i++)
			{
				SetPos(ix + 1, iy, a_SpriteManager);
			}
			return 0;
		}
		for (i = 0; i < 4; i++)
		{
			SetPos(ix + 1, iy, a_SpriteManager);
		}
	}
	if (bLeft)
	{
		if (Left >= PIN)
		{
			bMoving = false;
			bLeft = false;
			for (i = 0; i < 8; i++)
			{
				SetPos(ix - 1, iy, a_SpriteManager);
			}
			return 0;
		}
		for (i = 0; i < 4; i++)
		{
			SetPos(ix - 1, iy, a_SpriteManager);
		}
	}
	else if (bTop)
	{
		if (Top >= PIN)
		{
			bMoving = false;
			bTop = false;
			for (i = 0; i < 8; i++)
			{
				SetPos(ix , iy - 1, a_SpriteManager);
			}
			return 0;
		}
		for (i = 0; i < 4; i++)
		{
			SetPos(ix, iy - 1, a_SpriteManager);
		}
	}
	else if (bBottom)
	{
		if (Bottom >= PIN)
		{
			bMoving = false;
			bBottom = false;
			for (i = 0; i < 8; i++)
			{
				SetPos(ix, iy + 1, a_SpriteManager);
			}
			return 0;
		}
		for ( i = 0; i < 4; i++)
		{
			SetPos(ix, iy + 1, a_SpriteManager);
		}
	}


	if (keyHit(KEYS::A) && Current == LEVEL1)
	{
		return 1;

	}

	return 0;

}
u16 MarioManager::tile_lookup(u32 x, u32 y, u32 xscroll, u32 yscroll,
	u16* tilemap, u32 tilemap_w, u32 tilemap_h) {

	x += xscroll;
	y += yscroll *8;

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

void MarioManager::CheckCollisions()
{
	s32 iTileX = ix >> 8;
	s32 iTileY = iy >> 8;

	TopLeft = tile_lookup(iTileX, iTileY, iMapOffsetX,
		iMapOffsetY, (u16*)World1Level1Collision, iMapWidth, iMapHeight);
	BottomLeft = tile_lookup(iTileX, iTileY + iSpriteHeight, iMapOffsetX,
		iMapOffsetY, (u16*)World1Level1Collision, iMapWidth, iMapHeight);
	TopRight = tile_lookup(iTileX + iSpriteWidth, iTileY, iMapOffsetX,
		iMapOffsetY, (u16*)World1Level1Collision, iMapWidth, iMapHeight);
	BottomRight = tile_lookup(iTileX + iSpriteWidth, iTileY + iSpriteHeight, iMapOffsetX,
		iMapOffsetY, (u16*)World1Level1Collision, iMapWidth, iMapHeight);

	AlmostBotRight = tile_lookup(iTileX + iSpriteWidth+2, iTileY + iSpriteHeight - 2, iMapOffsetX,
		iMapOffsetY, (u16*)World1Level1Collision, iMapWidth, iMapHeight);
	AlmostBotLeft = tile_lookup(iTileX-2, iTileY + iSpriteHeight - 2, iMapOffsetX,
		iMapOffsetY, (u16*)World1Level1Collision, iMapWidth, iMapHeight);



}

void MarioManager::AnimateMario(SpriteManager& a_SpriteManager)
{
	if (iVelocityX != STOPPED || bMoving == true)
	{
		iFrame += iFrameSize;
		if (iFrame >= 4 * iFrameSize)
		{
			iFrame = 0;
		}
	}

	a_SpriteManager.SetFrame(iFrame, iSpriteID);
}
void MarioManager::PhysicsHandler()
{
	if (iVelocityX != STOPPED || bMoving == true)
	{
		if (iVelocityX > STOPPED && ((TopRight > COLLISIONTILE || AlmostBotRight > COLLISIONTILE)))
		{
			iVelocityX = STOPPED;
			ix -= 500;
		}
		if (iVelocityX < STOPPED && ((TopLeft > COLLISIONTILE || AlmostBotLeft > COLLISIONTILE)))
		{
			iVelocityX = STOPPED;
		}

		ix = fixAdd(ix, iVelocityX);
	}

	if (iVelocityY < STOPPED && ((TopLeft > COLLISIONTILE || TopRight > COLLISIONTILE)))
	{
		iVelocityY = PUSHBACK;
	}

	if (BottomLeft > COLLISIONTILE || BottomRight > COLLISIONTILE)
	{
		iy &= ALIGNMASK;
		bOnGround = true;
		if (bJump)
		{
			iVelocityY = JUMPHEIGHT;
			iy = fixAdd(iy, iVelocityY);
		}
	}
	else
	{
		bOnGround = false;
		iVelocityY = fixAdd(iVelocityY, GRAVITY);
		iy = fixAdd(iy, iVelocityY);
	}
	bJump = false;


	if (iVelocityX >= WALKSPEED)
	{
		iVelocityX = fixSub(iVelocityX, WALKSPEED);
	}
	else if (iVelocityX <= -WALKSPEED)
	{
		iVelocityX = fixAdd(iVelocityX, WALKSPEED);
	}
	else
	{
		iVelocityX = 0;
		iFrame = 0;
	}
}
void MarioManager::UpdateMario(SpriteManager& a_SpriteManager)
{
	CheckCollisions();
	PhysicsHandler();
	AnimateMario(a_SpriteManager);
	UpdateFireBall(a_SpriteManager);	
	a_SpriteManager.MoveSprite(fix2int(ix), fix2int(iy), iSpriteID);
	
	
	


	particleee.bActive = false;
	//particleee.SetEmitterPos(ix, iy+iSpriteHeight);
	//particleee.UpdateParticleArray(a_SpriteManager);

	


}
void MarioManager::UpdateFireBall(SpriteManager& a_SpriteManager)
{
	for (int i = 0; i < MAX_FIREBALLS; i++)
	{
		if (sfire[i].bActive)
		{
			s32 iTileXA = (sfire[i].fx >> 8);
			s32 iTileYA = (sfire[i].fy >> 8);
			u16 Bottom = tile_lookup(iTileXA + 4, iTileYA + 8, iMapOffsetX,
				iMapOffsetY, (u16*)World1Level1Collision, iMapWidth, iMapHeight);
			u16 Right = tile_lookup(iTileXA + 8, iTileYA + 4, iMapOffsetX,
				iMapOffsetY, (u16*)World1Level1Collision, iMapWidth, iMapHeight);

			if (Bottom > 0)
			{
				sfire[i].fvy = -356;
			}
			sfire[i].fvy = fixAdd(sfire[i].fvy, 20);
			sfire[i].fx = fixAdd(sfire[i].fx, sfire[i].fvx);
			sfire[i].fy = fixAdd(sfire[i].fy, sfire[i].fvy);

			a_SpriteManager.MoveSprite(fix2int(sfire[i].fx), fix2int(sfire[i].fy), sfire[i].iSpriteID);

			if (fix2int(sfire[i].fx) > SCREEN_W || Right > 1)
			{
				sfire[i].bActive = false;
				a_SpriteManager.HideSprite(sfire[i].iSpriteID);
			}

		}
	}
	
}

void MarioManager::ShootFireBall(SpriteManager& a_SpriteManager)
{
	for (int i = 0; i < MAX_FIREBALLS; i++)
	{
		if (sfire[i].bActive == false)
		{
			sfire[i].fvx = int2fix(3);
			sfire[i].fvy = int2fix(1);
			sfire[i].fx = ix;
			sfire[i].fy = iy;
			sfire[i].bActive = true;
			a_SpriteManager.ShowSprite(sfire[i].iSpriteID);
			break;
		}
		
	}
}
void MarioManager::InitFireBall(SpriteManager& a_SpriteManager)
{

	for (int i = 0; i < MAX_FIREBALLS; i++)
	{
		sfire[i].fx = ix;
		sfire[i].fy = iy;
		sfire[i].iSpriteID = a_SpriteManager.CreateSprite((u16*)particlesTiles, (u16*)particlesPal, particlesTilesLen, particlesPalLen * 3, ParticleTileBlock, ParticlePalb);
		a_SpriteManager.SpriteArray[sfire[i].iSpriteID]->attr0 = a_SpriteManager.setSpriteAttr0(sfire[i].fy, 2, 0, 0, A0_4BPP, A0_SQUARE);
		a_SpriteManager.SpriteArray[sfire[i].iSpriteID]->attr1 = a_SpriteManager.setSpriteAttr1(sfire[i].fx, 0, 0, 0, A1_SIZE_0);
		a_SpriteManager.SpriteArray[sfire[i].iSpriteID]->attr2 = a_SpriteManager.setSpriteAttr2(FireballTileBlock, 0, 0);
		a_SpriteManager.HideSprite(sfire[i].iSpriteID);
	}

	
}

