//==============================================================================================================================
// Project: Mario GBA
// File: DM_MarioManager.cpp
// Author: Daniel McCluskey
// Date Created: 01/12/17
// Brief: This is the cpp file for that contains the fireball class and the MAriomanager class.
// Last Edited by: (See BitBucket Commits: https://bitbucket.org/Danielmclovin/gba-mario
//==============================================================================================================================
#include "DM_MarioManager.h"
#include "Mario_Small.h"
#include "Mario_Tall.h"
#include "Mario_Fire.h"
#include "Fireball.h"
#include "gba.h"
#include "gba_math.h"

enum MARIOTYPES
{
	MARIO_TYPES_NORMAL,
	MARIO_TYPES_TALL,
	MARIO_TYPES_FIRE
};
enum MARIOFACING
{
	MARIO_FACING_RIGHT,
	MARIO_FACING_LEFT
};

enum MARIOPHYSICS
{
	MARIO_PHYSICS_WALKSPEED = 32,
	MARIO_PHYSICS_JUMPHEIGHT = -1024,
	MARIO_PHYSICS_GRAVITY = 32,
	MARIO_PHYSICS_PUSHBACK = 8,
	MARIO_PHYSICS_ALIGNMASK = ~0x7ff,
	MARIO_PHYSICS_STOPPED = 0,
	MARIO_PHYSICS_COLLISIONTILE = 0,
	MARIO_PHYSICS_ENDMAPTILE = 3,
	MARIO_PHYSICS_MAXXVELOCITY = 550,
	MARIO_PHYSICS_MAXYVELOCITY = 550,
	MARIO_PHYSICS_QUESTIONRANGEA = 2,
	MARIO_PHYSICS_QUESTIONRANGEB = 5,
	MARIO_PHYSICS_XPUSHBACK= 500
};

enum WorldSelection
{
	WORLD_MAP_TILES_BLANK,
	WORLD_MAP_TILES_WALL,
	WORLD_MAP_TILES_PIN,
	WORLD_MAP_TILES_LEVEL1,
	WORLD_MAP_TILES_LEVEL2,
	WORLD_MAP_TILES_LEVEL3,
	WORLD_MAP_TILES_LEVEL4,
	WORLD_MAP_TILES_LEVEL5,
	WORLD_MAP_TILES_LEVEL6
};

enum MarioFrames
{
	MARIO_FRAMES_STAND,
	MARIO_FRAMES_WALK1,
	MARIO_FRAMES_WALK2,
	MARIO_FRAMES_WALK3,
	MARIO_FRAMES_WALK4,
	MARIO_FRAMES_JUMP,
	MARIO_FRAMES_DEATH,
	MARIO_FRAMES_SWIM1
};
#define InvulnerableStartTime 500

s32 iTimer = 0;//Timer for flashing
s32 iMapWidth = 424;//Map width in tiles
s32 iMapHeight = 32;//Map height in tiles
void MarioManager::CreateMario(SpriteManager& a_oSpriteManager)
{
	iSpriteID=	a_oSpriteManager.CreateSprite((u16*)Mario_SmallTiles, (u16*)Mario_SmallPal, Mario_SmallTilesLen, Mario_SmallPalLen*3, MarioTileBlock, MarioPalb);//Create mario and assign a sprite ID
	a_oSpriteManager.MoveSprite(32, 120, iSpriteID);//Move mario to the starting position

	iCurrentType = MARIO_TYPES_NORMAL;//Set mario the small mario
	fpVelocityX = 0;//Reset velocity
	fpVelocityY = 0;//Reset velocity
	ix = 32;//Set Starting position
	iy = 120;//Set starting position
	bDead = false;//Reset dead 
	bFinished = false;//Reset finished	
	InitFireBall(a_oSpriteManager);//Initialise the fireball array
}

void MarioManager::ResetMario(SpriteManager& a_oSpriteManager)
{
	iInvulnerableTime = 0;//Reset invulnerable time
	bInvulnerable = false;//Set mario to vulnerable
	iTimer = -128;//Reset Invulonerable timer
	bFinished = false;//Reset finished flag
	bDead = false;//REset dead flag
	bMoving = false;//Reset moving flag
	fpVelocityX = 0;//Reset velocity
	fpVelocityY = 0;//REset velocity
	iCurrentType = MARIO_TYPES_NORMAL;//Reset marios type

	for (int i = 0; i < MAX_FIREBALLS; i++)//Loop through the fireballs
	{
		if (sfire[i].bActive)//If the fireball is active
		{
			sfire[i].bActive = false;//Deactivate the fireballs
			a_oSpriteManager.HideSprite(sfire[i].iSpriteID);//Hide the sprite
		}
	}

	Particlemanager.DeleteArray(a_oSpriteManager);//Delete all mario particles
	EnemyParticlemanager.DeleteArray(a_oSpriteManager);//Delete all enemy particles
}

