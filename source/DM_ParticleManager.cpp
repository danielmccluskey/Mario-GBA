#include "gba.h"
#include "DM_ParticleManager.h"
#include "Mario_Small.h"

fixed g_gravity = -0x9CC;
fixed g_frameTime = 0x04;
fixed g_pixels2Meter = int2fix(5);

void ParticleManager::InitArray(SpriteManager& a_SpriteManager)
{
	

	
	for (int i = 0; i < 12; ++i)
	{
		sParticles[i].iSpriteID = a_SpriteManager.CreateSprite((u16*)Mario_SmallTiles, (u16*)Mario_SmallPal, Mario_SmallTilesLen, Mario_SmallPalLen * 3, 4);
		EmitParticle(sParticles[i]);
	}
}
void ParticleManager::DeleteArray(SpriteManager& a_SpriteManager)
{
	for (int i = 0; i < 12; ++i)
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
	a_p.fvx = int2fix(qran_range(-5, 5)); a_p.fvy = int2fix(20 + qran_range(0, 5));
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
	}
}