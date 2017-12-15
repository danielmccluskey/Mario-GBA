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
enum MARIOFACING
{
	FACERIGHT,
	FACELEFT
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
void MarioManager::CreateMario(SpriteManager& a_oSpriteManager)
{
	iSpriteID=	a_oSpriteManager.CreateSprite((u16*)Mario_SmallTiles, (u16*)Mario_SmallPal, Mario_SmallTilesLen, Mario_SmallPalLen*3, MarioTileBlock, MarioPalb);
	a_oSpriteManager.MoveSprite(32, 120, iSpriteID);

	iCurrentType = NORMAL;
	fpVelocityX = 0;
	fpVelocityY = 0;
	ix = 32;
	iy = 120;
	bDead = false;
	bFinished = false;

	
	///particleee.InitArray(a_oSpriteManager,0);
	InitFireBall(a_oSpriteManager);
	//particleee.DeleteArray(a_oSpriteManager);
}

void MarioManager::ResetMario(SpriteManager& a_oSpriteManager)
{
	iInvulnerableTime = 0;
	bInvulnerable = false;
	iTimer = -128;
	bFinished = false;
	bDead = false;
	bMoving = false;
	fpVelocityX = 0;
	fpVelocityY = 0;
	iCurrentType = NORMAL;

	for (int i = 0; i < MAX_FIREBALLS; i++)
	{
		if (sfire[i].bActive)
		{
			sfire[i].bActive = false;
		}
	}

	Particlemanager.DeleteArray(a_oSpriteManager);
	EnemyParticlemanager.DeleteArray(a_oSpriteManager);
}

void MarioManager::FlashMario(SpriteManager& a_oSpriteManager)
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
		a_oSpriteManager.ShowSprite(iSpriteID);
	}
	if (bInvulnerable && iInvulnerableTime >= 0)
	{
		iInvulnerableTime -= 1;
		iTimer -= 1;
		if (iTimer == 0)
		{
			a_oSpriteManager.HideSprite(iSpriteID);
			iTimer = 20;
		}
		else if (iTimer == 10)
		{
			a_oSpriteManager.ShowSprite(iSpriteID);
		}
	}



}

void MarioManager::TransformMario(s32 a_iMarioType, SpriteManager& a_oSpriteManager, bool a_bHurtMario)
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
		a_oSpriteManager.LoadTiles((u16*)Mario_SmallTiles, (u16*)Mario_SmallPal, Mario_SmallTilesLen, Mario_SmallPalLen*2, MarioTileBlock, MarioPalb);
		a_oSpriteManager.SpriteArray[iSpriteID]->attr0 = a_oSpriteManager.setSpriteAttr0(fix2int(iy), 0, 0, 0, A0_4BPP, A0_SQUARE);
		a_oSpriteManager.SpriteArray[iSpriteID]->attr1 = a_oSpriteManager.setSpriteAttr1(fix2int(ix), 1, 0, 0, A1_SIZE_1);


		
	}
	break;
	case TALL:
	{
		iSpriteHeight = 32;
		iFrameSize = 8;
		iy = fix2int(iy);
		iy -= 16;
		a_oSpriteManager.LoadTiles((u16*)Mario_TallTiles, (u16*)Mario_TallPal, Mario_TallTilesLen, Mario_TallPalLen * 2, MarioTileBlock, MarioPalb);
		a_oSpriteManager.SpriteArray[iSpriteID]->attr0 = a_oSpriteManager.setSpriteAttr0(iy, 0, 0, 0, A0_4BPP, A0_TALL);
		a_oSpriteManager.SpriteArray[iSpriteID]->attr1 = a_oSpriteManager.setSpriteAttr1(fix2int(ix), 1, 0, 0, A1_SIZE_2);
		iy = int2fix(iy);

		break;
		
	}
	
	case FIRE:
	{
		iSpriteHeight = 32;
		iFrameSize = 8;
		iy = fix2int(iy);
		iy -= 16;
		a_oSpriteManager.LoadTiles((u16*)Mario_FireTiles, (u16*)Mario_FirePal, Mario_FireTilesLen, Mario_FirePalLen * 2, MarioTileBlock, MarioPalb);
		a_oSpriteManager.SpriteArray[iSpriteID]->attr0 = a_oSpriteManager.setSpriteAttr0(iy, 0, 0, 0, A0_4BPP, A0_TALL);
		a_oSpriteManager.SpriteArray[iSpriteID]->attr1 = a_oSpriteManager.setSpriteAttr1(fix2int(ix), 1, 0, 0, A1_SIZE_2);
		iy = int2fix(iy);
		break;
	}




	}

}

