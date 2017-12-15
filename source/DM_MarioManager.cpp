#include "DM_MarioManager.h"
#include "Mario_Small.h"
#include "Mario_Tall.h"
#include "Mario_Fire.h"
#include "Fireball.h"
#include "gba.h"
#include "gba_math.h"
#include "DM_Enums.h"

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
	ENDMAPTILE = 3,
	MAXXVELOCITY = 550,
	MAXYVELOCITY = 550,
	QUESTIONRANGEA = 2,
	QUESTIONRANGEB = 5
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
#define InvulnerableStartTime 500

s32 iTimer = 0;
s32 iMapWidth = 424;
s32 iMapHeight = 32;
void MarioManager::CreateMario(SpriteManager& a_SpriteManager)
{
	iSpriteID=	a_SpriteManager.CreateSprite((u16*)Mario_SmallTiles, (u16*)Mario_SmallPal, Mario_SmallTilesLen, Mario_SmallPalLen*3, MarioTileBlock, MarioPalb);
	a_SpriteManager.MoveSprite(32, 120, iSpriteID);

	iCurrentType = NORMAL;
	iVelocityX = 0;
	iVelocityY = 0;
	ix = 32;
	iy = 120;
	bDead = false;
	bFinished = false;

	
	///particleee.InitArray(a_SpriteManager,0);
	InitFireBall(a_SpriteManager);
	//particleee.DeleteArray(a_SpriteManager);
}

void MarioManager::ResetMario()
{
	iInvulnerableTime = 0;
	bInvulnerable = false;
	iTimer = -128;
	bFinished = false;
	bDead = false;
	iVelocityX = 0;
	iVelocityY = 0;
	iCurrentType = NORMAL;

	for (int i = 0; i < MAX_FIREBALLS; i++)
	{
		if (sfire[i].bActive)
		{
			sfire[i].bActive = false;
		}
	}
}

void MarioManager::FlashMario(SpriteManager& a_SpriteManager)
{
	if (bInvulnerable && iTimer == -128)
	{
		iInvulnerableTime = InvulnerableStartTime;
		iTimer = 20;

	}

	if (bInvulnerable &&  iInvulnerableTime <= 0)
	{
		iInvulnerableTime = 0;
		bInvulnerable = false;
		iTimer = -128;
		a_SpriteManager.ShowSprite(iSpriteID);
	}
	if (bInvulnerable && iInvulnerableTime >= 0)
	{
		iInvulnerableTime -= 1;
		iTimer -= 1;
		if (iTimer == 0)
		{
			a_SpriteManager.HideSprite(iSpriteID);
			iTimer = 20;
		}
		else if (iTimer == 10)
		{
			a_SpriteManager.ShowSprite(iSpriteID);
		}
	}



}

