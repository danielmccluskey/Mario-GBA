#include "../include/main.h"
#include "../include/Tiles.h"
#include "../include/Background.h"
#include "../include/TileData.h"


//Background Controllers
#define BGMODE0_CONTROL ((v_uint16*)0x4000008);
#define BGMODE1_CONTROL ((v_uint16*)0x400000a);
#define BGMODE2_CONTROL ((v_uint16*)0x400000c);
#define BGMODE3_CONTROL ((v_uint16*)0x400000e);

//Background Scrollers
v_uint16* BG0SC_X = (v_uint16*)0x4000010;
v_uint16* BG0SC_Y = (v_uint16*)0x4000012;
v_uint16* BG1SC_X = (v_uint16*)0x4000014;
v_uint16* BG1SC_Y = (v_uint16*)0x4000016;
v_uint16* BG2SC_X = (v_uint16*)0x4000018;
v_uint16* BG2SC_Y = (v_uint16*)0x400001a;
v_uint16* BG3SC_X = (v_uint16*)0x400001c;
v_uint16* BG3SC_Y = (v_uint16*)0x400001e;

int32 scroll_x = 0;
int32 scroll_y = 0;

v_uint16* bgcontroller = BGMODE0_CONTROL;
int32 MAPWIDTH = 32;
int32 MAPHEIGHT = 32;

v_uint16* backgroundPalette = (v_uint16*)0x5000000;
uint16 backgroundData[256];
v_uint16* Mapper;


v_uint16* Get_CharacterBlock(uint32 a_block)
{
	return (v_uint16*)(0x6000000 + (a_block * 0x4000));
}
v_uint16* Get_ScreenBlock(uint32 a_block)
{
	return (v_uint16*)(0x6000000 + (a_block * 0x800));
}

void TileManager::SetupBG()
{

	LoadBGPalette((uint16*)background_palette);
	LoadImage((uint16*)background_data);
	SetBGControl();
	LoadTileMap(newmap);

}

void TileManager::LoadBGPalette(uint16* a_palette)
{
	for (int32 i = 0; i < 256; i++)
	{
		backgroundPalette[i] = background_palette[i];
	}
}

void TileManager::LoadTileMap(uint16* a_map)
{
	Mapper = Get_ScreenBlock(16);
	for (int i = 0; i < (32 * 32); i++)
	{
		Mapper[i] = newmap[i];
	}

}
void TileManager::LoadImage(uint16* a_image)
{
	Mapper = Get_CharacterBlock(0);
	uint16* image = (uint16*)background_data;
	for (int i = 0; i < ((background_width * background_height) / 2); i++) {
		Mapper[i] = image[i];
	}
}

void TileManager::SetBGControl()
{
	*bgcontroller = 0 |   
		(0 << 2) |  
		(0 << 6) |   
		(1 << 7) |  
		(16 << 8) |   
		(1 << 13) |   
		(0 << 14);
}

void TileManager::ScrollBackGround()
{
	/*if (keyDown(KEYBINDS::UP))
	{
		scroll_y++;
	}
	if (keyDown(KEYBINDS::DOWN))
	{
		scroll_y--;
	}
	if (keyDown(KEYBINDS::LEFT))
	{
		scroll_x--;
	}
	if (keyDown(KEYBINDS::RIGHT))
	{
		scroll_x++;
	}

	*BG0SC_X = scroll_x;
	*BG0SC_Y = scroll_y;*/
}