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
};

class ParticleManager
{
public:
	fixed fx, fy;


	~ParticleManager() {};
	Particle sParticles[12];

	void InitParticle(Particle& a_p);
	void EmitParticle(Particle& a_p);
	void UpdateParticle(Particle& a_p);
	void InitArray(SpriteManager& a_SpriteManager);
	void DeleteArray(SpriteManager& a_SpriteManager);
};


#endif // !_PARTICLEMANAGER_H_
