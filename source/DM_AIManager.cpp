#include "DM_AIManager.h"
#include "EnemySprites.h"
#include "Powerups.h"
#include "DM_Enums.h"




enum EnemyTypes
{
	ENEMY_TYPES_GOOMBA,
	ENEMY_TYPES_GREENTURTLE,
	ENEMY_TYPES_REDTURTLE,
	ENEMY_TYPES_MUSHROOM,
	ENEMY_TYPES_FLOWER,
	ENEMY_TYPES_STAR,
	ENEMY_TYPES_GREENSHELL,
	ENEMY_TYPES_REDSHELL
};
enum ENEMYPHYSICS
{
	ENEMY_PHYSICS_WALKSPEED = 32,
	ENEMY_PHYSICS_JUMPHEIGHT = -1024,
	ENEMY_PHYSICS_GRAVITY = 32,
	ENEMY_PHYSICS_PUSHBACK = 8,
	ENEMY_PHYSICS_ALIGNMASK = ~0x7ff,
	ENEMY_PHYSICS_STOPPED = 0,
	ENEMY_PHYSICS_COLLISIONTILE = 0,
	ENEMY_PHYSICS_MAXXVELOCITY = 550,
	ENEMY_PHYSICS_MAXYVELOCITY = 550,
	ENEMY_PHYSICS_QUESTIONRANGEA = 2,
	ENEMY_PHYSICS_QUESTIONRANGEB = 5
};

void AIManager::CreateEnemy(SpriteManager& a_oSpriteManager, AIManager* a_aoEnemyArray, s8 a_iEnemyType, s32 a_ix, s32 a_iy)
{
	for (int i = 0; i < MAX_ENEMIES; i++)//Loop through the enemies array
	{
		if (a_aoEnemyArray[i].bActive == false)//If the enemy is not active, they are allowed to be used to spawn a new enemy
		{
			switch (a_iEnemyType)
			{
			case ENEMY_TYPES_GOOMBA:
				a_aoEnemyArray[i].iStartingFrame = EnemyTileBlock;//For animations, sets the starting location for the animation on the OAM Tiles
				a_aoEnemyArray[i].bDirection = -1;//Makes the sprite move left when it spawns.
				a_aoEnemyArray[i].iSpriteID = a_oSpriteManager.CreateSprite((u16*)EnemySpritesTiles, (u16*)EnemySpritesPal, EnemySpritesTilesLen, EnemySpritesPalLen, EnemyTileBlock, EnemyPalb);//Creates the sprite using the sprite manager and assigns it a sprite id.
				a_aoEnemyArray[i].bAnimate = true;//Allows the sprite to be animated
				break;
			case ENEMY_TYPES_GREENTURTLE:
				a_aoEnemyArray[i].iStartingFrame = EnemyTileBlock + 8;//For animations, sets the starting location for the animation on the OAM Tiles
				a_aoEnemyArray[i].bDirection = -1;//Makes the sprite move left when it spawns.
				a_aoEnemyArray[i].iSpriteID = a_oSpriteManager.CreateSprite((u16*)EnemySpritesTiles, (u16*)EnemySpritesPal, EnemySpritesTilesLen, EnemySpritesPalLen, EnemyTileBlock, EnemyPalb);//Creates the sprite using the sprite manager and assigns it a sprite id.
				a_aoEnemyArray[i].bAnimate = true;//Allows the sprite to be animated

				break;
			case ENEMY_TYPES_REDTURTLE:
				a_aoEnemyArray[i].iStartingFrame = EnemyTileBlock + 16;//For animations, sets the starting location for the animation on the OAM Tiles
				a_aoEnemyArray[i].bDirection = -1;//Makes the sprite move left when it spawns.
				a_aoEnemyArray[i].iSpriteID = a_oSpriteManager.CreateSprite((u16*)EnemySpritesTiles, (u16*)EnemySpritesPal, EnemySpritesTilesLen, EnemySpritesPalLen, EnemyTileBlock, EnemyPalb);//Creates the sprite using the sprite manager and assigns it a sprite id.
				a_aoEnemyArray[i].bAnimate = true;//Allows the sprite to be animated

				break;
			case ENEMY_TYPES_MUSHROOM:
				a_aoEnemyArray[i].iStartingFrame = PowerupsTileBlock;//For animations, sets the starting location for the animation on the OAM Tiles
				a_aoEnemyArray[i].bDirection = 1;//Makes the sprite move right when it spawns.
				a_aoEnemyArray[i].iSpriteID = a_oSpriteManager.CreateSprite((u16*)PowerupsTiles, (u16*)PowerupsPal, PowerupsTilesLen, PowerupsPalLen, PowerupsTileBlock, PowerupsPalb);//Creates the sprite using the sprite manager and assigns it a sprite id.
				a_aoEnemyArray[i].bAnimate = false;//Disallows the sprite to be animated

				break;
			case ENEMY_TYPES_FLOWER:
				a_aoEnemyArray[i].iStartingFrame = PowerupsTileBlock + 4;//For animations, sets the starting location for the animation on the OAM Tiles
				a_aoEnemyArray[i].bDirection = 1;//Makes the sprite move right when it spawns.
				a_aoEnemyArray[i].iSpriteID = a_oSpriteManager.CreateSprite((u16*)PowerupsTiles, (u16*)PowerupsPal, PowerupsTilesLen, PowerupsPalLen, PowerupsTileBlock, PowerupsPalb);//Creates the sprite using the sprite manager and assigns it a sprite id.
				a_aoEnemyArray[i].bAnimate = false;//Disallows the sprite to be animated

				break;

			}

			a_aoEnemyArray[i].iSpriteType = a_iEnemyType;//Sets the sprite type

			a_oSpriteManager.SetFrame(a_aoEnemyArray[i].iStartingFrame, a_aoEnemyArray[i].iSpriteID);//Sets the sprite to its starting frame.
			
			
			a_aoEnemyArray[i].iVelocityY = 0;//Sets its Y velocity to 0;
			a_aoEnemyArray[i].iMaxVelocityY = ENEMY_PHYSICS_MAXYVELOCITY;//Sets the Max velocity.
			a_aoEnemyArray[i].ix = a_ix;//Sets the position of the sprite.
			a_aoEnemyArray[i].iy = a_iy;//Sets the position of the sprite.
			a_aoEnemyArray[i].bActive = true;//Sets the sprite to active.
			a_aoEnemyArray[i].bDead = false;//Sets the sprite to alive.
			a_aoEnemyArray[i].bSquish = false;//Sets the sprite to Not squished

			a_oSpriteManager.MoveSprite(a_aoEnemyArray[i].ix, a_aoEnemyArray[i].iy, a_aoEnemyArray[i].iSpriteID);//Moves the sprite to its starting position.
			break;
		}
		
	}

	
	

	
}

