#include "../include/Tiles.h"
#include "../include/main.h"

void vsync()
{
	while (REG_VCOUNT >= 160);
	while (REG_VCOUNT < 160);
}
int main()
{
	//set GBA rendering to MODE 3 Bitmap rendering
	//*(unsigned int*)0x04000000 = 0x0403;
	REG_DISPLAYCONTROL = TILEMODE1 | BGMODE1_ENABLE;
	//REG_DISPLAYCONTROL = VIDEOMODE_3 | BGMODE_2;
	TileManager Tilemanager;
	Tilemanager.SetupBG();

	while (1)
	{
		vsync();
	}//loop forever

	return 0;
}