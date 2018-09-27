#ifndef DISPLAYKERNELS_DISPLAYKERNEL_H_
#define DISPLAYKERNELS_DISPLAYKERNEL_H_

#include "vcsLib.h"

extern const unsigned char reverseByte[256];
extern const int moveMap[16];

// Abstract class which should be the base for all display kernels
class DisplayKernel
{
public:
	virtual void PrepareFrame() = 0; // Called during vblank to give time to fill buffers etc.
	virtual void DrawFrame() = 0; // Called either just prior to vblank ending or during frame rendering. Must be called at exactly scanline cycle 0

	// TODO clean this mess up
	void strobePosition(unsigned char strobeAddress, int position);
};


#endif /* DISPLAYKERNELS_DISPLAYKERNEL_H_ */