void MarioManager::MoveMario(s32 a_ix, s32 a_iy, SpriteManager& a_oSpriteManager)
{
	if ((fpVelocityX < MAXXVELOCITY) && (fpVelocityX > -MAXXVELOCITY))
	{
		fpVelocityX = fixAdd(a_ix, fpVelocityX);
	}
	if (fpVelocityY < MAXYVELOCITY)
	{
		fpVelocityY = fixAdd(fpVelocityY, 10);
		fpVelocityY = fixAdd(a_iy, fpVelocityY);
	}
}

void MarioManager::SetPos(s32 a_ix, s32 a_iy, SpriteManager& a_oSpriteManager)
{
	ix = a_ix;
	iy = a_iy;
	a_oSpriteManager.MoveSprite(ix, iy, iSpriteID);
}

u16 MarioManager::MapManager(const unsigned short* a_bgCollisionMap, SpriteManager a_oSpriteManager)
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
				SetPos(ix + 1, iy, a_oSpriteManager);
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
				SetPos(ix - 1, iy, a_oSpriteManager);
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
				SetPos(ix, iy - 1, a_oSpriteManager);
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
				SetPos(ix, iy + 1, a_oSpriteManager);
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
				SetPos(ix + 1, iy, a_oSpriteManager);
			}
			return 0;
		}
		for (i = 0; i < 4; i++)
		{
			SetPos(ix + 1, iy, a_oSpriteManager);
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
				SetPos(ix - 1, iy, a_oSpriteManager);
			}
			return 0;
		}
		for (i = 0; i < 4; i++)
		{
			SetPos(ix - 1, iy, a_oSpriteManager);
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
				SetPos(ix , iy - 1, a_oSpriteManager);
			}
			return 0;
		}
		for (i = 0; i < 4; i++)
		{
			SetPos(ix, iy - 1, a_oSpriteManager);
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
				SetPos(ix, iy + 1, a_oSpriteManager);
			}
			return 0;
		}
		for ( i = 0; i < 4; i++)
		{
			SetPos(ix, iy + 1, a_oSpriteManager);
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
	uiTopLeft = tile_lookup(iTileX, iTileY, iMapOffsetX,
		iMapOffsetY, (u16*)apiMarioBGCollision, iMapWidth, iMapHeight);

	uiTopMiddle = tile_lookup(iTileX+8, iTileY, iMapOffsetX,
		iMapOffsetY, (u16*)apiMarioBGCollision, iMapWidth, iMapHeight);

	uiBottomLeft = tile_lookup(iTileX, iTileY + iSpriteHeight, iMapOffsetX,
		iMapOffsetY, (u16*)apiMarioBGCollision, iMapWidth, iMapHeight);
	uiTopRight = tile_lookup(iTileX + iSpriteWidth, iTileY, iMapOffsetX,
		iMapOffsetY, (u16*)apiMarioBGCollision, iMapWidth, iMapHeight);
	uiBottomRight = tile_lookup(iTileX + iSpriteWidth, iTileY + iSpriteHeight, iMapOffsetX,
		iMapOffsetY, (u16*)apiMarioBGCollision, iMapWidth, iMapHeight);

	uiAlmostBotRight = tile_lookup(iTileX + iSpriteWidth+2, iTileY + iSpriteHeight - (iSpriteHeight >> 3), iMapOffsetX,
		iMapOffsetY, (u16*)apiMarioBGCollision, iMapWidth, iMapHeight);
	uiAlmostBotLeft = tile_lookup(iTileX-2, iTileY + iSpriteHeight - (iSpriteHeight >> 3), iMapOffsetX,
		iMapOffsetY, (u16*)apiMarioBGCollision, iMapWidth, iMapHeight);


	if (uiAlmostBotRight == ENDMAPTILE)
	{
		bFinished = true;
	}
}

