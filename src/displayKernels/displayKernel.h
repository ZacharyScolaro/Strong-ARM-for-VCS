#ifndef DISPLAYKERNELS_DISPLAYKERNEL_H_
#define DISPLAYKERNELS_DISPLAYKERNEL_H_

#include "vcsLib.h"

// Abstract class which should be the base for all display kernels
class DisplayKernel
{
public:
	virtual void PrepareFrame() = 0; // Called during vblank to give time to fill buffers etc.
	virtual void DrawFrame() = 0; // Called either just prior to vblank ending or during frame rendering. Must be called at exactly scanline cycle 0
};


#endif /* DISPLAYKERNELS_DISPLAYKERNEL_H_ */