void MarioManager::FlashMario(SpriteManager& a_oSpriteManager)
{
	if (bInvulnerable && iTimer == -128)//If Mario is not currently invulnerable
	{
		iInvulnerableTime = InvulnerableStartTime;//Set the timer
		iTimer = 20;//Set the timer for flashing mario

	}

	if (bInvulnerable &&  iInvulnerableTime <= 0)//If the Invulnerable timer has ended
	{
		iInvulnerableTime = 0;//Reset timer
		bInvulnerable = false;//Set to vulnerable
		iTimer = -128;//Reset timer
		a_oSpriteManager.ShowSprite(iSpriteID);//Show mario
	}
	if (bInvulnerable && iInvulnerableTime >= 0)//If the timer is still going
	{
		iInvulnerableTime -= 1;//decremeent invulnerable tiemr
		iTimer -= 1;//decrement timer
		if (iTimer == 0)//If the flash timer is 0
		{
			a_oSpriteManager.HideSprite(iSpriteID);//Hide mario
			iTimer = 20;//Reset timer
		}
		else if (iTimer == 10)//If the timer is 10
		{
			a_oSpriteManager.ShowSprite(iSpriteID);//Hide mario
		}
	}



}

void MarioManager::TransformMario(s32 a_iMarioType, SpriteManager& a_oSpriteManager, bool a_bHurtMario)
{
	if (!a_bHurtMario && a_iMarioType == MARIO_TYPES_TALL && iCurrentType == MARIO_TYPES_FIRE)//If mario picks up a mushroom but is already FIRE mario
	{
		return;//Do nothing
	}
	if (a_bHurtMario)//If mario is hit by an enemy
	{
		iCurrentType -= 1;//Go to the previous mario
		if (iCurrentType < MARIO_TYPES_NORMAL)//If it is less than small mario
		{
			bDead = true;//Set dead flag to true
			return;
		}
	}
	if (a_iMarioType == MARIO_TYPES_TALL)
	{
		iCurrentType = a_iMarioType;
	}
	if (a_iMarioType == MARIO_TYPES_FIRE)
	{
		iCurrentType = a_iMarioType;
	}

	

	switch (iCurrentType)
	{
	case MARIO_TYPES_NORMAL:
	{
		iSpriteHeight = 16;//Set new sprite height
		iFrameSize = 4;//Set new frame size for animations
		a_oSpriteManager.LoadTiles((u16*)Mario_SmallTiles, (u16*)Mario_SmallPal, Mario_SmallTilesLen, Mario_SmallPalLen*2, MarioTileBlock, MarioPalb);//Load New mario tiles
		a_oSpriteManager.SpriteArray[iSpriteID]->attr0 = a_oSpriteManager.setSpriteAttr0(fix2int(iy), 0, 0, 0, A0_4BPP, A0_SQUARE);//Set new sprite sizes
		a_oSpriteManager.SpriteArray[iSpriteID]->attr1 = a_oSpriteManager.setSpriteAttr1(fix2int(ix), 1, 0, 0, A1_SIZE_1);//Set new sprite sizes


		
	}
	break;
	case MARIO_TYPES_TALL:
	{
		iSpriteHeight = 32;//Set new sprite height
		iFrameSize = 8;//Set new frame size for animations
		iy = fix2int(iy);//Convert to screen pos
		iy -= 16;//Pull mario out of the floor
		a_oSpriteManager.LoadTiles((u16*)Mario_TallTiles, (u16*)Mario_TallPal, Mario_TallTilesLen, Mario_TallPalLen * 2, MarioTileBlock, MarioPalb);//Load New mario tiles
		a_oSpriteManager.SpriteArray[iSpriteID]->attr0 = a_oSpriteManager.setSpriteAttr0(iy, 0, 0, 0, A0_4BPP, A0_TALL);//Set new sprite sizes
		a_oSpriteManager.SpriteArray[iSpriteID]->attr1 = a_oSpriteManager.setSpriteAttr1(fix2int(ix), 1, 0, 0, A1_SIZE_2);//Set new sprite sizes
		iy = int2fix(iy);

		break;
		
	}
	
	case MARIO_TYPES_FIRE:
	{
		iSpriteHeight = 32;//Set new sprite height
		iFrameSize = 8;//Set new frame size for animations
		iy = fix2int(iy);//Convert to screen pos
		iy -= 16;//Pull mario out of the floor
		a_oSpriteManager.LoadTiles((u16*)Mario_FireTiles, (u16*)Mario_FirePal, Mario_FireTilesLen, Mario_FirePalLen * 2, MarioTileBlock, MarioPalb);//Load New mario tiles
		a_oSpriteManager.SpriteArray[iSpriteID]->attr0 = a_oSpriteManager.setSpriteAttr0(iy, 0, 0, 0, A0_4BPP, A0_TALL);//Set new sprite sizes
		a_oSpriteManager.SpriteArray[iSpriteID]->attr1 = a_oSpriteManager.setSpriteAttr1(fix2int(ix), 1, 0, 0, A1_SIZE_2);//Set new sprite sizes
		iy = int2fix(iy);
		break;
	}
	}

}

