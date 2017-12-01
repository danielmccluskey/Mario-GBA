/*

	Header file for libgba bios sound functions

	Copyright (C) 2003-2005 by Dave Murphy.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Library General Public
	License as published by the Free Software Foundation; either
	version 2 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Library General Public License for more details.

	You should have received a copy of the GNU Library General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
	USA.

	Please report all bugs and problems through the bug tracker at
	"http://sourceforge.net/tracker/?group_id=114505&atid=668551".

*/

//---------------------------------------------------------------------------------
#ifndef	_gba_sound_h_
#define	_gba_sound_h_
//---------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
//---------------------------------------------------------------------------------

#include "gba.h"

typedef struct WaveData{
	u16 type;
	u16 stat;
	u32 freq;
	u32 loop;
	u32 size;
	s8 data[1];
} WaveData;

typedef struct SoundChannel {
	u8 Status;
	u8 reserved1;
	u8 RightVol;
	u8 LeftVol;
	u8 Attack;
	u8 Decay;
	u8 Sustain;
	u8 Release;
	u8 reserved2[24];
	u32 fr;
	WaveData *wp;
	u32 reserved3[6];
} SoundChannel;

#define PCM_DMA_BUF 1584
#define MAX_DIRECTSOUND_CHANNELS 12

typedef struct SoundArea {
	u32 ident;
	vu8 DmaCount;
	u8 reverb;
	u8 maxchn;
	u8 masvol;
	u8 freq;
	u8 mode;
	u8 r2[6];
	u32 r3[16];
	SoundChannel vchn[MAX_DIRECTSOUND_CHANNELS];
	s8 pcmbuf[PCM_DMA_BUF*2];
} SoundArea;

/*---------------------------------------------------------------------------------
	Control Registers
---------------------------------------------------------------------------------*/

#define SND1_R_ENABLE	(1<<8)		// Enable left & right speakers for each sound channel
#define SND1_L_ENABLE	(1<<12)
#define SND2_R_ENABLE	(1<<9)
#define SND2_L_ENABLE	(1<<13)
#define SND3_R_ENABLE	(1<<10)
#define SND3_L_ENABLE	(1<<14)
#define SND4_R_ENABLE	(1<<11)
#define SND4_L_ENABLE	(1<<15)

#define SNDA_VOL_50     (0<<2)
#define SNDA_VOL_100    (1<<2)
#define SNDB_VOL_50     (0<<3)
#define SNDB_VOL_100    (1<<3)
#define SNDA_R_ENABLE   (1<<8)
#define SNDA_L_ENABLE   (1<<9)
#define SNDA_RESET_FIFO (1<<11)
#define SNDB_R_ENABLE   (1<<12)
#define SNDB_L_ENABLE   (1<<13)
#define SNDB_RESET_FIFO (1<<15)


#define	REG_SOUNDCNT_L	(*((u16 volatile *) (REG_BASE + 0x080)))
#define	REG_SOUNDCNT_H	(*((u16 volatile *) (REG_BASE + 0x082)))
#define	REG_SOUNDCNT_X	(*((u16 volatile *) (REG_BASE + 0x084)))


#define	REG_SOUND1CNT_L	(*((u16 volatile *) (REG_BASE + 0x060)))
#define	REG_SOUND1CNT_H	(*((u16 volatile *) (REG_BASE + 0x062)))
#define	REG_SOUND1CNT_X	(*((u16 volatile *) (REG_BASE + 0x064)))

#define	REG_SOUND2CNT_L	(*((u16 volatile *) (REG_BASE + 0x068)))
#define	REG_SOUND2CNT_H	(*((u16 volatile *) (REG_BASE + 0x06C)))

#define	REG_SOUND3CNT_L	(*((u16 volatile *) (REG_BASE + 0x070)))
#define	REG_SOUND3CNT_H	(*((u16 volatile *) (REG_BASE + 0x072)))
#define	REG_SOUND3CNT_X	(*((u16 volatile *) (REG_BASE + 0x074)))

#define	REG_SOUND4CNT_L	(*((u16 volatile *) (REG_BASE + 0x078)))
#define	REG_SOUND4CNT_H	(*((u16 volatile *) (REG_BASE + 0x07C)))

#define	REG_SOUNDBIAS	(*((u16 volatile *) (REG_BASE + 0x088)))

