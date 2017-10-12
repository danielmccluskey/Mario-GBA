#ifndef _TILES_H_
#define _TILES_H_

#include "main.h"
//Tile Modes
#define TILEMODE1 0x00
#define TILEMODE2 0x01
#define TILEMODE3 0x02

//Background Enabler
#define BGMODE1_ENABLE 0x100
#define BGMODE2_ENABLE 0x200
#define BGMODE3_ENABLE 0x400
#define BGMODE4_ENABLE 0x800

class TileManager
{
public:

	void SetupBG();
	void LoadBGPalette(uint16* a_palette);
	void LoadTileMap(uint16* a_map);
	void LoadImage(uint16* a_image);
	void SetBGControl();
	void ScrollBackGround();
};


#endif // !_TILES_H