void MarioManager::MoveMario(s32 a_ix, s32 a_iy, SpriteManager& a_oSpriteManager)
{
	if ((fpVelocityX < MARIO_PHYSICS_MAXXVELOCITY) && (fpVelocityX > -MARIO_PHYSICS_MAXXVELOCITY))//If the Velocity is not exceeding the max values on each axis
	{
		fpVelocityX = fixAdd(a_ix, fpVelocityX);//Add the given velocity
	}
	if (fpVelocityY < MARIO_PHYSICS_MAXYVELOCITY)//If the Velocity is not exceeding the max values on each axis
	{
		fpVelocityY = fixAdd(a_iy, fpVelocityY);//Add the given velocity
	}
}

void MarioManager::SetPos(s32 a_ix, s32 a_iy, SpriteManager& a_oSpriteManager)
{
	ix = a_ix;//Set x pos
	iy = a_iy;//Set y pos
	a_oSpriteManager.MoveSprite(ix, iy, iSpriteID);//Move sprite to that pos
}

u16 MarioManager::MapManager(const unsigned short* a_bgCollisionMap, SpriteManager a_oSpriteManager)
{
	
	s32 i = 0;//There are a lot of For loops in this function


	s32 iTilesX = (ix >> 3);//Get tile co ordinates
	s32 iTilesY = (iy >> 3);//Get Tile co ordinates

	//Get collision points around the sprite
	u8 uiCurrent = a_bgCollisionMap[iTilesY * 32 + iTilesX];
	u8 uiRight = a_bgCollisionMap[iTilesY * 32 + (iTilesX + 1)];
	u8 uiLeft = a_bgCollisionMap[iTilesY * 32 + (iTilesX-1)];
	u8 uiTop = a_bgCollisionMap[(iTilesY - 1) * 32 + iTilesX];
	u8 uiBottom = a_bgCollisionMap[(iTilesY + 1) * 32 + iTilesX];


	if (keyHit(KEYS::RIGHT) && bMoving == false)//If the play attempts to move and the sprite is not currently moving
	{
		if (uiRight != WORLD_MAP_TILES_WALL && bRight == false)//If the tile in the attempted move direction is not a wall
		{
			bRight = true;//Set direction flag
			bMoving = true;//Set Moving flag

			for (i = 0; i < 16; i++)//Move the player 16 pixels in that direction
			{
				SetPos(ix + 1, iy, a_oSpriteManager);
			}
		}
	}
	if (keyHit(KEYS::LEFT) && bMoving == false)//If the play attempts to move and the sprite is not currently moving
	{
		if (uiLeft != WORLD_MAP_TILES_WALL && bLeft == false)//If the tile in the attempted move direction is not a wall
		{
			bLeft = true;//Set direction flag
			bMoving = true;//Set Moving flag

			for (i = 0; i < 16; i++)//Move the player 16 pixels in that direction
			{
				SetPos(ix - 1, iy, a_oSpriteManager);
			}
		}
	}
	if (keyHit(KEYS::UP) && bMoving == false)//If the play attempts to move and the sprite is not currently moving
	{
		if (uiTop != WORLD_MAP_TILES_WALL && bTop == false)//If the tile in the attempted move direction is not a wall
		{
			bTop = true;//Set direction flag
			bMoving = true;//Set Moving flag

			for (i = 0; i < 16; i++)//Move the player 16 pixels in that direction
			{
				SetPos(ix, iy - 1, a_oSpriteManager);
			}
		}
	}
	if (keyHit(KEYS::DOWN) && bMoving == false)//If the play attempts to move and the sprite is not currently moving
	{
		if (uiBottom != WORLD_MAP_TILES_WALL && bBottom == false)//If the tile in the attempted move direction is not a wall
		{
			bBottom = true;//Set direction flag
			bMoving = true;//Set Moving flag
			for (i = 0; i < 16; i++)//Move the player 16 pixels in that direction
			{
				SetPos(ix, iy + 1, a_oSpriteManager);
			}
		}
	}


	if (bRight)//If moving
	{
		if (uiRight >= WORLD_MAP_TILES_PIN)//if the player has reached a junction or corner
		{
			bRight = false;//Set direction flag
			bMoving = false;//Set moving flag

			for (i = 0; i < 8; i++)//Move the sprite a final 8 pixels in that direction
			{
				SetPos(ix + 1, iy, a_oSpriteManager);
			}
			return 0;
		}
		for (i = 0; i < 4; i++)//Move the sprite 4 pixels in that direction
		{
			SetPos(ix + 1, iy, a_oSpriteManager);
		}
	}
	if (bLeft)
	{
		if (uiLeft >= WORLD_MAP_TILES_PIN)//if the player has reached a junction or corner
		{
			bMoving = false;//Set moving flag
			bLeft = false;//Set direction flag
			for (i = 0; i < 8; i++)//Move the sprite a final 8 pixels in that direction
			{
				SetPos(ix - 1, iy, a_oSpriteManager);
			}
			return 0;
		}
		for (i = 0; i < 4; i++)//Move the sprite 4 pixels in that direction
		{
			SetPos(ix - 1, iy, a_oSpriteManager);
		}
	}
	else if (bTop)
	{
		if (uiTop >= WORLD_MAP_TILES_PIN)//if the player has reached a junction or corner
		{
			bMoving = false;//Set moving flag
			bTop = false;//Set direction flag
			for (i = 0; i < 8; i++)//Move the sprite a final 8 pixels in that direction
			{
				SetPos(ix , iy - 1, a_oSpriteManager);
			}
			return 0;
		}
		for (i = 0; i < 4; i++)//Move the sprite 4 pixels in that direction
		{
			SetPos(ix, iy - 1, a_oSpriteManager);
		}
	}
	else if (bBottom)
	{
		if (uiBottom >= WORLD_MAP_TILES_PIN)//if the player has reached a junction or corner
		{
			bMoving = false;//Set moving flag
			bBottom = false;//Set direction flag
			for (i = 0; i < 8; i++)//Move the sprite a final 8 pixels in that direction
			{
				SetPos(ix, iy + 1, a_oSpriteManager);
			}
			return 0;
		}
		for ( i = 0; i < 4; i++)//Move the sprite 4 pixels in that direction
		{
			SetPos(ix, iy + 1, a_oSpriteManager);
		}
	}


	if (keyHit(KEYS::A) && uiCurrent >= WORLD_MAP_TILES_LEVEL1)//If the player is attempting the select a level
	{
		return uiCurrent - WORLD_MAP_TILES_LEVEL1+1;//Return the level id
	}

	return 0;

}
u16 MarioManager::LookupTile(u32 a_uix, u32 a_uiy, u32 a_uixscroll, u32 a_uiyscroll, u16* a_uitilemap, u32 a_uitilemap_w, u32 a_uitilemap_h)
{
	a_uix += a_uixscroll;//Add the Map offset to the X value
	a_uiy += a_uiyscroll * 8;//Add the map offset to the Y value
	a_uix >>= 3;//Divide by 8 (TileWidth)
	a_uiy >>= 3;//Divide by 8 (TileHeight)
	s32 iCollisionIndex = a_uiy * a_uitilemap_w + a_uix;//Get the current position in the array.
	return a_uitilemap[iCollisionIndex];//return the tile found at that position
}
s32 MarioManager::GrabIndex(u32 a_uix, u32 a_uiy, u32 a_uixscroll, u32 a_uiyscroll, u16* a_uitilemap, u32 a_uitilemap_w, u32 a_uitilemap_h)
{
	a_uix += a_uixscroll;//Add the Map offset to the X value
	a_uiy += a_uiyscroll * 8;//Add the map offset to the Y value
	a_uix >>= 3;//Divide by 8 (TileWidth)
	a_uiy >>= 3;//Divide by 8 (TileHeight)
	s32 iCollisionIndex = a_uiy * a_uitilemap_w + a_uix;//Get the current position in the array.
	return iCollisionIndex;//return the tile found at that position
}