u16 AIManager::CheckSpriteCollision(SpriteManager& a_oSpriteManager, AIManager* a_AIManager, s32 a_ix, s32 a_iy, s32 a_iSpriteWidth, s32 a_iSpriteHeight, bool a_bMarioInvulnerable)
{
	//The following values are used for AABB collision against Mario. They determine the Min and Max positions on each axis.
	s32 iX1Min = fix2int(a_ix);
	s32 iX1Max = fix2int(a_ix) + a_iSpriteWidth;
	s32 iY1Max = fix2int(a_iy) + a_iSpriteHeight;
	s32 iY1Min = fix2int(a_iy);

	
	for (int i = 0; i < MAX_ENEMIES; i++)//Loops through the enemies array
	{
		if (a_AIManager[i].bActive == true && a_AIManager[i].bDead == false)//If the enemy is active and not dead
		{
			if (a_bMarioInvulnerable && !(a_AIManager[i].iSpriteType >= ENEMY_TYPES_MUSHROOM))//Ignore if Mario is invulnverable and the sprite he collides with is not a power up.
			{
				continue;//Move to the next enemy in the array
			}
			//The following values are used for AABB collision against Mario. They determine the Min and Max positions on each axis.
			s32 iX2Min = a_AIManager[i].ix;
			s32 iX2Max = a_AIManager[i].ix + iSpriteWidth;
			s32 iY2Max = fix2int(a_AIManager[i].iy) + iSpriteHeight;
			s32 iY2Min = fix2int(a_AIManager[i].iy);

			

			if (iX1Max < iX2Min || iX1Min > iX2Max)//if the X axis of the sprites are not colliding
			{
				continue;//Move to the next enemy in the array
			}
			if (iY1Max < iY2Min || iY1Min > iY2Max)//if the Y axis of the sprites are not colliding
			{
				continue;//Move to the next enemy in the array
			}
			

			if (iY2Min + 5 > iY1Max && a_AIManager[i].iSpriteType < ENEMY_TYPES_MUSHROOM)//Stomping mechanic (Simple but effective) just tests if the bottom of mario is slightly higher than the bottom of the enemy.
			{
				a_AIManager[i].bDead = true;//Set the enemy to dead to perform death "animation"
				a_AIManager[i].bSquish = true;//Set squish to true for spawning the particles.
				continue;//Move to the next enemy in the array
			}
			else
			{
				if (a_AIManager[i].iSpriteType >= ENEMY_TYPES_MUSHROOM)//if the enemy is a powerup
				{
					a_AIManager[i].bActive = false;//Set the sprite to inactive
					a_oSpriteManager.DeleteSprite(a_AIManager[i].iSpriteID);//Delete the sprite.
				}
				
				return a_AIManager[i].iSpriteType;//Return what sprite mario collided with
			}
		}		
	}
	return 15;//Mario did not collide with an enemy
}