void MarioManager::AnimateMario(SpriteManager& a_oSpriteManager)
{
	if (fpVelocityX != STOPPED || bMoving == true)
	{
		iFrame += iFrameSize;
		if (iFrame >= 4 * iFrameSize)
		{
			iFrame = 0;
		}

		if (fpVelocityX >= 0)
		{
			bFacing = FACERIGHT;
		}
		else
		{
			bFacing = FACELEFT;
		}
	}

	a_oSpriteManager.SetFrame(iFrame, iSpriteID);
}
void MarioManager::PhysicsHandler()
{
	if (fpVelocityX != STOPPED || bMoving == true)
	{
		if (fpVelocityX > STOPPED && ((uiTopRight > COLLISIONTILE || uiAlmostBotRight > COLLISIONTILE)))
		{
			fpVelocityX = STOPPED;
			ix -= 500;
		}
		if (fpVelocityX < STOPPED && ((uiTopLeft > COLLISIONTILE || uiAlmostBotLeft > COLLISIONTILE)))
		{
			fpVelocityX = STOPPED;
		}

		ix = fixAdd(ix, fpVelocityX);
	}

	if (fpVelocityY < STOPPED && ((uiTopLeft > COLLISIONTILE || uiTopRight > COLLISIONTILE)))
	{
		fpVelocityY = PUSHBACK;
	}

	if (uiBottomLeft > COLLISIONTILE || uiBottomRight > COLLISIONTILE)
	{
		iy &= ALIGNMASK;
		if (bOnGround == false)
		{
			bJustLanded = true;
		}
		bOnGround = true;
		if (bJump)
		{
			fpVelocityY = JUMPHEIGHT;
			iy = fixAdd(iy, fpVelocityY);
		}
	}
	else
	{
		bOnGround = false;
		fpVelocityY = fixAdd(fpVelocityY, GRAVITY);
		iy = fixAdd(iy, fpVelocityY);
	}
	bJump = false;


	if (fpVelocityX >= WALKSPEED)
	{
		fpVelocityX = fixSub(fpVelocityX, WALKSPEED);
	}
	else if (fpVelocityX <= -WALKSPEED)
	{
		fpVelocityX = fixAdd(fpVelocityX, WALKSPEED);
	}
	else if (!bMoving)
	{
		fpVelocityX = 0;
		iFrame = 0;
	}
}

void MarioManager::UpdateMario(SpriteManager& a_oSpriteManager, PrizeBlockManager* a_oPrizeBlockManagerArray, u16 a_iScrollOffset)
{
	CheckCollisions();

	if (uiTopMiddle == 5)// && TopLeft <= QUESTIONRANGEB || keyHit(KEYS::DOWN))
	{
		s32 iTileX = fix2int(ix);// >> 8;
		s32 iTileY = fix2int(iy);// >> 8;
		s32 newindex = GrabIndex(iTileX+8, iTileY, iMapOffsetX, iMapOffsetY, (u16*)apiMarioBGCollision, iMapWidth, iMapHeight);
		apiMarioBGCollision[newindex] = 0x0019;
		apiMarioBGCollision[newindex + 1] = 0x0019;
		a_oPrizeBlockManagerArray[0].CreateBlock(ix, iy, a_oPrizeBlockManagerArray, a_oSpriteManager, a_iScrollOffset, false);
	}
	if (uiTopMiddle == 4)// && TopLeft <= QUESTIONRANGEB || keyHit(KEYS::DOWN))
	{
		s32 iTileX = fix2int(ix);// >> 8;
		s32 iTileY = fix2int(iy);// >> 8;
		s32 newindex = GrabIndex(iTileX + 8, iTileY, iMapOffsetX, iMapOffsetY, (u16*)apiMarioBGCollision, iMapWidth, iMapHeight);
		apiMarioBGCollision[newindex] = 0x0019;
		apiMarioBGCollision[newindex + 1] = 0x0019;
		a_oPrizeBlockManagerArray[0].CreateBlock(ix, iy, a_oPrizeBlockManagerArray, a_oSpriteManager, a_iScrollOffset, true);
	}
	FlashMario(a_oSpriteManager);
	PhysicsHandler();
	AnimateMario(a_oSpriteManager);
	UpdateFireBall(a_oSpriteManager);


	if (fix2int(iy) >= SCREEN_H)
	{
		bDead = true;
	}

	




	a_oSpriteManager.MoveSprite(fix2int(ix), fix2int(iy), iSpriteID);
	

	
	if (bJustLanded)
	{
		bJustLanded = false;
		if (Particlemanager.bActive == true)
		{
			Particlemanager.DeleteArray(a_oSpriteManager);
		}
		Particlemanager.InitArray(a_oSpriteManager, 1);
		Particlemanager.bActive = true;
		Particlemanager.SetEmitterPos(ix + int2fix(iSpriteWidth >> 2), iy + int2fix(iSpriteHeight));
	}

	//particleee.bActive = true;
	
	Particlemanager.UpdateParticleArray(a_oSpriteManager);
	EnemyParticlemanager.UpdateParticleArray(a_oSpriteManager);

	


}