void MarioManager::CheckCollisions()
{
	s32 iTileX = fix2int(ix);//Sets to screen pos
	s32 iTileY = fix2int(iy);//Sets the screen pos


	//Following is for finding various collision points around the sprite
	uiTopLeft = LookupTile(iTileX, iTileY, iMapOffsetX,
		iMapOffsetY, (u16*)apiMarioBGCollision, iMapWidth, iMapHeight);

	uiTopMiddle = LookupTile(iTileX+(iSpriteWidth >> 2), iTileY, iMapOffsetX,
		iMapOffsetY, (u16*)apiMarioBGCollision, iMapWidth, iMapHeight);

	uiBottomLeft = LookupTile(iTileX, iTileY + iSpriteHeight, iMapOffsetX,
		iMapOffsetY, (u16*)apiMarioBGCollision, iMapWidth, iMapHeight);
	uiTopRight = LookupTile(iTileX + iSpriteWidth, iTileY, iMapOffsetX,
		iMapOffsetY, (u16*)apiMarioBGCollision, iMapWidth, iMapHeight);
	uiBottomRight = LookupTile(iTileX + iSpriteWidth, iTileY + iSpriteHeight, iMapOffsetX,
		iMapOffsetY, (u16*)apiMarioBGCollision, iMapWidth, iMapHeight);

	uiAlmostBotRight = LookupTile(iTileX + iSpriteWidth+2, iTileY + iSpriteHeight - (iSpriteHeight >> 3), iMapOffsetX,
		iMapOffsetY, (u16*)apiMarioBGCollision, iMapWidth, iMapHeight);
	uiAlmostBotLeft = LookupTile(iTileX-2, iTileY + iSpriteHeight - (iSpriteHeight >> 3), iMapOffsetX,
		iMapOffsetY, (u16*)apiMarioBGCollision, iMapWidth, iMapHeight);


	if (uiAlmostBotRight == MARIO_PHYSICS_ENDMAPTILE)//If the player is colliding with the flag at the end of the level
	{
		bFinished = true;
	}
}

