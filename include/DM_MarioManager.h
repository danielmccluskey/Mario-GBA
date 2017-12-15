//==============================================================================================================================
// Project: Mario GBA
// File: DM_MarioManager.h
// Author: Daniel McCluskey
// Date Created: 01/12/17
// Brief: This is the header file that contains the fireball class and the MAriomanager class.
// Last Edited by: (See BitBucket Commits: https://bitbucket.org/Danielmclovin/gba-mario
//==============================================================================================================================
#ifndef _MARIOMANAGER_H_
#define _MARIOMANAGER_H_

#include "DM_AIManager.h"
#include "DM_ParticleManager.h"
#include "DM_PrizeBlockManager.h"
#include "DM_SpriteManager.h"
#include "gba_math.h"

struct Fireball
{
	bool bActive = false;//Flag for active fireballs
	fixed fvx, fvy;//Stores velocity of fireball
	fixed fx, fy;//Stores position of fireball
	u16 iFrame = 0;//Animation frame store
	u16 iFrameSize = 1;//Size of frame
	u16 iLifeTime = 255;//Lifetime of fireball
	u16 iSpriteID;//SpriteID of fireball
};
class MarioManager
{
public:
	//==============================================================================================================================
	//Bools
	//==============================================================================================================================
	bool bBottom = false;//For map mode movement
	bool bDead = false;//Flag for player death
	bool bFacing = false;//What direction the player is facing
	bool bFinished = false;//Flag for finishing the level
	bool bHitPrizeBlock = false;//IF the player has hit a prize block
	bool bInvulnerable = false;//If the player is invulnerable
	bool bJump = false;//Flag to make player jump
	bool bJustLanded = false;//Flag to show if the player has just landed
	bool bLeft = false;//For map mode movement
	bool bMapMode = false;//For map mode functions
	bool bMoving = false;//Determines if the player is moving
	bool bOnGround = false;//Flg for if the player is on the ground
	bool bRight = false;//For map mode movement
	bool bTop = false;//For map mode movement

	//==============================================================================================================================
	//Struct/Class Members
	//==============================================================================================================================
	Fireball sfire[MAX_FIREBALLS];//Array of fireballs that the player can shoot

	ParticleManager EnemyParticlemanager;//Putting a particle class member in the enemy class broke everything so I put it here to save time
	ParticleManager Particlemanager;//Stores particle array


	//==============================================================================================================================
	//Fixed point
	//==============================================================================================================================
	fixed fpVelocityX;//Stores X Velocity
	fixed fpVelocityY;//Stores Y velocity

	//==============================================================================================================================
	//Ints
	//==============================================================================================================================
	
	
	// @brief	Function to get a index position from an array
	// @param	u32 a_uix = x pos to check
	// @param	u32 a_uiy = Y pos to check
	// @param	u32 a_uixscroll = Map scroll offset
	// @param 	u32 a_uiyscroll = Map scroll offset
	// @param 	u16* a_uitilemap = The collision map to check against
	// @param 	u32 a_uitilemap_w = Width of the collision map in tiles
	// @param 	u32 a_uitilemap_h = Height of the collision map in tiles
	s32 GrabIndex(u32 x, u32 y, u32 xscroll, u32 yscroll, u16* tilemap, u32 tilemap_w, u32 tilemap_h);


	s32 iAnimationDelay = 20;//Delay for animation
	s32 iFrame = 0;//Stores frame animation
	s32 iFrameSize = 4;//Stores size of animations
	s32 iMapOffsetX = 0;//Stores Map offset
	s32 iMapOffsetY = 0;//Stores map offset
	s32 iSpriteID;//Stores spriteid
	s32 ix; s32 iy;//Stores player positions
	s8 iCurrentType = 0;//Stores type of mario player is

	//==============================================================================================================================
	//Unsigned ints
	//==============================================================================================================================
	u16 iInvulnerableTime = 0;//Timer for invulnerablity

	// @brief	Function to get a collision tile from an array based off the input
	// @param	u32 a_uix = x pos to check
	// @param	u32 a_uiy = Y pos to check
	// @param	u32 a_uixscroll = Map scroll offset
	// @param 	u32 a_uiyscroll = Map scroll offset
	// @param 	u16* a_uitilemap = The collision map to check against
	// @param 	u32 a_uitilemap_w = Width of the collision map in tiles
	// @param 	u32 a_uitilemap_h = Height of the collision map in tiles
	u16 LookupTile(u32 a_uix, u32 a_uiy, u32 a_uixscroll, u32 a_uiyscroll, u16* a_uitilemap, u32 a_uitilemap_w, u32 a_uitilemap_h);