#define	REG_FIFO_A		(*((u32 volatile *) (REG_BASE + 0x0A0)))
#define	REG_FIFO_B		(*((u32 volatile *) (REG_BASE + 0x0A4)))

#define	WAVE_RAM	((u16 volatile *)	(REG_BASE + 0x090))

#define	SOUND3_STEP32		(0<<5)	// Use two banks of 32 steps each
#define SOUND3_STEP64		(1<<5)	// Use one bank of 64 steps
#define SOUND3_SETBANK(n)	(n<<6)	// Bank to play 0 or 1 (non set bank is written to)
#define SOUND3_PLAY			(1<<7)	// Output sound
#define SOUND3_STOP			(0<<7)	// Stop sound output

//---------------------------------------------------------------------------------
// pin8 compatible sound macros 
//---------------------------------------------------------------------------------

/*---------------------------------------------------------------------------------
	DMG Sound Control (0x04000080)
fedcba9876543210
|||||||| ||| |||
|||||||| ||| +++- DMG left volume
|||||||| +++----- DMG right volume
|||||||+--------- Enable sqr1 on left
||||||+---------- Enable sqr2 on left
|||||+----------- Enable triangle on left
||||+------------ Enable noise on left
|||+------------- Enable sqr1 on right
||+-------------- Enable sqr2 on right
|+--------------- Enable triangle on right
+---------------- 
---------------------------------------------------------------------------------*/
#define DMGSNDCTRL         (*(volatile u16 *)0x04000080)
#define DMGSNDCTRL_LVOL(x) (x)
#define DMGSNDCTRL_RVOL(x) ((x) << 4)
#define DMGSNDCTRL_LSQR1   0x0100
#define DMGSNDCTRL_LSQR2   0x0200
#define DMGSNDCTRL_LTRI    0x0400
#define DMGSNTCTRL_LNOISE  0x0800
#define DMGSNDCTRL_RSQR1   0x1000
#define DMGSNDCTRL_RSQR2   0x2000
#define DMGSNDCTRL_RTRI    0x4000
#define DMGSNDCTRL_RNOISE  0x8000

/*---------------------------------------------------------------------------------
	Direct Sound Control (0x04000082)
-----------------------------------------------------------------------------------
fedcba9876543210
||||||||    ||||
||||||||    ||++- DMG sound output volume
||||||||    ||    (00: 25%; 01: 50%; 10: 100%)
||||||||    |+--- DSound A output volume (0: 50%; 1: 100%)
||||||||    +---- DSound B output volume (0: 50%; 1: 100%)
|||||||+--------- Enable DSound A on right
||||||+---------- Enable DSound A on left
|||||+----------- DSound A sample timer (0 or 1)
||||+------------ DSound A FIFO reset
|||+------------- Enable DSound B on right
||+-------------- Enable DSound B on left
|+--------------- DSound B sample timer (0 or 1)
+---------------- DSound B FIFO reset
---------------------------------------------------------------------------------*/
#define DSOUNDCTRL           (*(volatile u16 *)0x04000082)
#define DSOUNDCTRL_DMG25     0x0000
#define DSOUNDCTRL_DMG50     0x0001
#define DSOUNDCTRL_DMG100    0x0002
#define DSOUNDCTRL_A50       0x0000
#define DSOUNDCTRL_A100      0x0004
#define DSOUNDCTRL_B50       0x0000
#define DSOUNDCTRL_B100      0x0008
#define DSOUNDCTRL_AR        0x0100
#define DSOUNDCTRL_AL        0x0200
#define DSOUNDCTRL_ATIMER(x) ((x) << 10)
#define DSOUNDCTRL_ARESET    0x0800
#define DSOUNDCTRL_BR        0x1000
#define DSOUNDCTRL_BL        0x2000
#define DSOUNDCTRL_BTIMER(x) ((x) << 14)
#define DSOUNDCTRL_BRESET    0x8000