void MarioManager::AnimateMario(SpriteManager& a_oSpriteManager)
{
	if (fpVelocityX != MARIO_PHYSICS_STOPPED || bMoving == true)//If the playeris moving
	{
		iFrame += iFrameSize;//Increment frame counter
		if (iFrame >= 4 * iFrameSize)//If the frame counter has gone past the walking animations
		{
			iFrame = 0;//Reset counter
		}

		if (fpVelocityX >= 0)//If velocity is positive
		{
			bFacing = MARIO_FACING_RIGHT;//Face mario right
		}
		else
		{
			bFacing = MARIO_FACING_LEFT;//Face mario left
		}
	}

	a_oSpriteManager.SetFrame(iFrame, iSpriteID);//Set the animation frame
}
void MarioManager::PhysicsHandler()
{
	if (fpVelocityX != MARIO_PHYSICS_STOPPED || bMoving == true)//If mario is moving
	{
		if (fpVelocityX > MARIO_PHYSICS_STOPPED && ((uiTopRight > MARIO_PHYSICS_COLLISIONTILE || uiAlmostBotRight > MARIO_PHYSICS_COLLISIONTILE)))//If mario collides on the right
		{
			fpVelocityX = MARIO_PHYSICS_STOPPED;//Stop mario
			ix -= MARIO_PHYSICS_XPUSHBACK;//Move mario away from the collided block to avoid getting stuck in it
		}
		if (fpVelocityX < MARIO_PHYSICS_STOPPED && ((uiTopLeft > MARIO_PHYSICS_COLLISIONTILE || uiAlmostBotLeft > MARIO_PHYSICS_COLLISIONTILE)))//If mario collides on the left
		{
			fpVelocityX = MARIO_PHYSICS_STOPPED;//Stop mario
			ix += MARIO_PHYSICS_XPUSHBACK;//Move mario away from the collided block to avoid getting stuck in it
		}

		ix = fixAdd(ix, fpVelocityX);//Move mario to the calculated position
	}

	if (fpVelocityY < MARIO_PHYSICS_STOPPED && ((uiTopLeft > MARIO_PHYSICS_COLLISIONTILE || uiTopRight > MARIO_PHYSICS_COLLISIONTILE)))//If mario hits his head off something
	{
		fpVelocityY = MARIO_PHYSICS_PUSHBACK;//Push mario back down to avoid getting stuck
	}

	if (uiBottomLeft > MARIO_PHYSICS_COLLISIONTILE || uiBottomRight > MARIO_PHYSICS_COLLISIONTILE)//If mario is colliding with something below
	{
		iy &= MARIO_PHYSICS_ALIGNMASK;//Align mario with the floor (Removes the last bits of the variable making it aligned with the map since fixed will sometimes merge through the floor)
		if (bOnGround == false)//if mario is not on ground (but has just collided with floor)
		{
			bJustLanded = true;//Set just landed flag (For landing particles)
		}
		bOnGround = true;//Set on ground flag
		if (bJump)//If jump flag active (Controlled from main)
		{
			fpVelocityY = MARIO_PHYSICS_JUMPHEIGHT;//Make mario jump
			iy = fixAdd(iy, fpVelocityY);//Make mario jump
		}
	}
	else
	{
		bOnGround = false;//Set not on ground flag
		fpVelocityY = fixAdd(fpVelocityY, MARIO_PHYSICS_GRAVITY);//Add gravity to mario
		iy = fixAdd(iy, fpVelocityY);
	}
	bJump = false;//Set jump to false


	if (fpVelocityX >= MARIO_PHYSICS_WALKSPEED)//If marios velocity is greater than the walkspeed
	{
		fpVelocityX = fixSub(fpVelocityX, MARIO_PHYSICS_WALKSPEED);//Subtract walk speed from the velocity
	}
	else if (fpVelocityX <= -MARIO_PHYSICS_WALKSPEED)//If marios velocity is less than the walkspeed
	{
		fpVelocityX = fixAdd(fpVelocityX, MARIO_PHYSICS_WALKSPEED);//add walk speed to the velocity
	}
	else if (!bMoving)
	{
		fpVelocityX = 0;
		iFrame = 0;
	}
}

