#ifndef DISPLAYKERNELS_RUNANDGUN_H_
#define DISPLAYKERNELS_RUNANDGUN_H_

#include "displayKernel.h"

#define PlatformRows 44

class RnGPlayer
{
public:
	bool Reflect;
	unsigned char InitialNusiz;
	int X;
	int Y;
	int Height;
	unsigned char Graphic[192];
	unsigned char ColorNusiz[192]; // Color when D0 cleared, D7-D5 NUSIZ0 when D0 set
	unsigned char HMove[192]; // D7-D5 hmove for normal orientation, D4-D0 hmove for reflected orientation.
};

class RunAndGun : public DisplayKernel
{
public:
	RunAndGun();
	void PrepareFrame();
	void DrawFrame();
	void SetScrollOffset(int offset);

	// Player
	int _p0x;
	unsigned char _p0initialColor;
	unsigned char _p0initialNusiz;
	RnGPlayer * _player;

	// Status Bar
	unsigned char _leftIconGraphic[14];
	unsigned char _leftIconColor[14];
	unsigned char _middleIconGraphic[14];
	unsigned char _middleIconColor[14];
	unsigned char _rightIconGraphic[14];
	unsigned char _rightIconColor[14];
	unsigned char _statusBarPF1a[14];
	unsigned char _statusBarPF2a[14];
	unsigned char _statusBarPF0b[14];
	unsigned char _statusBarPF1b[14];
	unsigned char _statusBarPF2b[14];


	unsigned char PlatformColumns[3 * 11 * PlatformRows];
	unsigned char BulletPF[PlatformRows * 6 + 1];

private:
	int _scrollOffset;
	unsigned char _p0grp[192];
	unsigned char _p0col[192];
	unsigned char _p0hmove[192];
};


#endif /* DISPLAYKERNELS_RUNANDGUN_H_ */
