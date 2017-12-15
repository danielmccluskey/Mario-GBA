#include "gba.h"
#include "DM_ParticleManager.h"
#include "Mario_Small.h"
#include "Fireball.h"
#include "ScoreParticle.h"

fixed g_gravity = -500;
fixed g_antigravity = 0x9CC;
fixed g_frameTime = 0x04;
fixed g_pixels2Meter = int2fix(5);

void ParticleManager::InitArray(SpriteManager& a_SpriteManager, s32 a_iSpriteType)
{
	if (a_iSpriteType == 0)
	{
		iMaxParticles = 1;
	}
	else
	{
		iMaxParticles = 10;

	}

	for (int i = 0; i < iMaxParticles; ++i)
	{
		if (a_iSpriteType == 0)
		{
			sParticles[i].iSpriteID = a_SpriteManager.CreateSprite((u16*)ScoreParticleTiles, (u16*)ScoreParticlePal, ScoreParticleTilesLen, ScoreParticlePalLen, ScoreParticleTileBlock, ScoreParticlePalb);//Create sprite and grab generated spriteID
			a_SpriteManager.SpriteArray[sParticles[i].iSpriteID]->attr1 = a_SpriteManager.setSpriteAttr0(fix2int(fy), 0, 0, 0, A0_4BPP, A0_WIDE);																								  //a_SpriteManager.SpriteArray[sParticles[i].iSpriteID]->attr0 = a_SpriteManager.setSpriteAttr0(fy, 0, 0, 0, A0_4BPP, A0_SQUARE);//Set attributes
			a_SpriteManager.SpriteArray[sParticles[i].iSpriteID]->attr1 = a_SpriteManager.setSpriteAttr1(fix2int(fx), 0, 0, 0, A1_SIZE_0);//Set attributes
			sParticles[i].iGravity = g_gravity;
			//a_SpriteManager.HideSprite(sParticles[i].iSpriteID);//Hide sprite
			
		}
		else
		{
			//sParticles[i].iSpriteID = a_SpriteManager.CreateSprite((u16*)ScoreParticleTiles, (u16*)ScoreParticlePal, ScoreParticleTilesLen, ScoreParticlePalLen, ScoreParticleTileBlock, ScoreParticlePalb);//Create sprite and grab generated spriteID

			sParticles[i].iSpriteID = a_SpriteManager.CreateSprite((u16*)FireballTiles, (u16*)FireballPal, FireballTilesLen, FireballPalLen, FireballTileBlock, FireballPalb);//Create sprite and grab generated spriteID
			a_SpriteManager.SpriteArray[sParticles[i].iSpriteID]->attr1 = a_SpriteManager.setSpriteAttr0(fix2int(fy), 0, 0, 0, A0_4BPP, A0_SQUARE);																																										  //a_SpriteManager.SpriteArray[sParticles[i].iSpriteID]->attr0 = a_SpriteManager.setSpriteAttr0(fy, 0, 0, 0, A0_4BPP, A0_SQUARE);//Set attributes
			a_SpriteManager.SpriteArray[sParticles[i].iSpriteID]->attr1 = a_SpriteManager.setSpriteAttr1(fix2int(fx), 0, 0, 0, A1_SIZE_0);//Set attributes
			//a_SpriteManager.HideSprite(sParticles[i].iSpriteID);//Hide sprite
			sParticles[i].iGravity = g_antigravity;
		}
		
		sParticles[i].fx = fx;
		sParticles[i].fy = fy;
		sParticles[i].iParticleType = a_iSpriteType;
		EmitParticle(sParticles[i]);
	}
}
void ParticleManager::DeleteArray(SpriteManager& a_SpriteManager)
{
	for (int i = 0; i < iMaxParticles; ++i)
	{
		a_SpriteManager.DeleteSprite(sParticles[i].iSpriteID);
	
	}
}

void ParticleManager::InitParticle(Particle& a_p)
{
	a_p.fx = 0; a_p.fy = 0; a_p.fvx = 0; a_p.fvy = 0; a_p.iLife = 0x1FF;
}

void ParticleManager::EmitParticle(Particle& a_p)
{
	a_p.fx = fx; a_p.fy = fy;
	a_p.fvx = int2fix(qran_range(-5, 5)); a_p.fvy = int2fix(20 + qran_range(0, 1));
	a_p.iLife = 0;
}

void ParticleManager::UpdateParticle(Particle& a_p, SpriteManager& a_SpriteManager)
{
	a_p.fx = fixAdd(a_p.fx, fixMul(fixMul(a_p.fvx, g_frameTime), a_p.iGravity));
	a_p.fy = fixAdd(a_p.fy, fixMul(fixMul(a_p.fvy, g_frameTime), a_p.iGravity));
	a_p.fvy -= fixMul(a_p.iGravity, g_frameTime);
	a_p.iLife += 16;

	if ((a_p.iParticleType == 0 && (a_p.fy + fy) < (40 << 8)) || (a_p.iParticleType == 1 && (a_p.fy - fy) > (10 << 8)))
	{
		EmitParticle(a_p);
		for (int y = 0; y < iMaxParticles; ++y)
		{
			a_SpriteManager.DeleteSprite(sParticles[y].iSpriteID);
		}
		
		bActive = false;
	}
}

void ParticleManager::UpdateParticleArray(SpriteManager& a_SpriteManager)
{
	if (bActive)
	{
		for (int i = 0; i < iMaxParticles; ++i)
		{
			a_SpriteManager.ShowSprite(sParticles[i].iSpriteID);
			UpdateParticle(sParticles[i], a_SpriteManager);
			a_SpriteManager.MoveSprite(fix2int(sParticles[i].fx), fix2int(sParticles[i].fy), sParticles[i].iSpriteID);
		}
		
	}
	

}
void ParticleManager::SetEmitterPos(s32 a_ix, s32 a_iy)
{
	fx = a_ix;
	fy = a_iy;

	for (int i = 0; i < iMaxParticles; ++i)
	{
		EmitParticle(sParticles[i]);

	}

}