void MarioManager::UpdateMario(SpriteManager& a_oSpriteManager, PrizeBlockManager* a_oPrizeBlockManagerArray, u16 a_iScrollOffset)
{
	CheckCollisions();//Update collision points

	if (uiTopMiddle == 5)//If mario hits left side of prize block
	{
		s32 iTileX = fix2int(ix);//Convert to screen pos
		s32 iTileY = fix2int(iy);//Convert to screen pos
		iTileX += (iSpriteWidth >> 2);//Get index from middle of marios head
		s32 newindex = GrabIndex(iTileX, iTileY, iMapOffsetX, iMapOffsetY, (u16*)apiMarioBGCollision, iMapWidth, iMapHeight);//Get index for collision array
		apiMarioBGCollision[newindex] = 0x0019;//Set to magic number
		apiMarioBGCollision[newindex + 1] = 0x0019;//Set to magic number
		a_oPrizeBlockManagerArray[0].CreateBlock(ix, iy, a_oPrizeBlockManagerArray, a_oSpriteManager, a_iScrollOffset, false);//Create a prize block at that pos
	}
	if (uiTopMiddle == 4)//If mario hits left side of prize block
	{
		s32 iTileX = fix2int(ix);//Convert to screen pos
		s32 iTileY = fix2int(iy);//Convert to screen pos
		iTileX += (iSpriteWidth >> 2);//Get index from middle of marios head
		s32 newindex = GrabIndex(iTileX, iTileY, iMapOffsetX, iMapOffsetY, (u16*)apiMarioBGCollision, iMapWidth, iMapHeight);//Get index for collision array
		apiMarioBGCollision[newindex] = 0x0019;//Set to magic number
		apiMarioBGCollision[newindex - 1] = 0x0019;//Set to magic number
		a_oPrizeBlockManagerArray[0].CreateBlock(ix, iy, a_oPrizeBlockManagerArray, a_oSpriteManager, a_iScrollOffset, true);//Create a prize block at that pos
	}
	FlashMario(a_oSpriteManager);//Make mario flash if he is invulnerable
	PhysicsHandler();//Handle physics
	AnimateMario(a_oSpriteManager);//Animate mario
	UpdateFireBall(a_oSpriteManager);//Update the fireball array


	if (fix2int(iy) >= (SCREEN_H-8))//Simple check to see if mario is off screen
	{
		bDead = true;//Set dead flag
	}

	a_oSpriteManager.MoveSprite(fix2int(ix), fix2int(iy), iSpriteID);//Move mario to calculated positions
	
	if (bJustLanded)//If mario has just landed on the ground make a god awful landing particle thing
	{
		bJustLanded = false;//Reset flag
		if (Particlemanager.bActive == true)//If there is already a particle going on
		{
			Particlemanager.DeleteArray(a_oSpriteManager);//Delete those particles make way for the "newer, betterer" landing particles
		}
		Particlemanager.InitArray(a_oSpriteManager, 1, 20);//Initialise a new landing particle emitter
		Particlemanager.SetEmitterPos(ix + int2fix(iSpriteWidth >> 2), iy + int2fix(iSpriteHeight));//Set new emitter position
	}

	//If you just want constant buggy particles uncomment the below line
	//particleee.bActive = true;
	
	Particlemanager.UpdateParticleArray(a_oSpriteManager);//Update particle arrays
	EnemyParticlemanager.UpdateParticleArray(a_oSpriteManager);//Update particle arrays

	


}








