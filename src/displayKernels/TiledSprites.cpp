#include "TiledSprites.h"

TiledSprites::TiledSprites()
{
}


void TiledSprites::PrepareFrame()
{
	_oddFrame = false;
}

void TiledSprites::DrawFrame()
{
	vcsWrite5(CTRLPF, 0x00);
	vcsWrite5(NUSIZ0, 6);
	vcsWrite5(NUSIZ1, 6);
	vcsWrite5(VDELP0, 0xff);
	vcsWrite5(VDELP1, 0xff);
	vcsWrite5(GRP0, 0);
	vcsWrite5(GRP1, 0);
	vcsWrite5(ENABL, 2);

	vcsWrite5(COLUBK, 0);
	vcsWrite5(COLUPF, 0);
	vcsWrite5(PF0, 0);
	vcsWrite5(PF1, 0);
	vcsWrite5(PF2, 0);

	vcsSta3(WSYNC);
	strobePosition(RESBL, 153);
	strobePosition(RESP0, 38);
	strobePosition(RESPone, 54);
	//
	vcsSta3(HMOVE);
	vcsSta3(WSYNC);
	vcsSta3(HMCLR);
	vcsLdx2(GRP1);
	vcsWrite5(VBLANK, 0);
	vcsSta3(WSYNC);
	int i = 0;
	unsigned short jsrl = 0xf150;
	unsigned short jsrr = 0xf150;
	unsigned char ctrlpfr = 0;
	vcsNop2n(38);
	//vcsJmp3();
	////if (_oddFrame)
	////{
	////	goto StaggeredFrame;
	////}
		for (; i < 192;)
	{
		// Left group starts cycle 0
		vcsJsr6(jsrl);  // G, I Graphics <-e1 results in I=$ff
		vcsWrite5(GRP0, i); // A graphic
		vcsWrite5(GRP1, i+7); // C graphic
		vcsWrite5(GRP0, i+13); // E graphic
		vcsWrite5(COLUP0, 0x18); // A color
		vcsLda2(0x38); // C color
		vcsSta4(COLUP1); // C color
		vcsLda2(0x58);	// E color
		vcsTxs2();
		// Should be 36 cycles prior to here
		vcsSta3(COLUP0); // E color
		vcsLda2(i);   // G Color - 41
														 // 0x20 - 23 = 0x09 => sample will offset range by 0-15 giving 0x20-0x2f
		vcsJsr6(0x1009 + (0 & 0xf));
		vcsSta3(COLUP1); // G Color
		vcsSta3(GRP1);   // Flush delay register
		vcsSty3(COLUP0); // I Color - 56

		i++;
		jsrr = (((unsigned short)0xff) << 8) | 0xff; // H and J graphics bytes
		vcsWrite5(HMP0, 0x80);
		ctrlpfr = ((jsrr & 0x1000) >> 10) ^ 0x5;
		vcsSta3(HMP1);
		jsrr = (jsrr | 0x1000) - 33; //31 bytes between JSRs
		vcsSta4(HMBL);
		if ((jsrr & 0x1000) == 0)
		{
			jsrr += 0x800;
			ctrlpfr |= 0x10;
		}
		vcsLdy2(0xa8);	// J Color
		vcsWrite5(HMOVE, 2);

	StaggeredFrame:
		if (i >= 192)
		{
			break;
		}
		// Right group starts cycle -1
		vcsJsr6(jsrr);  // H, J Graphics <-de results in J=$ff
		vcsWrite5(GRP0, 0xff); // B graphic
		vcsWrite5(GRP1, 0xff); // D graphic
		vcsWrite5(GRP0, 0xff); // F graphic
		vcsWrite5(COLUP0, 0x28); // B color
		vcsWrite5(COLUP1, 0x48); // D color
		vcsLda2(ctrlpfr);
		vcsSta3(CTRLPF);
		vcsLda2(0x68);	// F color
		vcsTxs2();
		// Should be 39 cycles prior to here
		vcsSta3(COLUP0); // F color
		vcsLda2(0x88);   // H Color - 44
														 // 0x20 - 22 = 0x0a => sample will offset range by 0-15 giving 0x20-0x2f
		vcsJsr6(0x100a + (0 & 0xf));
		i++;
		jsrl = (((unsigned short)0xff) << 8) | 0xff; // G and I graphics bytes
		vcsSta3(COLUP1); // H Color
		unsigned char ctrlpfl = ((jsrl & 0x1000) >> 10) ^ 0x5;
		vcsSta3(GRP1);   // Flush delay register
		jsrl = (jsrl | 0x1000) - 30; //28 bytes between JSRs
		vcsSty3(COLUP0); // J Color - 59
		if ((jsrl & 0x1000) == 0)
		{
			jsrl += 0x800;
			ctrlpfl |= 0x10;
		}
		vcsWrite5(CTRLPF, ctrlpfl);
		vcsWrite5(HMCLR, 0x00);
		vcsWrite5(HMOVE, 2);
		vcsLdy2(0x98);	// I Color
	}

	// On cycle 0 or 1 depending on scroll offset
	// give some extra time to return from function call
	vcsNop2n(4);
	//_oddFrame = !_oddFrame;
}
