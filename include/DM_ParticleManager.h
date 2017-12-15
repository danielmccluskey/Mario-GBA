#ifndef _PARTICLEMANAGER_H_
#define _PARTICLEMANAGER_H_


#include "gba_types.h"
#include "gba_math.h"
#include "DM_SpriteManager.h"


struct Particle
{
	fixed fx, fy;
	fixed fvx, fvy;
	u32 iLife;
	u16 iSpriteID;
	u8 iParticleType;
	s32 iGravity;
};

class ParticleManager
{
public:
	fixed fx, fy;
	bool bActive = false;
	s32 iMaxParticles = 10;

	~ParticleManager() {};
	Particle sParticles[10];

	void InitParticle(Particle& a_p);
	void EmitParticle(Particle& a_p);
	void UpdateParticle(Particle& a_p, SpriteManager& a_oSpriteManager);
	void InitArray(SpriteManager& a_oSpriteManager, s32 a_iSpriteType);
	void DeleteArray(SpriteManager& a_oSpriteManager);
	void UpdateParticleArray(SpriteManager& a_oSpriteManager);
	void SetEmitterPos(s32 a_ix, s32 a_iy);
};


#endif // !_PARTICLEMANAGER_H_