/*---------------------------------------------------------------------------------
 Sound Status (0x04000084)
-----------------------------------------------------------------------------------
Note that unlike NES's $4014, bits 0 to 3 of this register are
read-only.  They do not enable sound.

fedcba9876543210
        |   ||||
        |   |||+- Square 1 playing
        |   ||+-- Square 2 playing
        |   |+--- Triangle playing
        |   +---- Noise playing
        +-------- 0: save 10% battery power by turning off ALL sound;
                  1: play sound
---------------------------------------------------------------------------------*/
#define SNDSTAT        (*(volatile u16*)0x04000084)
#define SNDSTAT_SQR1   0x0001
#define SNDSTAT_SQR2   0x0002
#define SNDSTAT_TRI    0x0004
#define SNDSTAT_NOISE  0x0008
#define SNDSTAT_ENABLE 0x0080

/*---------------------------------------------------------------------------------
	Sound Bias: will not be documented.
-----------------------------------------------------------------------------------
fedcba9876543210
||    ||||||||||
||    ++++++++++- PWM bias
++--------------- Amplitude resolution
                  00: 9-bit at 32768 Hz
                  01: 8-bit at 65536 Hz (most common)
                  10: 7-bit at 131072 Hz
                  11: 6-bit at 262144 Hz

Do NOT use SNDBIAS directly.  To set the resolution, use
  SETSNDRES(1);
---------------------------------------------------------------------------------*/
#define SNDBIAS      (*(volatile u16 *)0x04000088)
#define SETSNDRES(x) SNDBIAS = (SNDBIAS & 0x3fff) | (x << 14)

#define DSOUND_FIFOA (*(volatile u32 *)0x040000a0)
#define DSOUND_FIFOB (*(volatile u32 *)0x040000a4)


/*---------------------------------------------------------------------------------
 Square 1 Sweep Register
---------------------------------------------------------------------------------

fedcba9876543210
         |||||||
         ||||+++- Sweep shifts (1 fastest; 7 slowest)
         |||+---- 0: Sweep up; 1: Sweep down

Write 0x0040 into this register to disable sweep.
---------------------------------------------------------------------------------*/
#define SQR1SWEEP   (*(volatile u16 *)0x04000060)
#define SQR1SWEEP_OFF 0x0008


/*---------------------------------------------------------------------------------
   Square 1 Control Register
   Square 2 Control Register
-----------------------------------------------------------------------------------
fedcba9876543210
||||||||||||||||
||||||||||++++++- Sound length (1 longest; 63: shortest)
||||||||++------- Duty cycle (00: 1/8; 01: 1/4; 10: 1/2; 11: 3/4)
|||||+++--------- Envelope step time (0: off; 1 fastest; 7 slowest)
||||+------------ Envelope direction (0: decrease; 1: increase)
++++------------- Volume
---------------------------------------------------------------------------------*/
#define SQR1CTRL    (*(volatile u16 *)0x04000062)
#define SQR2CTRL    (*(volatile u16 *)0x04000068)
#define SQR_DUTY(n) ((n) << 6)
#define SQR_VOL(n)  ((n) << 12)

/*---------------------------------------------------------------------------------
   Square 1 Frequency
   Square 2 Frequency
   Triangle Channel Frequency (0x04000074)
-----------------------------------------------------------------------------------
fedcba9876543210
||   |||||||||||
||   +++++++++++- frequency (131072 Hz/(2048-x)) (halved for tri channel)
|+--------------- 0: hold note; 1: use length
+---------------- 1: Reset channel
---------------------------------------------------------------------------------*/
#define SQR1FREQ      (*(volatile u16 *)0x04000064)
#define SQR2FREQ      (*(volatile u16 *)0x0400006c)
#define TRIFREQ       (*(volatile u16 *)0x04000074)
#define TRIFREQ_HOLD  0x0000
#define TRIFREQ_TIMED 0x4000
#define TRIFREQ_RESET 0x8000


/*---------------------------------------------------------------------------------
	Triangle Channel Control Register
-----------------------------------------------------------------------------------
fedcba9876543210
        |||
        ||+------ Bank mode (0: 2 banks of 32; 1: 1 bank of 64)
        |+------- Play this bank (and write other bank)
        +-------- Enable triangle channel
---------------------------------------------------------------------------------*/
#define TRICTRL         (*(volatile u16 *)0x04000070)
#define TRICTRL_2X32    0x0000
#define TRICTRL_1X64    0x0020
#define TRICTRL_BANK(x) ((x) << 6)
#define TRICTRL_ENABLE  0x0080