	// @brief	Function to get a collision tile from an array based off the input
	// @param	const unsigned short* a_bgCollisionMap = The collision map to check against
	// @param	SpriteManager& a_oSpriteManager = The global sprite manager
	u16 MapManager(const unsigned short* a_bgCollisionMap, SpriteManager a_oSpriteManager);


	u8 iSpriteHeight = 16;//Stores height of mario
	u8 iSpriteWidth = 16;//Stores width of mario
	u8 uiAlmostBotLeft;//Stores what tile mario is colliding with
	u8 uiAlmostBotRight;//Stores what tile mario is colliding with
	u8 uiBottomLeft;//Stores what tile mario is colliding with
	u8 uiBottomRight;//Stores what tile mario is colliding with
	u8 uiTopLeft;//Stores what tile mario is colliding with
	u8 uiTopMiddle;//Stores what tile mario is colliding with
	u8 uiTopRight;//Stores what tile mario is colliding with
	unsigned short* apiMarioBGCollision;//Current collision map

	//==============================================================================================================================
	//Voids
	//==============================================================================================================================
	
	// @brief	Function to Animate mario if moving
	// @param	SpriteManager& a_oSpriteManager = The global sprite manager
	void AnimateMario(SpriteManager& a_oSpriteManager);

	// @brief	Function to update all collision variables
	void CheckCollisions();

	// @brief	Function to Check collisions of Fireballs against enemies
	// @param	SpriteManager& a_oSpriteManager = The global sprite manager
	// @param	AIManager* a_aoEnemyArray = The enemy array to check
	void CheckFireballCollisions(SpriteManager& a_oSpriteManager, AIManager* a_aoEnemyArray);

	// @brief	Function to create a mario sprite
	// @param	SpriteManager& a_oSpriteManager = The global sprite manager
	void CreateMario(SpriteManager& a_oSpriteManager);

	// @brief	Function to make mario flash if invulnerable
	// @param	SpriteManager& a_oSpriteManager = The global sprite manager
	void FlashMario(SpriteManager& a_oSpriteManager);

	// @brief	Function to Initialise the fireball array
	// @param	SpriteManager& a_oSpriteManager = The global sprite manager
	void InitFireBall(SpriteManager& a_oSpriteManager);

	// @brief	Function to add velocity to mario
	// @param	s32 a_ix = How much to move mario
	// @param	s32 a_iy = How much to move mario
	// @param	SpriteManager& a_oSpriteManager = The global sprite manager
	void MoveMario(s32 a_ix, s32 a_iy, SpriteManager& a_oSpriteManager);

	// @brief	Function to update velocity and position of mario
	void PhysicsHandler();

	// @brief	Function to reset mario when he dies or finishes a map
	// @param	SpriteManager& a_oSpriteManager = The global sprite manager
	void ResetMario(SpriteManager& a_oSpriteManager);

	// @brief	Function to set mario to a certain position
	// @param	s32 a_ix = Where to move mario
	// @param	s32 a_iy = Where to move mario
	// @param	SpriteManager& a_oSpriteManager = The global sprite manager	
	void SetPos(s32 a_ix, s32 a_iy, SpriteManager& a_oSpriteManager);

	// @brief	Function to shoot a fireball
	// @param	SpriteManager& a_oSpriteManager = The global sprite manager
	void ShootFireBall(SpriteManager& a_oSpriteManager);

	// @brief	Function to get a collision tile from an array based off the input
	// @param	s32 a_iMarioType = The type of mario to change to
	// @param	SpriteManager& a_oSpriteManager = The global sprite manager
	// @param	bool a_bHurtMario = Go to previous mode
	void TransformMario(s32 a_iMarioType, SpriteManager& a_oSpriteManager, bool a_bHurtMario);

	// @brief	Function to update the fireball array
	// @param	SpriteManager& a_oSpriteManager = The global sprite manager
	void UpdateFireBall(SpriteManager& a_oSpriteManager);

	// @brief	Function to Update all of mario
	// @param	SpriteManager& a_oSpriteManager = The global sprite manager
	// @param	PrizeBlockManager* a_oPrizeBlockManagerArray = The array of prizeblocks that are spawnable
	// @param	u16 a_iScrollOffset = The current scroll offset for prize blocks
	void UpdateMario(SpriteManager& a_oSpriteManager, PrizeBlockManager* a_oPrizeBlockManagerArray, u16 a_iScrollOffset);
};

#endif // !_MARIOMANAGER_H_
