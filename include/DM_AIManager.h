//==============================================================================================================================
// Project: Mario GBA
// File: DM_AIManager.h
// Author: Daniel McCluskey
// Date Created: 01/12/17
// Brief: This is the header file that contains the class for handling the AI of the game and the powerups.
// Last Edited by: (See BitBucket Commits: https://bitbucket.org/Danielmclovin/gba-mario
//==============================================================================================================================
#ifndef _AIMANAGER_H_
#define _AIMANAGER_H_
#include "gba_math.h"
#include "DM_SpriteManager.h"
#include "DM_ParticleManager.h"
class AIManager
{
public:


	/*
	Could not get constructors or deconstructors to work at all, so CreateEnemy function has been used.
	*/


	//==============================================================================================================================
	//Bools
	//==============================================================================================================================
	bool bActive = false;//Controls whether the enemy is active or not
	bool bAnimate = false;//Allows the enemy to be animated
	bool bDead = false;//Controls the death animation of the enemy
	bool bDeleteParticles = false;//Flag for other classes to delete the particles for this member
	bool bOnGround = false;//Flag for whether the enemy is on ground or not
	bool bSquish = false;//Flag for spawning score particles

	//==============================================================================================================================
	//Fixed
	//==============================================================================================================================
	fixed iMaxVelocityY;//Stores MaxYVelocity
	fixed iVelocityY;//Stores Y velocity

	//==============================================================================================================================
	//Ints
	//==============================================================================================================================
	s32 iAnimationDelay = 20;//Delay for animation
	s32 iFrame = 0;//Frame for animation
	s32 iFrameSize = 4;//Size of increment for animation
	s32 iMapOffsetX = 0;//Offset for collisions
	s32 iMapOffsetY = 0;//Offset for collisions
	s32 iSpriteHeight = 16;//Current height of sprite
	s32 iSpriteID = -1;//Current spriteid
	s32 iSpriteWidth = 16;//Current width of sprite
	s32 iStartingFrame = 0;//Stores location of tiles
	s32 ix;//X position
	s32 iy;//Y position

	//==============================================================================================================================
	//s8
	//==============================================================================================================================
	s8 bDirection = -1;//Direction the enemy should move in
	s8 iSpriteType = 0;//The type of enemy

	//==============================================================================================================================
	//Unsigned Shorts
	//==============================================================================================================================
	
	// @brief	Function to check collisions against the mario sprites
	// @param	SpriteManager& a_oSpriteManager = The global sprite manager
	// @param	AIManager* a_AIManager = The enemy array to check
	// @param	s32 a_ix = Xpos of mario
	// @param	s32 a_iy = Ypos of mario
	// @param	s32 a_iSpriteWidth = Width of mario
	// @param	s32 a_iSpriteHeight = Height of mario
	// @param	bool a_bMarioInvulnerable = Invulnerable status of mario
	u16 CheckSpriteCollision(SpriteManager& a_oSpriteManager, AIManager* a_AIManager, s32 a_ix, s32 a_iy, s32 a_iSpriteWidth, s32 a_iSpriteHeight, bool a_bMarioInvulnerable);


	// @brief	Function to get a collision tile from an array based off the input
	// @param	u32 a_uix = x pos to check
	// @param	u32 a_uiy = Y pos to check
	// @param	u32 a_uixscroll = Map scroll offset
	// @param 	u32 a_uiyscroll = Map scroll offset
	// @param 	u16* a_uitilemap = The collision map to check against
	// @param 	u32 a_uitilemap_w = Width of the collision map in tiles
	// @param 	u32 a_uitilemap_h = Height of the collision map in tiles
	u16 LookupTile(u32 a_uix, u32 a_uiy, u32 a_uixscroll, u32 a_uiyscroll, u16* a_uitilemap, u32 a_uitilemap_w, u32 a_uitilemap_h);


	unsigned short* iEnemyBGCollision;//Stores the collision map for enemies

	//==============================================================================================================================
	//Voids
	//==============================================================================================================================
	
	// @brief	Function to create the enemy since contructors dont work
	// @param	SpriteManager& a_oSpriteManager = The global sprite manager
	// @param	AIManager* a_aoEnemyArray = The array of enemies
	// @param	s8 a_iEnemyType = The type of enemy to spawn
	// @param 	s32 a_ix = X pos to spawn enemy at
	// @param 	s32 a_iy = Y pos to spawn enemy at
	void CreateEnemy(SpriteManager& a_oSpriteManager, AIManager* a_aoEnemyArray, s8 a_iEnemyType, s32 a_ix, s32 a_iy);

	// @brief	Function to delete the enemy since decontructors dont work
	// @param	SpriteManager& a_oSpriteManager = The global sprite manager
	// @param	AIManager* a_aoEnemyArray = The array of enemies
	void DeleteEnemies(SpriteManager& a_oSpriteManager, AIManager* a_aoEnemyArray);

	// @brief	Function to move the enemies when the map scrolls
	// @param	SpriteManager& a_oSpriteManager = The global sprite manager
	// @param	AIManager* a_aoEnemyArray = The array of enemies
	// @param	s32 a_ix = How much the enemies should scroll
	void ScrollEnemies(SpriteManager& a_oSpriteManager, AIManager* a_aoEnemyArray, s32 a_ix);

	// @brief	Function to update the enemies
	// @param	SpriteManager& a_oSpriteManager = The global sprite manager
	// @param	AIManager* a_aoEnemyArray = The array of enemies
	void UpdateEnemies(SpriteManager& a_oSpriteManager, AIManager* a_aoEnemyArray);
};
#endif // !_AIMANAGER_H_