void MarioManager::UpdateFireBall(SpriteManager& a_oSpriteManager)
{
	for (int i = 0; i < MAX_FIREBALLS; i++)//Loop through the fireball array
	{
		if (sfire[i].bActive)//If that fireball is active
		{
			s32 iTileXA = fix2int(sfire[i].fx);//Convert to screen pos
			s32 iTileYA = fix2int(sfire[i].fy);//Convert to screen pos

			//Get various collision points for the sprite
			u16 Bottom = LookupTile(iTileXA + 4, iTileYA + 8, iMapOffsetX,
				iMapOffsetY, (u16*)apiMarioBGCollision, iMapWidth, iMapHeight);
			u16 Right = LookupTile(iTileXA + 8, iTileYA + 4, iMapOffsetX,
				iMapOffsetY, (u16*)apiMarioBGCollision, iMapWidth, iMapHeight);

			if (Bottom > MARIO_PHYSICS_COLLISIONTILE)//If the fireball is colliding with the floor
			{
				sfire[i].fvy = -356;//Make it bounce magic number
			}
			sfire[i].fvy = fixAdd(sfire[i].fvy, MARIO_PHYSICS_GRAVITY);//Add the velocity and gravity
			sfire[i].fx = fixAdd(sfire[i].fx, sfire[i].fvx);//Move the sprite
			sfire[i].fy = fixAdd(sfire[i].fy, sfire[i].fvy);//Move the sprite

			a_oSpriteManager.MoveSprite(fix2int(sfire[i].fx), fix2int(sfire[i].fy), sfire[i].iSpriteID);//Move the sprite

			sfire[i].iFrame++;//Increment the animation frame
			if (sfire[i].iFrame >= 4)//Fireballs have 4 frames
			{
				sfire[i].iFrame = 0;//Reset counter
			}
			a_oSpriteManager.SetFrame(FireballTileBlock+(sfire[i].iFrame*sfire[i].iFrameSize), sfire[i].iSpriteID);//Set the frame

			if (fix2int(sfire[i].fx) > SCREEN_W || fix2int(sfire[i].fx) <= 0 || Right > MARIO_PHYSICS_COLLISIONTILE)//if the fireball goes off screen or hits something
			{
				sfire[i].bActive = false;//Deactivate fireball
				a_oSpriteManager.HideSprite(sfire[i].iSpriteID);//Hide the fireball
			}

		}
	}
	
}

