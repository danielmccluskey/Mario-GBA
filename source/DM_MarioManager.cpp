#include "DM_MarioManager.h"
#include "Mario.h"
#include "gba.h"
#include "BG_Collisions.h"
//#include "gba_math.h"

fixed	fixAdder(fixed a_fa, fixed a_fb)
{
	return a_fa + a_fb;
}
s32		fix2inter(fixed a_f)
{
	return a_f / FIX_SCALE;
}

//subtract two fixed point values
fixed	fixSubber(fixed a_fa, fixed a_fb)
{
	return a_fa - a_fb;
}

fixed	int2fixer(s32 a_i)
{
	return a_i << FIX_SHIFT;
}

void MarioManager::CreateMario(SpriteManager& a_SpriteManager)
{
	iSpriteID = a_SpriteManager.SpriteIndex;
	a_SpriteManager.CreateSprite((u16*)marTiles, (u16*)marPal, 256, 24, 4);
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
		iVelocityX = fixAdder(a_ix, iVelocityX);
	}
	if (iVelocityY < iMaxVelocityY)
	{
		iVelocityY = fixAdder(iVelocityY, 10);
		iVelocityY = fixAdder(a_iy, iVelocityY);
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

	y = 44 + y;

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
	
	ix = fixAdder(ix, iVelocityX);

	

	u32 ax = (fix2inter(ix)) >> 3;
	u32 ay = (fix2inter(iy)) >> 3;


	u16 tile = tile_lookup((ix >> 8), (iy >> 8), iMapOffset,
	0, bgCollision, 424, 64);
	iVelocityY = fixAdder(iVelocityY, 32);
	if (tile > 0)
	{
		
		iVelocityY = 0;
		iy &= ~0x7ff;

		
	}
	
	
	
	iy = fixAdder(iy, iVelocityY);
	/*if (iVelocityY < 0 && ((TopLeft > 0 || TopRight > 0)))
	{
		iy = last_iy;
		iVelocityY = 8;
	}*/

	//if (BottomLeft > 0 || BottomRight > 0)
	//{
	//	//iVelocityY = -31;
	//	iy = last_iy;
	//	bOnGround = true;
	//	if (bJump)
	//	{
	//		iVelocityY = -1024;
	//	}
	//}

	//else
	//{
	//	bOnGround = false;
	//	iVelocityY = fixAdder(iVelocityY, 32);
	//	iy = fixAdder(iy, iVelocityY);
	//	
	//}
	//bJump = false;

	//iVelocityY = fixAdder(ix, int2fixer(test));

	
	a_SpriteManager.MoveSprite(fix2inter(ix), fix2inter(iy), iSpriteID);
	if (iVelocityX >= 32)
	{
		iVelocityX = fixSubber(iVelocityX, 32);
	}
	else if (iVelocityX <= -30)
	{
		iVelocityX = fixAdder(iVelocityX, 32);
	}
	else
	{
		iVelocityX = 0;
	}
}