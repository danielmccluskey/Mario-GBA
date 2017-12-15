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
	if (a_iSpriteType == 0)//Score particle magic number
	{
		iMaxParticles = 1;
	}
	else//Landing particle
	{
		iMaxParticles = 10;

	}

	for (int i = 0; i < iMaxParticles; ++i)//Loop through particles array
	{
		if (a_iSpriteType == 0)//Score particle magic number
		{
			sParticles[i].iSpriteID = a_oSpriteManager.CreateSprite((u16*)ScoreParticleTiles, (u16*)ScoreParticlePal, ScoreParticleTilesLen, ScoreParticlePalLen, ScoreParticleTileBlock, ScoreParticlePalb);//Create sprite and grab generated spriteID
			a_oSpriteManager.SpriteArray[sParticles[i].iSpriteID]->attr1 = a_oSpriteManager.setSpriteAttr0(fix2int(fy), 0, 0, 0, A0_4BPP, A0_WIDE);	//Set attributes																						 
			a_oSpriteManager.SpriteArray[sParticles[i].iSpriteID]->attr1 = a_oSpriteManager.setSpriteAttr1(fix2int(fx), 0, 0, 0, A1_SIZE_0);//Set attributes
			sParticles[i].iGravity = PARTICLE_GRAVITY;//Set custom particle gravity
			
		}
		else
		{
			sParticles[i].iSpriteID = a_oSpriteManager.CreateSprite((u16*)NewParticleTiles, (u16*)NewParticlePal, NewParticleTilesLen, NewParticlePalLen, ParticleTileBlock, ParticlePalb);//Create sprite and grab generated spriteID
			a_oSpriteManager.SpriteArray[sParticles[i].iSpriteID]->attr1 = a_oSpriteManager.setSpriteAttr0(fix2int(fy), 0, 0, 0, A0_4BPP, A0_SQUARE);//Set attributes																																									 
			a_oSpriteManager.SpriteArray[sParticles[i].iSpriteID]->attr1 = a_oSpriteManager.setSpriteAttr1(fix2int(fx), 0, 0, 0, A1_SIZE_0);//Set attributes
			sParticles[i].iGravity = PARTICLE_ANTIGRAVITY;//Set custom gravity
		}
		iLifeTime = a_iLifeTime;//Set lifetime for emitter
		sParticles[i].fx = fx;//Set pos
		sParticles[i].fy = fy;//Set pos
		sParticles[i].iParticleType = a_iSpriteType;
		bActive = true;//Set to active
		EmitParticle(sParticles[i]);//Set starting valeus
	}
}
void ParticleManager::DeleteArray(SpriteManager& a_oSpriteManager)
{
	for (int i = 0; i < iMaxParticles; ++i)//Loop through particles array
	{
		
		a_oSpriteManager.DeleteSprite(sParticles[i].iSpriteID);//Delete that particle
	
	}
	bActive = false;//Set to inactive
}
void ParticleManager::EmitParticle(Particle& a_p)
{
	a_p.fx = fx; a_p.fy = fy;//Set pos to emitter pos
	a_p.fvx = int2fix(qran_range(-15, 15)); a_p.fvy = int2fix(20 + qran_range(0, 1));//Choose random direction
	a_p.iLife = 0;//Set life to 0

}

void ParticleManager::UpdateParticle(Particle& a_p, SpriteManager& a_oSpriteManager)
{
	a_p.fx = fixAdd(a_p.fx, fixMul(fixMul(a_p.fvx, PARTICLE_FRAMETIME), a_p.iGravity));//Add gravity to X
	a_p.fy = fixAdd(a_p.fy, fixMul(fixMul(a_p.fvy, PARTICLE_FRAMETIME), a_p.iGravity));//Add gravity to Y
	a_p.fvy -= fixMul(a_p.iGravity, PARTICLE_FRAMETIME);//Subtract gravity from Y
	a_p.iLife += 1;//Add to life counter
	
	if (a_p.iLife >= 200)//If life is over 200
	{
		EmitParticle(a_p);//Reset particle
	}	
}

void ParticleManager::UpdateParticleArray(SpriteManager& a_oSpriteManager)
{
	if (bActive)//If emitter is active
	{
		for (int i = 0; i < iMaxParticles; ++i)//lloop through particle array
		{			
			UpdateParticle(sParticles[i], a_oSpriteManager);//Update the particles position
			a_oSpriteManager.MoveSprite(fix2int(sParticles[i].fx), fix2int(sParticles[i].fy), sParticles[i].iSpriteID);//move that particle
		}
		if (iLifeTime <= 0)//If the emitters life time has ended
		{
			bActive = false;//Deactivate the emitter
			iLifeTime = MAX_LIFETIME;//Reset timer
			DeleteArray(a_oSpriteManager);//Delete particles
		}
		iLifeTime--;//Decrement counter
		
	}
	

}
void ParticleManager::SetEmitterPos(s32 a_ix, s32 a_iy)
{
	fx = a_ix;//Set pos
	fy = a_iy;//Set pos

	for (int i = 0; i < iMaxParticles; ++i)//Loop through particle array
	{
		EmitParticle(sParticles[i]);//Set pos of all particles

	}

}