/*---------------------------------------------------------------------------------
	Triangle Channel Length/Volume (0x04000072)
-----------------------------------------------------------------------------------

fedcba9876543210
|||     ||||||||
|||     ++++++++- Length ((256-x)/256 seconds)
+++-------------- Volume (000: mute; 001: 100%; 010: 50%;
                          011: 25%; 100: 75%)
---------------------------------------------------------------------------------*/
#define TRILENVOL        (*(volatile u16 *)0x04000072)
#define TRILENVOL_LEN(x) (256 - (x))
#define TRILENVOL_MUTE   0x0000
#define TRILENVOL_25     0x6000
#define TRILENVOL_50     0x4000
#define TRILENVOL_75     0x8000
#define TRILENVOL_100    0x2000

#define TRIWAVERAM ((volatile u32 *)0x04000090)

//---------------------------------------------------------------------------------
//	Bios sound functions
//---------------------------------------------------------------------------------


void SoundDriverInit(SoundArea *sa);
void SoundDriverMode(u32 mode);

u32  MidiKey2Freq(WaveData *wa, u8 mk, u8 fp);

// === SOUND REGISTERS ===
// sound regs, partially following pin8gba's nomenclature

//! \name Channel 1: Square wave with sweep
//\{
#define REG_SND1SWEEP		*(vu16*)(REG_BASE+0x0060)	//!< Channel 1 Sweep
#define REG_SND1CNT			*(vu16*)(REG_BASE+0x0062)	//!< Channel 1 Control
#define REG_SND1FREQ		*(vu16*)(REG_BASE+0x0064)	//!< Channel 1 frequency
//\}

//! \name Channel 2: Simple square wave
//\{
#define REG_SND2CNT			*(vu16*)(REG_BASE+0x0068)	//!< Channel 2 control
#define REG_SND2FREQ		*(vu16*)(REG_BASE+0x006C)	//!< Channel 2 frequency
//\}

//! \name Channel 3: Wave player
//\{
#define REG_SND3SEL			*(vu16*)(REG_BASE+0x0070)	//!< Channel 3 wave select
#define REG_SND3CNT			*(vu16*)(REG_BASE+0x0072)	//!< Channel 3 control
#define REG_SND3FREQ		*(vu16*)(REG_BASE+0x0074)	//!< Channel 3 frequency
//\}

//! \name Channel 4: Noise generator
//\{
#define REG_SND4CNT			*(vu16*)(REG_BASE+0x0078)	//!< Channel 4 control
#define REG_SND4FREQ		*(vu16*)(REG_BASE+0x007C)	//!< Channel 4 frequency
//\}

//! \name Sound control
//\{
#define REG_SNDCNT			*(vu32*)(REG_BASE+0x0080)	//!< Main sound control
#define REG_SNDDMGCNT		*(vu16*)(REG_BASE+0x0080)	//!< DMG channel control
#define REG_SNDDSCNT		*(vu16*)(REG_BASE+0x0082)	//!< Direct Sound control
#define REG_SNDSTAT			*(vu16*)(REG_BASE+0x0084)	//!< Sound status
#define REG_SNDBIAS			*(vu16*)(REG_BASE+0x0088)	//!< Sound bias
//\}

//! \name Sound buffers
//\{
#define REG_WAVE_RAM		 (vu32*)(REG_BASE+0x0090)	//!< Channel 3 wave buffer

#define REG_WAVE_RAM0		*(vu32*)(REG_BASE+0x0090)
#define REG_WAVE_RAM1		*(vu32*)(REG_BASE+0x0094)
#define REG_WAVE_RAM2		*(vu32*)(REG_BASE+0x0098)
#define REG_WAVE_RAM3		*(vu32*)(REG_BASE+0x009C)

#define REG_FIFO_A			*(vu32*)(REG_BASE+0x00A0)	//!< DSound A FIFO
#define REG_FIFO_B			*(vu32*)(REG_BASE+0x00A4)	//!< DSound B FIFO
//\}


// --- REG_SND1SWEEP ---------------------------------------------------

/*!	\defgroup grpAudioSSW	Tone Generator, Sweep Flags
\ingroup grpMemBits
\brief	Bits for REG_SND1SWEEP (aka REG_SOUND1CNT_L)
*/
/*!	\{	*/