void MarioManager::TransformMario(s32 a_iMarioType, SpriteManager& a_SpriteManager, bool a_bHurtMario)
{
	if (!a_bHurtMario && a_iMarioType == TALL && iCurrentType == FIRE)
	{
		return;
	}
	if (a_bHurtMario)
	{
		iCurrentType -= 1;
		if (iCurrentType < NORMAL)
		{
			bDead = true;
			return;
		}
	}
	if (a_iMarioType == TALL)
	{
		iCurrentType = a_iMarioType;
	}
	if (a_iMarioType == FIRE)
	{
		iCurrentType = a_iMarioType;
	}

	

	switch (iCurrentType)
	{
	case NORMAL:
	{
		iSpriteHeight = 16;
		iFrameSize = 4;
		a_SpriteManager.LoadTiles((u16*)Mario_SmallTiles, (u16*)Mario_SmallPal, Mario_SmallTilesLen, Mario_SmallPalLen*2, MarioTileBlock, MarioPalb);
		a_SpriteManager.SpriteArray[iSpriteID]->attr0 = a_SpriteManager.setSpriteAttr0(fix2int(iy), 0, 0, 0, A0_4BPP, A0_SQUARE);
		a_SpriteManager.SpriteArray[iSpriteID]->attr1 = a_SpriteManager.setSpriteAttr1(fix2int(ix), 1, 0, 0, A1_SIZE_1);


		
	}
	break;
	case TALL:
	{
		iSpriteHeight = 32;
		iFrameSize = 8;
		iy = fix2int(iy);
		iy -= 16;
		a_SpriteManager.LoadTiles((u16*)Mario_TallTiles, (u16*)Mario_TallPal, Mario_TallTilesLen, Mario_TallPalLen * 2, MarioTileBlock, MarioPalb);
		a_SpriteManager.SpriteArray[iSpriteID]->attr0 = a_SpriteManager.setSpriteAttr0(iy, 0, 0, 0, A0_4BPP, A0_TALL);
		a_SpriteManager.SpriteArray[iSpriteID]->attr1 = a_SpriteManager.setSpriteAttr1(fix2int(ix), 1, 0, 0, A1_SIZE_2);
		iy = int2fix(iy);

		break;
		
	}
	
	case FIRE:
	{
		iSpriteHeight = 32;
		iFrameSize = 8;
		iy = fix2int(iy);
		iy -= 16;
		a_SpriteManager.LoadTiles((u16*)Mario_FireTiles, (u16*)Mario_FirePal, Mario_FireTilesLen, Mario_FirePalLen * 2, MarioTileBlock, MarioPalb);
		a_SpriteManager.SpriteArray[iSpriteID]->attr0 = a_SpriteManager.setSpriteAttr0(iy, 0, 0, 0, A0_4BPP, A0_TALL);
		a_SpriteManager.SpriteArray[iSpriteID]->attr1 = a_SpriteManager.setSpriteAttr1(fix2int(ix), 1, 0, 0, A1_SIZE_2);
		iy = int2fix(iy);
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


	if (keyHit(KEYS::A) && Current >= LEVEL1)
	{
		return Current - LEVEL1+1;
	}

	return 0;

}
u16 MarioManager::tile_lookup(u32 x, u32 y, u32 xscroll, u32 yscroll, u16* tilemap, u32 tilemap_w, u32 tilemap_h)
{
	x += xscroll;
	y += yscroll * 8;
	x >>= 3;
	y >>= 3;
	s32 index = y * tilemap_w + x;
	return tilemap[index];
}
s32 MarioManager::GrabIndex(u32 x, u32 y, u32 xscroll, u32 yscroll, u16* tilemap, u32 tilemap_w, u32 tilemap_h)
{
	x += xscroll;
	y += yscroll * 8;
	x >>= 3;
	y >>= 3;
	s32 index = y * tilemap_w + x;
	return index;
}

void MarioManager::CheckCollisions()
{
	s32 iTileX = fix2int(ix);
	s32 iTileY = fix2int(iy);
	TopLeft = tile_lookup(iTileX, iTileY, iMapOffsetX,
		iMapOffsetY, (u16*)iMarioBGCollision, iMapWidth, iMapHeight);

	TopMiddle = tile_lookup(iTileX+8, iTileY, iMapOffsetX,
		iMapOffsetY, (u16*)iMarioBGCollision, iMapWidth, iMapHeight);

	BottomLeft = tile_lookup(iTileX, iTileY + iSpriteHeight, iMapOffsetX,
		iMapOffsetY, (u16*)iMarioBGCollision, iMapWidth, iMapHeight);
	TopRight = tile_lookup(iTileX + iSpriteWidth, iTileY, iMapOffsetX,
		iMapOffsetY, (u16*)iMarioBGCollision, iMapWidth, iMapHeight);
	BottomRight = tile_lookup(iTileX + iSpriteWidth, iTileY + iSpriteHeight, iMapOffsetX,
		iMapOffsetY, (u16*)iMarioBGCollision, iMapWidth, iMapHeight);

	AlmostBotRight = tile_lookup(iTileX + iSpriteWidth+2, iTileY + iSpriteHeight - (iSpriteHeight >> 3), iMapOffsetX,
		iMapOffsetY, (u16*)iMarioBGCollision, iMapWidth, iMapHeight);
	AlmostBotLeft = tile_lookup(iTileX-2, iTileY + iSpriteHeight - (iSpriteHeight >> 3), iMapOffsetX,
		iMapOffsetY, (u16*)iMarioBGCollision, iMapWidth, iMapHeight);


	if (AlmostBotRight == ENDMAPTILE)
	{
		bFinished = true;
	}
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
		if (bOnGround == false)
		{
			bJustLanded = true;
		}
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

void MarioManager::UpdateMario(SpriteManager& a_SpriteManager, PrizeBlockManager* a_PrizeBlockManagerArray, u16 a_iScrollOffset)
{
	CheckCollisions();

	if (TopMiddle == 5)// && TopLeft <= QUESTIONRANGEB || keyHit(KEYS::DOWN))
	{
		s32 iTileX = fix2int(ix);// >> 8;
		s32 iTileY = fix2int(iy);// >> 8;
		s32 newindex = GrabIndex(iTileX+8, iTileY, iMapOffsetX, iMapOffsetY, (u16*)iMarioBGCollision, iMapWidth, iMapHeight);
		iMarioBGCollision[newindex] = 0x0019;
		iMarioBGCollision[newindex + 1] = 0x0019;
		a_PrizeBlockManagerArray[0].CreateBlock(ix, iy, a_PrizeBlockManagerArray, a_SpriteManager, a_iScrollOffset, false);
	}
	if (TopMiddle == 4)// && TopLeft <= QUESTIONRANGEB || keyHit(KEYS::DOWN))
	{
		s32 iTileX = fix2int(ix);// >> 8;
		s32 iTileY = fix2int(iy);// >> 8;
		s32 newindex = GrabIndex(iTileX + 8, iTileY, iMapOffsetX, iMapOffsetY, (u16*)iMarioBGCollision, iMapWidth, iMapHeight);
		iMarioBGCollision[newindex] = 0x0019;
		iMarioBGCollision[newindex + 1] = 0x0019;
		a_PrizeBlockManagerArray[0].CreateBlock(ix, iy, a_PrizeBlockManagerArray, a_SpriteManager, a_iScrollOffset, true);
	}
	FlashMario(a_SpriteManager);
	PhysicsHandler();
	AnimateMario(a_SpriteManager);
	UpdateFireBall(a_SpriteManager);


	if (fix2int(iy) >= SCREEN_H)
	{
		bDead = true;
	}

	




	a_SpriteManager.MoveSprite(fix2int(ix), fix2int(iy), iSpriteID);
	

	
	if (bJustLanded)
	{
		bJustLanded = false;
		if (Particlemanager.bActive == true)
		{
			Particlemanager.DeleteArray(a_SpriteManager);
		}
		Particlemanager.InitArray(a_SpriteManager, 1);
		Particlemanager.bActive = true;
		Particlemanager.SetEmitterPos(ix + int2fix(iSpriteWidth >> 2), iy + int2fix(iSpriteHeight));
	}

	//particleee.bActive = true;
	
	Particlemanager.UpdateParticleArray(a_SpriteManager);
	EnemyParticlemanager.UpdateParticleArray(a_SpriteManager);

	


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
				iMapOffsetY, (u16*)iMarioBGCollision, iMapWidth, iMapHeight);
			u16 Right = tile_lookup(iTileXA + 8, iTileYA + 4, iMapOffsetX,
				iMapOffsetY, (u16*)iMarioBGCollision, iMapWidth, iMapHeight);

			if (Bottom > COLLISIONTILE)
			{
				sfire[i].fvy = -256;
			}
			sfire[i].fvy = fixAdd(sfire[i].fvy, GRAVITY);
			sfire[i].fx = fixAdd(sfire[i].fx, sfire[i].fvx);
			sfire[i].fy = fixAdd(sfire[i].fy, sfire[i].fvy);

			a_SpriteManager.MoveSprite(fix2int(sfire[i].fx), fix2int(sfire[i].fy), sfire[i].iSpriteID);

			sfire[i].iFrame++;
			if (sfire[i].iFrame >= 4)
			{
				sfire[i].iFrame = 0;
			}
			a_SpriteManager.SetFrame(FireballTileBlock+(sfire[i].iFrame*sfire[i].iFrameSize), sfire[i].iSpriteID);

			if (fix2int(sfire[i].fx) > SCREEN_W || Right > COLLISIONTILE)
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
			sfire[i].fvx = int2fix(2);
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
		sfire[i].iSpriteID = a_SpriteManager.CreateSprite((u16*)FireballTiles, (u16*)FireballPal, FireballTilesLen, FireballPalLen, FireballTileBlock, FireballPalb);
		a_SpriteManager.SpriteArray[sfire[i].iSpriteID]->attr0 = a_SpriteManager.setSpriteAttr0(sfire[i].fy, 2, 0, 0, A0_4BPP, A0_SQUARE);
		a_SpriteManager.SpriteArray[sfire[i].iSpriteID]->attr1 = a_SpriteManager.setSpriteAttr1(sfire[i].fx, 0, 0, 0, A1_SIZE_0);
		a_SpriteManager.SpriteArray[sfire[i].iSpriteID]->attr2 = a_SpriteManager.setSpriteAttr2(FireballTileBlock, FireballPalb, 0);
		a_SpriteManager.HideSprite(sfire[i].iSpriteID);
	}

	
}

void MarioManager::CheckFireballCollisions(SpriteManager& a_SpriteManager, AIManager* a_EnemyArray)
{
	

			for (int y = 0; y < MAX_ENEMIES; y++)
			{
				if (a_EnemyArray[y].bSquish)
				{
					EnemyParticlemanager.InitArray(a_SpriteManager, 0);
					EnemyParticlemanager.bActive = true;
					EnemyParticlemanager.SetEmitterPos(ix + int2fix(iSpriteWidth >> 2), iy + int2fix(iSpriteHeight));
					a_EnemyArray[y].bSquish = false;

				}


				if (a_EnemyArray[y].bActive == true && a_EnemyArray[y].bDead == false)
				{
					int x2Min = a_EnemyArray[y].ix;
					int x2Max = a_EnemyArray[y].ix + 16;
					int y2Max = fix2int(a_EnemyArray[y].iy) + 16;
					int y2Min = fix2int(a_EnemyArray[y].iy);

					for (int i = 0; i < MAX_FIREBALLS; i++)
					{
						if (sfire[i].bActive == true)
						{
							int x1Min = fix2int(sfire[i].fx);
							int x1Max = fix2int(sfire[i].fx) + 8;
							int y1Max = fix2int(sfire[i].fy) + 8;
							int y1Min = fix2int(sfire[i].fy);
							
							if (x1Max < x2Min || x1Min > x2Max)
							{
								
							}
							else if (y1Max < y2Min || y1Min > y2Max)
							{
								
							}
							else
							{
								//a_SpriteManager.DeleteSprite(a_EnemyArray[y].iSpriteID);
								a_EnemyArray[y].bDead = true;
								sfire[i].bActive = false;
								a_SpriteManager.HideSprite(sfire[i].iSpriteID);
								sfire[i].fx = 0;
								sfire[i].fy = 0;

							}
						}
					}
				}

	}

}