u16 AIManager::LookupTile(u32 x, u32 y, u32 xscroll, u32 yscroll, u16* tilemap, u32 tilemap_w, u32 tilemap_h)
{
	x += xscroll;//Add the Map offset to the X value
	y += yscroll * 8;//Add the map offset to the Y value
	x >>= 3;//Divide by 8 (TileWidth)
	y >>= 3;//Divide by 8 (TileHeight)
	s32 iCollisionIndex = y * tilemap_w + x;//Get the current position in the array.
	return tilemap[iCollisionIndex];//return the tile found at that position
}

void AIManager::UpdateEnemies(SpriteManager& a_oSpriteManager, AIManager* a_aoEnemyArray)
{
	iFrame++;//Increment the Animation frame variable (All sprites animate on the same frame)
	if (iFrame == 2)//All sprites only have 2 animations 
	{
		iFrame = 0;//Reset the frame counter
	}

	for (int i = 0; i < MAX_ENEMIES; i++)//Loop through enemy array
	{
		if (a_aoEnemyArray[i].bActive == true)//If the enemy is active
		{


			s32 iTileX = a_aoEnemyArray[i].ix;//     Took me 4 hours to figure out it wasnt a fixed variable kill me>>fix2int(a_aoEnemyArray[i].ix);
			s32 iTileY = fix2int(a_aoEnemyArray[i].iy);//Get the screen Y  pos


			//Variables for getting collision points of the enemies.
			u8 uiBottomLeft = LookupTile(iTileX, iTileY + iSpriteHeight, iMapOffsetX,
				iMapOffsetY, (u16*)iEnemyBGCollision, 424, 32);
			u8 uiLeft = LookupTile(iTileX - 4, iTileY + 8, iMapOffsetX,
				iMapOffsetY, (u16*)iEnemyBGCollision, 424, 32);
			u8 uiRight = LookupTile(iTileX + iSpriteWidth + 4, iTileY, iMapOffsetX,
				iMapOffsetY, (u16*)iEnemyBGCollision, 424, 32);
			u8 uiBottomRight = LookupTile(iTileX + iSpriteWidth, iTileY + iSpriteHeight, iMapOffsetX,
				iMapOffsetY, (u16*)iEnemyBGCollision, 424, 32);


			if (a_aoEnemyArray[i].iSpriteType != ENEMY_TYPES_FLOWER )//If the enemy is not a flower (Stops the flowers from moving like all other AI)
			{
				if (uiLeft > ENEMY_PHYSICS_COLLISIONTILE)//If the enemy collided with a tile on its left
				{
					a_aoEnemyArray[i].bDirection = 1;//Make the enemy go right
					a_oSpriteManager.SetHFlip(true, a_aoEnemyArray[i].iSpriteID);//Turn the enemy around


				}
				if (uiRight > ENEMY_PHYSICS_COLLISIONTILE)//If the enemy is colliding on the right
				{
					a_aoEnemyArray[i].bDirection = -1;//Make the enemy go left
					a_oSpriteManager.SetHFlip(false, a_aoEnemyArray[i].iSpriteID);//Turn the enemy around
				}
				a_aoEnemyArray[i].ix += a_aoEnemyArray[i].bDirection;//Move the enemy in the direction chosen
			}

			if ((uiBottomLeft > ENEMY_PHYSICS_COLLISIONTILE || uiBottomRight > ENEMY_PHYSICS_COLLISIONTILE ) && !(a_aoEnemyArray[i].bDead))//If the enemy is not dead and is colliding with something below it
			{
				a_aoEnemyArray[i].iy &= ENEMY_PHYSICS_ALIGNMASK;//Align the enemy with the floor (Removes the last bits of the variable making it aligned with the map since fixed will sometimes merge through the floor)
				a_aoEnemyArray[i].bOnGround = true;//The enemy is on the ground
			}
			else//The enemy is in the air or dead
			{
				a_aoEnemyArray[i].bOnGround = false;//Enemy is not on ground
				a_aoEnemyArray[i].iVelocityY = fixAdd(a_aoEnemyArray[i].iVelocityY, ENEMY_PHYSICS_GRAVITY);//Add gravity to the enemies yvelocity
				a_aoEnemyArray[i].iy = fixAdd(a_aoEnemyArray[i].iy, a_aoEnemyArray[i].iVelocityY);//add the yvelocity to the position
			}



			a_oSpriteManager.MoveSprite(a_aoEnemyArray[i].ix, fix2int(a_aoEnemyArray[i].iy), a_aoEnemyArray[i].iSpriteID);//Move the enemy to the calculated positions



			if (a_aoEnemyArray[i].ix <= 0 || fix2int(a_aoEnemyArray[i].iy) >= 160)//If the enemy is off screen.
			{
				a_aoEnemyArray[i].bActive = false;//Deactive the enemy
				a_oSpriteManager.DeleteSprite(a_aoEnemyArray[i].iSpriteID);//Delete the enemy sprite
				a_aoEnemyArray[i].bDead = false;//Reset enemy dead status
				a_aoEnemyArray[i].bDeleteParticles = true;//Flag for mario class to delete the particles for this enemy
			}
			
			if (a_aoEnemyArray[i].bAnimate)//If the enemy is allowed to animate
			{
				a_oSpriteManager.SetFrame(a_aoEnemyArray[i].iStartingFrame + (iFrame*iFrameSize), a_aoEnemyArray[i].iSpriteID);//Animate the sprite

			}
		}
	}
}

