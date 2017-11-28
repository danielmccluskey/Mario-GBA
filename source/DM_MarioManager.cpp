#include "DM_MarioManager.h"
#include "Mario.h"
#include "gba.h"
#include "BG_Collisions.h"
#include "gba_math.h"



void MarioManager::CreateMario(SpriteManager& a_SpriteManager)
{
	iSpriteID = a_SpriteManager.SpriteIndex;
	a_SpriteManager.CreateSprite((u16*)marTiles, (u16*)marPal, 512, 24, 4);
	a_SpriteManager.MoveSprite(0, 120, 0);

	iVelocityX = 0;
	iVelocityY = 0;

	iMaxVelocityX = 550;
	iMaxVelocityY = 500;

	ix = 0;
	iy = 120;
}

void MarioManager::MoveMario(s32 a_ix, s32 a_iy, SpriteManager& a_SpriteManager)
{
	if ((iVelocityX < iMaxVelocityX) && (iVelocityX > -iMaxVelocityX))
	{
		iVelocityX = fixAdd(a_ix, iVelocityX);
	}
	if (iVelocityY < iMaxVelocityY)
	{
		iVelocityY = fixAdd(iVelocityY, 10);
		iVelocityY = fixAdd(a_iy, iVelocityY);
	}
}
u16 tile_lookup(u32 x, u32 y, u32 xscroll, u32 yscroll,
	const unsigned short* tilemap, u32 tilemap_w, u32 tilemap_h) {

	/* adjust for the scroll */
	x += xscroll;
	y += yscroll;

	/* convert from screen coordinates to tile coordinates */
	x >>= 3;
	y >>= 3;

	//y = 44 + y;

	/* account for wraparound */
	while (x >= tilemap_w) {
		x -= tilemap_w;
	}
	while (y >= tilemap_h) {
		y -= tilemap_h;
	}
	while (x < 0) {
		x += tilemap_w;
	}
	while (y < 0) {
		y += tilemap_h;
	}

	/* lookup this tile from the map */
	s32 index = y * tilemap_w + x;

	/* return the tile */
	return tilemap[index];
}

void MarioManager::UpdateMario(SpriteManager& a_SpriteManager)
{

	last_ix = ix;
	last_iy = iy;
	
	

	

	//u32 ax = (fix2inter(ix)) >> 3;
	//u32 ay = (fix2inter(iy)) >> 3;


	u16 TopLeft = tile_lookup((ix >> 8), (iy >> 8), iMapOffset,
	44*8, bgCollision, 424, 64);
	u16 BottomLeft = tile_lookup((ix >> 8), (iy >> 8)+16, iMapOffset,
		44*8, bgCollision, 424, 64);
	u16 TopRight = tile_lookup((ix >> 8)+16, (iy >> 8), iMapOffset,
		44*8, bgCollision, 424, 64);
	u16 BottomRight = tile_lookup((ix >> 8)+16, (iy >> 8)+16, iMapOffset,
		44*8, bgCollision, 424, 64);
	
	
	/*if (iVelocityX > 0 && ((TopRight > 0 || BottomRight > 0)))
	{
		iVelocityX = -300;
	}*/

	a_SpriteManager.SetFrame(0, iSpriteID);

	if (iVelocityX != 0)
	{		
		iFrame += 4;
		if (iFrame >= 16)
		{
			iFrame = 0;
		}
		
		
	}

	ix = fixAdd(ix, iVelocityX);
	
	if (iVelocityY < 0 && ((TopLeft > 0 || TopRight > 0)))
	{
		iy = last_iy;
		iVelocityY = 8;
	}

	if (BottomLeft > 0 || BottomRight > 0)
	{
		//iVelocityY = -31;
		iy &= ~0x7ff;
		bOnGround = true;
		if (bJump)
		{
			iFrame = 4;
			iVelocityY = -1024;
			iy = fixAdd(iy, iVelocityY);			
		}		
	}
	else
	{
		bOnGround = false;
		iVelocityY = fixAdd(iVelocityY, 32);
		iy = fixAdd(iy, iVelocityY);		
	}
	bJump = false;
	
	a_SpriteManager.SetFrame(iFrame, iSpriteID);
	a_SpriteManager.MoveSprite(fix2int(ix), fix2int(iy), iSpriteID);
	if (iVelocityX >= 32)
	{
		iVelocityX = fixSub(iVelocityX, 32);
	}
	else if (iVelocityX <= -32)
	{
		iVelocityX = fixAdd(iVelocityX, 32);
	}
	else
	{
		iVelocityX = 0;
		iFrame = 0;
	}
}

