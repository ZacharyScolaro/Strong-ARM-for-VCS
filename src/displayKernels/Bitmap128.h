#ifndef DISPLAYKERNELS_BITMAP128_H_
#define DISPLAYKERNELS_BITMAP128_H_

#include "displayKernel.h"

// Abstract class which should be the base for all display kernels
class Bitmap128 : public DisplayKernel
{
public:
	Bitmap128(int height, unsigned char backColor, const unsigned char * pColors, const unsigned char * pBitmap);
	void PrepareFrame();
	void DrawFrame();
	
private:
	int _height;
	unsigned char _backColor;
	const unsigned char * _pColors;
	const unsigned char * _pBitmap;
};


#endif /* DISPLAYKERNELS_BITMAP128_H_ */
