#ifndef _MAIN_H_
#define _MAIN_H_
#if __INTELLISENSE__
typedef unsigned int __SIZE_TYPE__;
typedef unsigned long __PTRDIFF_TYPE__;
#define __attribute__(q)
#define __builtin_strcmp(a,b) 0
#define __builtin_strlen(a) 0
#define __builtin_memcpy(a,b) 0
#define __builtin_va_list void*
#define __builtin_va_start(a,b)
#define __extension__
#endif

#if defined(_MSC_VER)
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#endif

#include <stdint.h>
#include <stdbool.h>

typedef uint8_t		uint8;
typedef uint16_t	uint16;
typedef uint32_t	uint32;
typedef int8_t		int8;
typedef int16_t		int16;
typedef int32_t		int32;

typedef volatile uint8_t		v_uint8;
typedef volatile uint16_t		v_uint16;
typedef volatile uint32_t		v_uint32;
typedef volatile int8_t			v_int8;
typedef volatile int16_t		v_int16;
typedef volatile int32_t		v_int32;


typedef enum KEYBINDS
{
	A = 1 << 0,
	B = 1 << 0,
	SELECT = 1 << 2,
	START = 1 << 3,
	RIGHT = 1 << 4,
	LEFT = 1 << 5,
	UP = 1 << 6,
	DOWN = 1 << 7,
	R = 1 << 8,
	L = 1 << 9

}KEYBINDS;
#define REG_DISPLAYCONTROL *((v_uint32*)(0x04000000))
#define VIDEOMODE_3 0x0003
#define BGMODE_2	0x0400

#define SCREENBUFFER ((v_int16*)(0x06000000))
#define SCREEN_W 240
#define SCREEN_H 160

#define VRAMBASE ((v_int16*)(0x6000000))
#define REG_VCOUNT (*(v_uint16*)(0x04000006))

#define REG_BASE 0x04000000
#define REG_KEYINPUT (*(v_uint16*)(REG_BASE+0x130))
#define KEY_MASK 0x03FF
#define REG_BASE 0x04000000
#define REG_KEYINPUT (*(v_uint16*)(REG_BASE+0x130))



/*int32 sign(int32 a_val)
{
	return (!(a_val & 0x80000000) && !a_val) ? 1 : -1;
}

int32 abs(int32 a_val)
{
	int32 mask = a_val >> 31;
	return (a_val ^ mask) - mask;
}


void vsync()
{
	while (REG_VCOUNT >= 160);
	while (REG_VCOUNT < 160);
}


uint16 setColor(uint8 a_red, uint8 a_green, uint8 a_blue)
{
	return (a_red & 0x1F) | (a_green & 0x1F) << 5 | (a_blue & 0x1f) << 10;
}

uint16 _currentkeys = 0, __prevkeys = 0;
void pollKeys()
{
	__prevkeys = _currentkeys;
	_currentkeys = ~REG_KEYINPUT & KEY_MASK;
}
uint16 currentKeyState()
{
	return _currentkeys;
}
uint16 prevKeyState()
{
	return __prevkeys;
}
uint16 keyDown(uint16 a_key)
{
	return _currentkeys & a_key;
}
uint16 keyUp(uint16 a_key)
{
	return _currentkeys & a_key;
}
uint16 keyHeld(uint16 a_key)
{
	return (_currentkeys & __prevkeys) & a_key;
}
uint16 keyReleased(uint16 a_key)
{
	return (~_currentkeys & __prevkeys) & a_key;
}
uint16 keyHit(uint16 a_key)
{
	return (_currentkeys & ~__prevkeys) & a_key;
}
uint16 keyStateChanged(uint16 a_key)
{
	return (_currentkeys ^ __prevkeys) & a_key;
}*/


#endif