void AIManager::ScrollEnemies(SpriteManager& a_oSpriteManager, AIManager* a_aoEnemyArray, s32 a_ix)
{
	for (int i = 0; i < MAX_ENEMIES; i++)//Loop through the enemy array
	{
		if (a_aoEnemyArray[i].bActive == true)//If the enemy is active
		{
			a_aoEnemyArray[i].ix -= a_ix;//Subtract the scroll from the position
			a_oSpriteManager.MoveSprite(a_aoEnemyArray[i].ix, a_aoEnemyArray[i].iy, a_aoEnemyArray[i].iSpriteID);//Move the sprite to that posiiton
			if (a_aoEnemyArray[i].ix <= (-iSpriteWidth))//If the enemy is now offscreen
			{
				a_aoEnemyArray[i].bActive = false;//Deactivate the enemy
				a_oSpriteManager.DeleteSprite(a_aoEnemyArray[i].iSpriteID);//Delete the enemy
				a_aoEnemyArray[i].bDead = false;//Reset enemy dead status
				a_aoEnemyArray[i].bDeleteParticles = true;//Flag for mario class to delete the particles for this enemy
			}
		}
	}

}

void AIManager::DeleteEnemies(SpriteManager& a_oSpriteManager, AIManager* a_aoEnemyArray)
{
	for (int i = 0; i < MAX_ENEMIES; i++)//Loop through enemy array
	{
		if (a_aoEnemyArray[i].bActive == true)//if the enemy is active
		{
			a_aoEnemyArray[i].bActive = false;//Deactivate the enemy
			a_oSpriteManager.DeleteSprite(a_aoEnemyArray[i].iSpriteID);//Delete the enemy
			a_aoEnemyArray[i].bDead = false;//Reset enemy dead status
			a_aoEnemyArray[i].bDeleteParticles = true;//Flag for mario class to delete the particles for this enemy
		}
	}
}