#include "gba.h"
#include "DM_ParticleManager.h"
#include "Mario_Small.h"
#include "NewParticle.h"
#include "ScoreParticle.h"
#define MAX_LIFETIME 200



enum PARTICLEENUM
{
	PARTICLE_GRAVITY = -500,
	PARTICLE_ANTIGRAVITY = -100,
	PARTICLE_FRAMETIME = 0x04,
	PARTICLE_PXELS2METER = 5 << 8
};

void ParticleManager::InitArray(SpriteManager& a_oSpriteManager, s32 a_iSpriteType, s32 a_iLifeTime)
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
			sParticles[i].iSpriteID = a_oSpriteManager.CreateSprite((u16*)ScoreParticleTiles, (u16*)ScoreParticlePal, ScoreParticleTilesLen, ScoreParticlePalLen, ScoreParticleTileBlock, ScoreParticlePalb);//Create sprite and grab generated spriteID
			a_oSpriteManager.SpriteArray[sParticles[i].iSpriteID]->attr1 = a_oSpriteManager.setSpriteAttr0(fix2int(fy), 0, 0, 0, A0_4BPP, A0_WIDE);																								  //a_oSpriteManager.SpriteArray[sParticles[i].iSpriteID]->attr0 = a_oSpriteManager.setSpriteAttr0(fy, 0, 0, 0, A0_4BPP, A0_SQUARE);//Set attributes
			a_oSpriteManager.SpriteArray[sParticles[i].iSpriteID]->attr1 = a_oSpriteManager.setSpriteAttr1(fix2int(fx), 0, 0, 0, A1_SIZE_0);//Set attributes
			sParticles[i].iGravity = PARTICLE_GRAVITY;
			//a_oSpriteManager.HideSprite(sParticles[i].iSpriteID);//Hide sprite
			
		}
		else
		{
			//sParticles[i].iSpriteID = a_oSpriteManager.CreateSprite((u16*)ScoreParticleTiles, (u16*)ScoreParticlePal, ScoreParticleTilesLen, ScoreParticlePalLen, ScoreParticleTileBlock, ScoreParticlePalb);//Create sprite and grab generated spriteID

			sParticles[i].iSpriteID = a_oSpriteManager.CreateSprite((u16*)NewParticleTiles, (u16*)NewParticlePal, NewParticleTilesLen, NewParticlePalLen, ParticleTileBlock, ParticlePalb);//Create sprite and grab generated spriteID
			a_oSpriteManager.SpriteArray[sParticles[i].iSpriteID]->attr1 = a_oSpriteManager.setSpriteAttr0(fix2int(fy), 0, 0, 0, A0_4BPP, A0_SQUARE);																																										  //a_oSpriteManager.SpriteArray[sParticles[i].iSpriteID]->attr0 = a_oSpriteManager.setSpriteAttr0(fy, 0, 0, 0, A0_4BPP, A0_SQUARE);//Set attributes
			a_oSpriteManager.SpriteArray[sParticles[i].iSpriteID]->attr1 = a_oSpriteManager.setSpriteAttr1(fix2int(fx), 0, 0, 0, A1_SIZE_0);//Set attributes
			//a_oSpriteManager.HideSprite(sParticles[i].iSpriteID);//Hide sprite
			sParticles[i].iGravity = PARTICLE_ANTIGRAVITY;
		}
		iLifeTime = a_iLifeTime;
		sParticles[i].fx = fx;
		sParticles[i].fy = fy;
		sParticles[i].iParticleType = a_iSpriteType;
		bActive = true;
		EmitParticle(sParticles[i]);
	}
}
void ParticleManager::DeleteArray(SpriteManager& a_oSpriteManager)
{
	for (int i = 0; i < iMaxParticles; ++i)
	{
		bActive = false;
		a_oSpriteManager.DeleteSprite(sParticles[i].iSpriteID);
	
	}
}

void ParticleManager::InitParticle(Particle& a_p)
{
	a_p.fx = 0; a_p.fy = 0; a_p.fvx = 0; a_p.fvy = 0; a_p.iLife = 0x1FF;
}

void ParticleManager::EmitParticle(Particle& a_p)
{
	a_p.fx = fx; a_p.fy = fy;
	a_p.fvx = int2fix(qran_range(-15, 15)); a_p.fvy = int2fix(20 + qran_range(0, 1));
	a_p.iLife = 0;

}

void ParticleManager::UpdateParticle(Particle& a_p, SpriteManager& a_oSpriteManager)
{
	a_p.fx = fixAdd(a_p.fx, fixMul(fixMul(a_p.fvx, PARTICLE_FRAMETIME), a_p.iGravity));
	a_p.fy = fixAdd(a_p.fy, fixMul(fixMul(a_p.fvy, PARTICLE_FRAMETIME), a_p.iGravity));
	a_p.fvy -= fixMul(a_p.iGravity, PARTICLE_FRAMETIME);
	a_p.iLife += 1;
	

	//EmitParticle(a_p);
		
		
		
	
}

void ParticleManager::UpdateParticleArray(SpriteManager& a_oSpriteManager)
{
	if (bActive)
	{
		for (int i = 0; i < iMaxParticles; ++i)
		{
			
			UpdateParticle(sParticles[i], a_oSpriteManager);
			
			a_oSpriteManager.MoveSprite(fix2int(sParticles[i].fx), fix2int(sParticles[i].fy), sParticles[i].iSpriteID);
			
			
			
		}
		if (iLifeTime <= 0)
		{
			bActive = false;
			iLifeTime = MAX_LIFETIME;
			DeleteArray(a_oSpriteManager);
		}
		iLifeTime--;
		
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