void MarioManager::ShootFireBall(SpriteManager& a_oSpriteManager)
{
	if (iCurrentType == MARIO_TYPES_FIRE)//If mario is currently fire mario
	{
		for (int i = 0; i < MAX_FIREBALLS; i++)//Loop through array
		{
			if (sfire[i].bActive == false)//Choose first iinactive fireball
			{
				if (bFacing == false)//if mario is facing right
				{
					sfire[i].fvx = int2fix(2);//Shoot fireball right
				}
				else//Facing left
				{
					sfire[i].fvx = int2fix(-2);//Shoot fireball left
				}

				sfire[i].fvy = int2fix(1);//Add small increment to y velocity for start
				sfire[i].fx = ix;//Move fireball to mario
				sfire[i].fy = iy;//Move fireball to mario
				sfire[i].bActive = true;//Set fireball to active
				a_oSpriteManager.ShowSprite(sfire[i].iSpriteID);//Shopw the fireball
				break;
			}

		}
	}
	
}
void MarioManager::InitFireBall(SpriteManager& a_oSpriteManager)
{

	for (int i = 0; i < MAX_FIREBALLS; i++)//Loop through fireball array
	{
		sfire[i].fx = ix;//Set position
		sfire[i].fy = iy;//Set position
		sfire[i].iSpriteID = a_oSpriteManager.CreateSprite((u16*)FireballTiles, (u16*)FireballPal, FireballTilesLen, FireballPalLen, FireballTileBlock, FireballPalb);//Assign sprite id and create sprite
		a_oSpriteManager.SpriteArray[sfire[i].iSpriteID]->attr0 = a_oSpriteManager.setSpriteAttr0(sfire[i].fy, 2, 0, 0, A0_4BPP, A0_SQUARE);//Set sprite to square
		a_oSpriteManager.SpriteArray[sfire[i].iSpriteID]->attr1 = a_oSpriteManager.setSpriteAttr1(sfire[i].fx, 0, 0, 0, A1_SIZE_0);//Set sprite to 8x8
		a_oSpriteManager.SpriteArray[sfire[i].iSpriteID]->attr2 = a_oSpriteManager.setSpriteAttr2(FireballTileBlock, FireballPalb, 0);//Set pal banks and tile indexes
		a_oSpriteManager.HideSprite(sfire[i].iSpriteID);//Hide the sprite
	}

	
}

void MarioManager::CheckFireballCollisions(SpriteManager& a_oSpriteManager, AIManager* a_aoEnemyArray)
{
	for (int y = 0; y < MAX_ENEMIES; y++)//Loop through enemy array
	{
		if (a_aoEnemyArray[y].bSquish)//If the enemy has been squished
		{
			EnemyParticlemanager.InitArray(a_oSpriteManager, 0, 100);//Init score particle
			EnemyParticlemanager.SetEmitterPos(int2fix(a_aoEnemyArray[y].ix) + int2fix(iSpriteWidth >> 2), a_aoEnemyArray[y].iy + int2fix(iSpriteHeight));//et emitter pos to the enemy pos
			a_aoEnemyArray[y].bSquish = false;//Reset squish flag

		}


		if (a_aoEnemyArray[y].bActive == true && a_aoEnemyArray[y].bDead == false)//If enemy is active and not dead
		{
			//The following values are used for AABB collision against the fireballs. They determine the Min and Max positions on each axis.
			int iX2Min = a_aoEnemyArray[y].ix;
			int iX2Max = a_aoEnemyArray[y].ix + iSpriteWidth;
			int iY2Max = fix2int(a_aoEnemyArray[y].iy) + iSpriteWidth;
			int iY2Min = fix2int(a_aoEnemyArray[y].iy);

			for (int i = 0; i < MAX_FIREBALLS; i++)
			{
				if (sfire[i].bActive == true)
				{
					//The following values are used for AABB collision against the fireballs. They determine the Min and Max positions on each axis.
					int iX1Min = fix2int(sfire[i].fx);
					int iX1Max = fix2int(sfire[i].fx) + (iSpriteWidth >>2);
					int iY1Max = fix2int(sfire[i].fy) + (iSpriteWidth >>2);
					int iY1Min = fix2int(sfire[i].fy);

					if (iX1Max < iX2Min || iX1Min > iX2Max)//If the X axis is not colliding
					{

					}
					else if (iY1Max < iY2Min || iY1Min > iY2Max)//If the Y axis is not colliding
					{

					}
					else//If colliding
					{
						a_aoEnemyArray[y].bSquish = true;//Set squish flag for particles
						a_aoEnemyArray[y].bDead = true;//Set death flag for animation
						sfire[i].bActive = false;//Deactivate sprite
						a_oSpriteManager.HideSprite(sfire[i].iSpriteID);//Hide the sprite
						sfire[i].fx = 0;//REset pos
						sfire[i].fy = 0;//Reset pos

					}
				}
			}
		}

	}

}