#define SSW_INC			 0		//!< Increasing sweep rate
#define SSW_DEC			0x0008	//!< Decreasing sweep rate
#define SSW_OFF         0x0008	//!< Disable sweep altogether

#define SSW_SHIFT_MASK	0x0007
#define SSW_SHIFT_SHIFT		 0
#define SSW_SHIFT(n)	((n)<<SSW_SHIFT_SHIFT)

#define SSW_TIME_MASK	0x0070
#define SSW_TIME_SHIFT		 4
#define SSW_TIME(n)		((n)<<SSW_TIME_SHIFT)


#define SSW_BUILD(shift, dir, time) \
	( (((time)&7)<<4) | ((dir)<<3) | ((shift)&7) )

/*!	\}	/defgroup	*/

// --- REG_SND1CNT, REG_SND2CNT, REG_SND4CNT ---------------------------

/*!	\defgroup grpAudioSSQR	Tone Generator, Square Flags
\ingroup grpMemBits
\brief	Bits for REG_SND{1,2,4}CNT
(aka REG_SOUND1CNT_H, REG_SOUND2CNT_L, REG_SOUND4CNT_L, respectively)
*/
/*!	\{	*/

#define SSQR_DUTY1_8		 0	//!< 12.5% duty cycle (#-------)
#define SSQR_DUTY1_4	0x0040	//!< 25% duty cycle (##------)
#define SSQR_DUTY1_2	0x0080	//!< 50% duty cycle (####----)
#define SSQR_DUTY3_4	0x00C0	//!< 75% duty cycle (######--) Equivalent to 25%
#define SSQR_INC			 0	//!< Increasing volume
#define SSQR_DEC		0x0800	//!< Decreasing volume

#define SSQR_LEN_MASK	0x003F
#define SSQR_LEN_SHIFT		 0
#define SSQR_LEN(n)		((n)<<SSQR_LEN_SHIFT)

#define SSQR_DUTY_MASK	0x00C0
#define SSQR_DUTY_SHIFT		 6
#define SSQR_DUTY(n)	((n)<<SSQR_DUTY_SHIFT)

#define SSQR_TIME_MASK	0x0700
#define SSQR_TIME_SHIFT		 8
#define SSQR_TIME(n)	((n)<<SSQR_TIME_SHIFT)

#define SSQR_IVOL_MASK	0xF000
#define SSQR_IVOL_SHIFT		12
#define SSQR_IVOL(n)	((n)<<SSQR_IVOL_SHIFT)


#define SSQR_ENV_BUILD(ivol, dir, time)				\
	(  ((ivol)<<12) | ((dir)<<11) | (((time)&7)<<8) )

#define SSQR_BUILD(_ivol, dir, step, duty, len)		\
	( SSQR_ENV_BUILD(ivol,dir,step) | (((duty)&3)<<6) | ((len)&63) )


/*!	\}	/defgroup	*/

// --- REG_SND1FREQ, REG_SND2FREQ, REG_SND3FREQ ------------------------

/*!	\defgroup grpAudioSFREQ	Tone Generator, Frequency Flags
\ingroup grpMemBits
\brief	Bits for REG_SND{1-3}FREQ
(aka REG_SOUND1CNT_X, REG_SOUND2CNT_H, REG_SOUND3CNT_X)
*/
/*!	\{	*/

#define SFREQ_HOLD				 0	//!< Continuous play
#define SFREQ_TIMED			0x4000	//!< Timed play
#define SFREQ_RESET			0x8000	//!< Reset sound

#define SFREQ_RATE_MASK		0x07FF
#define SFREQ_RATE_SHIFT		 0
#define SFREQ_RATE(n)		((n)<<SFREQ_RATE_SHIFT)

#define SFREQ_BUILD(rate, timed, reset)				\
	( ((rate)&0x7FF) | ((timed)<<14) | ((reset)<<15) )

/*!	\}	/defgroup	*/

// --- REG_SNDDMGCNT ---------------------------------------------------

/*!	\defgroup grpAudioSDMG	Tone Generator, Control Flags
\ingroup grpMemBits
\brief	Bits for REG_SNDDMGCNT (aka REG_SOUNDCNT_L)
*/
/*!	\{	*/


