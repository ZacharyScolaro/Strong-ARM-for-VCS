#ifndef DISPLAYKERNELS_TILEDSPRITES_H_
#define DISPLAYKERNELS_TILEDSPRITES_H_

#include "displayKernel.h"

class TiledSprites : public DisplayKernel
{
public:
	TiledSprites();
	void PrepareFrame();
	void DrawFrame();

private:
	bool _oddFrame;
};


#endif /* DISPLAYKERNELS_TILEDSPRITES_H_ */
