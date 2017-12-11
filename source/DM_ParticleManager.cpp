#include "gba.h"
#include "DM_ParticleManager.h"
#include "Mario_Small.h"
#include "particles.h"

fixed g_gravity = -0x9CC;
fixed g_frameTime = 0x04;
fixed g_pixels2Meter = int2fix(5);

void ParticleManager::InitArray(SpriteManager& a_SpriteManager)
{
	for (int i = 0; i < MAX_PARTICLES; ++i)
	{
		sParticles[i].iSpriteID = a_SpriteManager.CreateSprite((u16*)particlesTiles, (u16*)particlesPal, particlesTilesLen, particlesPalLen * 3, ParticleTileBlock, ParticlePalb);
		a_SpriteManager.SpriteArray[sParticles[i].iSpriteID]->attr0 = a_SpriteManager.setSpriteAttr0(fy, 0, 0, 0, A0_4BPP, A0_SQUARE);
		a_SpriteManager.SpriteArray[sParticles[i].iSpriteID]->attr1 = a_SpriteManager.setSpriteAttr1(fx, 0, 0, 0, A1_SIZE_0);
		a_SpriteManager.SpriteArray[sParticles[i].iSpriteID]->attr2 = a_SpriteManager.setSpriteAttr2(129, 0, 0);
		a_SpriteManager.HideSprite(sParticles[i].iSpriteID);
		EmitParticle(sParticles[i]);
	}
}
void ParticleManager::DeleteArray(SpriteManager& a_SpriteManager)
{
	for (int i = 0; i < MAX_PARTICLES; ++i)
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

void ParticleManager::UpdateParticle(Particle& a_p)
{
	a_p.fx = fixAdd(a_p.fx, fixMul(fixMul(a_p.fvx, g_frameTime), g_pixels2Meter));
	a_p.fy = fixAdd(a_p.fy, fixMul(fixMul(a_p.fvy, g_frameTime), g_pixels2Meter));
	a_p.fvy -= fixMul(g_gravity, g_frameTime);
	a_p.iLife += 16;

	if ((a_p.fy - fy) > (40 << 8))
	{
		EmitParticle(a_p);
		bActive = false;
	}
}

void ParticleManager::UpdateParticleArray(SpriteManager& a_SpriteManager)
{
	if (bActive)
	{
		for (int i = 0; i < MAX_PARTICLES; ++i)
		{
			UpdateParticle(sParticles[i]);
			a_SpriteManager.MoveSprite(fix2int(sParticles[i].fx), fix2int(sParticles[i].fy), sParticles[i].iSpriteID);
			u32 frameID = (1 << 9) - sParticles[i].iLife;
			frameID = frameID << 4 >> 9;
			//sParticles[i].at = SetAttrib2(32 + frameID, 0, 1);
		}
	}
	

}
void ParticleManager::SetEmitterPos(s32 a_ix, s32 a_iy)
{
	fx = a_ix;
	fy = a_iy;

}