void MarioManager::UpdateFireBall(SpriteManager& a_oSpriteManager)
{
	for (int i = 0; i < MAX_FIREBALLS; i++)
	{
		if (sfire[i].bActive)
		{
			s32 iTileXA = (sfire[i].fx >> 8);
			s32 iTileYA = (sfire[i].fy >> 8);
			u16 Bottom = tile_lookup(iTileXA + 4, iTileYA + 8, iMapOffsetX,
				iMapOffsetY, (u16*)apiMarioBGCollision, iMapWidth, iMapHeight);
			u16 Right = tile_lookup(iTileXA + 8, iTileYA + 4, iMapOffsetX,
				iMapOffsetY, (u16*)apiMarioBGCollision, iMapWidth, iMapHeight);

			if (Bottom > COLLISIONTILE)
			{
				sfire[i].fvy = -356;
			}
			sfire[i].fvy = fixAdd(sfire[i].fvy, GRAVITY);
			sfire[i].fx = fixAdd(sfire[i].fx, sfire[i].fvx);
			sfire[i].fy = fixAdd(sfire[i].fy, sfire[i].fvy);

			a_oSpriteManager.MoveSprite(fix2int(sfire[i].fx), fix2int(sfire[i].fy), sfire[i].iSpriteID);

			sfire[i].iFrame++;
			if (sfire[i].iFrame >= 4)
			{
				sfire[i].iFrame = 0;
			}
			a_oSpriteManager.SetFrame(FireballTileBlock+(sfire[i].iFrame*sfire[i].iFrameSize), sfire[i].iSpriteID);

			if (fix2int(sfire[i].fx) > SCREEN_W || Right > COLLISIONTILE)
			{
				sfire[i].bActive = false;
				a_oSpriteManager.HideSprite(sfire[i].iSpriteID);
			}

		}
	}
	
}

void MarioManager::ShootFireBall(SpriteManager& a_oSpriteManager)
{
	for (int i = 0; i < MAX_FIREBALLS; i++)
	{
		if (sfire[i].bActive == false)
		{
			if (bFacing == false)
			{
				sfire[i].fvx = int2fix(2);
			}
			else
			{
				sfire[i].fvx = int2fix(-2);
			}
			
			sfire[i].fvy = int2fix(1);
			sfire[i].fx = ix;
			sfire[i].fy = iy;
			sfire[i].bActive = true;
			a_oSpriteManager.ShowSprite(sfire[i].iSpriteID);
			break;
		}
		
	}
}
void MarioManager::InitFireBall(SpriteManager& a_oSpriteManager)
{

	for (int i = 0; i < MAX_FIREBALLS; i++)
	{
		sfire[i].fx = ix;
		sfire[i].fy = iy;
		sfire[i].iSpriteID = a_oSpriteManager.CreateSprite((u16*)FireballTiles, (u16*)FireballPal, FireballTilesLen, FireballPalLen, FireballTileBlock, FireballPalb);
		a_oSpriteManager.SpriteArray[sfire[i].iSpriteID]->attr0 = a_oSpriteManager.setSpriteAttr0(sfire[i].fy, 2, 0, 0, A0_4BPP, A0_SQUARE);
		a_oSpriteManager.SpriteArray[sfire[i].iSpriteID]->attr1 = a_oSpriteManager.setSpriteAttr1(sfire[i].fx, 0, 0, 0, A1_SIZE_0);
		a_oSpriteManager.SpriteArray[sfire[i].iSpriteID]->attr2 = a_oSpriteManager.setSpriteAttr2(FireballTileBlock, FireballPalb, 0);
		a_oSpriteManager.HideSprite(sfire[i].iSpriteID);
	}

	
}

void MarioManager::CheckFireballCollisions(SpriteManager& a_oSpriteManager, AIManager* a_aoEnemyArray)
{
	

			for (int y = 0; y < MAX_ENEMIES; y++)
			{
				if (a_aoEnemyArray[y].bSquish)
				{
					EnemyParticlemanager.InitArray(a_oSpriteManager, 0);
					EnemyParticlemanager.bActive = true;
					EnemyParticlemanager.SetEmitterPos(int2fix(a_aoEnemyArray[y].ix) + int2fix(iSpriteWidth >> 2), a_aoEnemyArray[y].iy + int2fix(iSpriteHeight));
					a_aoEnemyArray[y].bSquish = false;

				}


				if (a_aoEnemyArray[y].bActive == true && a_aoEnemyArray[y].bDead == false)
				{
					int x2Min = a_aoEnemyArray[y].ix;
					int x2Max = a_aoEnemyArray[y].ix + 16;
					int y2Max = fix2int(a_aoEnemyArray[y].iy) + 16;
					int y2Min = fix2int(a_aoEnemyArray[y].iy);

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
								//a_oSpriteManager.DeleteSprite(a_aoEnemyArray[y].iSpriteID);
								a_aoEnemyArray[y].bSquish = true;
								a_aoEnemyArray[y].bDead = true;
								sfire[i].bActive = false;
								a_oSpriteManager.HideSprite(sfire[i].iSpriteID);
								sfire[i].fx = 0;
								sfire[i].fy = 0;

							}
						}
					}
				}

	}

}

