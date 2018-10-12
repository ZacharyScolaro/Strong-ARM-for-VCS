#ifndef DISPLAYKERNELS_RUNANDGUN_H_
#define DISPLAYKERNELS_RUNANDGUN_H_

#include "displayKernel.h"

#define PlatformRows 44

struct RnGPlayerSprite;

struct RnGPlayer
{
	bool Reflect;
	int X;
	int Y;
	const RnGPlayerSprite * Sprite;
};

struct RnGPlayerSprite
{
	RnGPlayerSprite(unsigned char initialNusiz, int height, int leftXIndent, int rightXIndent,
		const unsigned char * graphic, const unsigned char * colorNusiz, const unsigned char * hMove)
		: InitialNusiz(initialNusiz), Height(height), LeftXIndent(leftXIndent), RightXIndent(rightXIndent),
		Graphic(graphic), ColorNusiz(colorNusiz), HMove(hMove)
	{
	}

	unsigned char InitialNusiz;
	int Height;
	int LeftXIndent;
	int RightXIndent;
	// Must point to arrays of at least size Height elements
	const unsigned char * Graphic;
	const unsigned char * ColorNusiz; // Color when D0 cleared, D7-D5 NUSIZ0 when D0 set
	const unsigned char * HMove; // D7-D5 hmove for normal orientation, D4-D0 hmove for reflected orientation.
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
	RnGPlayer _player;

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