#define SDMG_LSQR1		0x0100	//!< Enable channel 1 on left 
#define SDMG_LSQR2		0x0200	//!< Enable channel 2 on left
#define SDMG_LWAVE		0x0400	//!< Enable channel 3 on left
#define SDMG_LNOISE		0x0800	//!< Enable channel 4 on left	
#define SDMG_RSQR1		0x1000	//!< Enable channel 1 on right
#define SDMG_RSQR2		0x2000	//!< Enable channel 2 on right
#define SDMG_RWAVE		0x4000	//!< Enable channel 3 on right
#define SDMG_RNOISE		0x8000	//!< Enable channel 4 on right

#define SDMG_LVOL_MASK	0x0007
#define SDMG_LVOL_SHIFT		 0
#define SDMG_LVOL(n)	((n)<<SDMG_LVOL_SHIFT)

#define SDMG_RVOL_MASK	0x0070
#define SDMG_RVOL_SHIFT		 4
#define SDMG_RVOL(n)	((n)<<SDMG_RVOL_SHIFT)


// Unshifted values
#define SDMG_SQR1		0x01
#define SDMG_SQR2		0x02
#define SDMG_WAVE		0x04
#define SDMG_NOISE		0x08


#define SDMG_BUILD(_lmode, _rmode, _lvol, _rvol)	\
	( ((_rmode)<<12) | ((_lmode)<<8) | (((_rvol)&7)<<4) | ((_lvol)&7) )

#define SDMG_BUILD_LR(_mode, _vol) SDMG_BUILD(_mode, _mode, _vol, _vol)

/*!	\}	/defgroup	*/

// --- REG_SNDDSCNT ----------------------------------------------------

/*!	\defgroup grpAudioSDS	Direct Sound Flags
\ingroup grpMemBits
\brief	Bits for REG_SNDDSCNT (aka REG_SOUNDCNT_H)
*/
/*!	\{	*/

#define SDS_DMG25			 0	//!< Tone generators at 25% volume
#define SDS_DMG50		0x0001	//!< Tone generators at 50% volume
#define SDS_DMG100		0x0002	//!< Tone generators at 100% volume
#define SDS_A50			 0	//!< Direct Sound A at 50% volume
#define SDS_A100		0x0004	//!< Direct Sound A at 100% volume
#define SDS_B50			 0	//!< Direct Sound B at 50% volume
#define SDS_B100		0x0008	//!< Direct Sound B at 100% volume
#define SDS_AR			0x0100	//!< Enable Direct Sound A on right
#define SDS_AL			0x0200	//!< Enable Direct Sound A on left
#define SDS_ATMR0			 0	//!< Direct Sound A to use timer 0
#define SDS_ATMR1		0x0400	//!< Direct Sound A to use timer 1
#define SDS_ARESET		0x0800	//!< Reset FIFO of Direct Sound A
#define SDS_BR			0x1000	//!< Enable Direct Sound B on right
#define SDS_BL			0x2000	//!< Enable Direct Sound B on left
#define SDS_BTMR0			 0	//!< Direct Sound B to use timer 0
#define SDS_BTMR1		0x4000	//!< Direct Sound B to use timer 1
#define SDS_BRESET		0x8000	//!< Reset FIFO of Direct Sound B

/*!	\}	/defgroup	*/

// --- REG_SNDSTAT -----------------------------------------------------

/*!	\defgroup grpAudioSSTAT	Sound Status Flags
\ingroup grpMemBits
\brief	Bits for REG_SNDSTAT (and REG_SOUNDCNT_X)
*/
/*!	\{	*/

#define SSTAT_SQR1		0x0001	//!< (R) Channel 1 status
#define SSTAT_SQR2		0x0002	//!< (R) Channel 2 status
#define SSTAT_WAVE		0x0004	//!< (R) Channel 3 status
#define SSTAT_NOISE		0x0008	//!< (R) Channel 4 status
#define SSTAT_DISABLE		 0	//!< Disable sound
#define SSTAT_ENABLE	0x0080	//!< Enable sound. NOTE: enable before using any other sound regs



#define SND_RATE(note, oct, test) ( 2048-(test[note]>>(4+(oct))) )
//---------------------------------------------------------------------------------
#ifdef __cplusplus
}	   // extern "C"
#endif
//---------------------------------------------------------------------------------
#endif //_gba_sound_h_
