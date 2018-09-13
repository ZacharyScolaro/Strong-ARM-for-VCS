#include "Bitmap128.h"

Bitmap128::Bitmap128(int height, unsigned char backColor, const unsigned char * pColors, const unsigned char * pBitmap)
	: _height(height), _backColor(backColor), _pColors(pColors), _pBitmap(pBitmap)
{
}


void Bitmap128::PrepareFrame()
{
}

// _pColors must be at least height bytes
// bitmap must be at least height * 16 bytes
void Bitmap128::DrawFrame()
{
	int line = 0;
	static int evenOdd = 1;
	evenOdd ^= 1;

	vcsWrite5(CTRLPF, 0x35);
	vcsLdy2(0);
	vcsSty3(NUSIZ0);
	vcsSty3(NUSIZ1);
	vcsSty3(VDELP0);
	vcsSty3(VDELP1);
	vcsSty3(GRP0);
	vcsSty3(GRP1);
	vcsSty3(PF0);
	vcsSty3(PF1);
	vcsSty3(PF2);

	vcsWrite5(COLUP0, _pColors[0]);
	vcsWrite5(COLUP1, _pColors[0]);
	vcsWrite5(COLUBK, _backColor);
	vcsWrite5(COLUPF, _backColor);

	// Start 128 pixel kernel
	vcsSta3(WSYNC);
	vcsNop2n(18);
	vcsSta3(RESPone);
	vcsWrite5(HMP1, 0xe0);
	vcsJmp3();
	vcsNop2n(2);
	vcsSta3(RESBL);
	vcsWrite5(HMBL, 0x20);

	vcsSta3(WSYNC);
	vcsSta3(HMOVE);
	vcsNop2n(12);
	vcsSta3(HMCLR);
	vcsWrite5(HMP0, 0xb0);

	for (int i = 0; i <  97 - _height; i++)
	{
		vcsSty3(VBLANK);
		vcsSta3(WSYNC);
	}

	for (int i = 0, j = 0; i <= _height; i++, j++)
	{
		if (i == 0 && !evenOdd)
		{
			vcsNop2n(32);
			vcsLdy2(0x86);
			vcsJmp3();
		}
		else
		{
			if (i == 0 && evenOdd)
			{
				vcsNop2n(28);
				vcsLdy2(3);
				vcsSty3(NUSIZ0);
				vcsJmp3();
			}
			else
			{
				vcsLdy2(3);
				vcsSty3(NUSIZ0);
				vcsWrite5(GRP1, _pBitmap[line + 5]); // 5
				vcsWrite5(GRP0, _pBitmap[line + 1]); // 1
				vcsWrite5(GRP0, _pBitmap[line + 3]); // 3
				vcsWrite5(GRP0, _pBitmap[line + 10]); // 10
				vcsWrite5(GRP1, _pBitmap[line + 9]); // 9
				vcsSta3(RESP0);
				vcsLda2(_pBitmap[line + 13]);			  // 13
				vcsSta4(RESP0);
				vcsSta3(GRP1);
				vcsStx3(GRP0);
				vcsWrite5(GRP0, _pBitmap[line + 14]); // 14
				if (evenOdd == 0) { line += 16; }
			}
			// EntryEven:
			vcsSty3(HMP1);
			vcsSty3(HMBL);
			if (evenOdd && i == _height) { break; }
			vcsSta3(HMOVE);
			vcsSta3(RESP0);
			vcsSty3(ENABL);
			vcsSty3(NUSIZ1);

			// COLOR CHANGE
			vcsLda2(_pColors[j++]);
			vcsSta4(COLUP0);

			vcsLdx2(_pBitmap[line + 6]);				// 6
			vcsLdy2(_pBitmap[line + 8]);				// 8
			vcsWrite5(GRP0, _pBitmap[line]);	// 0

			vcsWrite5(GRP1, _pBitmap[line + 4]);	// 4
			vcsWrite5(GRP0, _pBitmap[line + 2]);	// 2
			vcsWrite5(GRP0, _pBitmap[line + 7]);  // 7
			vcsSta3(RESP0);
			vcsStx3(GRP1);
			vcsSta3(RESP0);
			vcsSty3(GRP1);

			vcsLdy2(0x86);
			vcsWrite5(GRP0, _pBitmap[line + 11]);	// 11
			vcsSty3(NUSIZ0);
			vcsWrite5(GRP0, _pBitmap[line + 15]);	// 15

			vcsSty3(HMP1);
			vcsSty3(HMBL);

			if (evenOdd) { line += 16; }
		}
		// EntryOdd:
		vcsLda2(0);
		vcsLdx2(_pBitmap[line + 12]);				// 12
		vcsSta3(RESP0);
		vcsSta3(HMOVE);
		vcsSty3(NUSIZ1);
		vcsSta3(ENABL);
		// COLOR CHANGE
		if (j > 1 && _pColors[j] == _pColors[j - 2])
		{
			vcsNop2();
			vcsJmp3();
		}
		else
		{
			vcsWrite5(COLUP1, _pColors[j]);
		}
	}

	vcsLda2(0);
	vcsSta3(GRP0);
	vcsSta3(GRP1);
	vcsSta3(WSYNC);

	for (int i = 0; i < 97 - _height; i++)
	{
		vcsSta3(WSYNC);
	}
}
