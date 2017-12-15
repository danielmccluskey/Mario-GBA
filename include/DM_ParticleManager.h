//==============================================================================================================================
// Project: Mario GBA
// File: DM_ParticleManager.h
// Author: Daniel McCluskey
// Date Created: 01/12/17
// Brief: This is the header file That contains the struct and class for managing particles and emitters.
// Last Edited by: (See BitBucket Commits: https://bitbucket.org/Danielmclovin/gba-mario
//==============================================================================================================================
#ifndef _PARTICLEMANAGER_H_
#define _PARTICLEMANAGER_H_


#include "gba_types.h"
#include "gba_math.h"
#include "DM_SpriteManager.h"


struct Particle
{
	fixed fx, fy;//Position of particle
	fixed fvx, fvy;//Velocity of particle
	u32 iLife;//Lifetime left on particle
	u16 iSpriteID;//SpriteID
	u8 iParticleType;//Type of particle
	s32 iGravity;//Gravity value for particle
};

class ParticleManager
{
public:
	//==============================================================================================================================
	//Bools
	//==============================================================================================================================
	bool bActive = false;//If the emitter is active or not

	//==============================================================================================================================
	//Fixed Point
	//==============================================================================================================================
	fixed fx, fy;//The position of the emitter

	//==============================================================================================================================
	//Structure/Class members
	//==============================================================================================================================
	Particle sParticles[MAX_PARTICLES];//The array of particles that can spawn

	//==============================================================================================================================
	//Ints
	//==============================================================================================================================
	s32 iLifeTime = 0;//Lifetime of the emitter
	s32 iMaxParticles = 10;//Max amount of particles the emitter can spawn

	//==============================================================================================================================
	//Voids
	//==============================================================================================================================

	// @brief	Function to Delete all particle sprites in the array
	// @param	SpriteManager& a_oSpriteManager = The global sprite manager
	void DeleteArray(SpriteManager& a_oSpriteManager);

	// @brief	Function to reset a certain particle back to the emitter
	// @param	Particle& a_p = The particle to reset
	void EmitParticle(Particle& a_p);

	// @brief	Function to initilise the emitter and all particles
	// @param	SpriteManager& a_oSpriteManager = The global sprite manager
	// @param	s32 a_iSpriteType = The type of emitter to use
	// @param	s32 a_iLifeTime = The life time of the emitter
	void InitArray(SpriteManager& a_oSpriteManager, s32 a_iSpriteType, s32 a_iLifeTime);

	// @brief	Function to Move the emitter
	// @param	s32 a_ix = X Pos to set
	// @param	s32 a_iy = Y pos to set
	void SetEmitterPos(s32 a_ix, s32 a_iy);

	// @brief	Function to update a particles position
	// @param	Particle& a_p = The particle to update
	// @param	SpriteManager& a_oSpriteManager = The global sprite manager
	void UpdateParticle(Particle& a_p, SpriteManager& a_oSpriteManager);

	// @brief	Function to update all particle sprites in the array
	// @param	SpriteManager& a_oSpriteManager = The global sprite manager
	void UpdateParticleArray(SpriteManager& a_oSpriteManager);
};


#endif // !_PARTICLEMANAGER_H_
