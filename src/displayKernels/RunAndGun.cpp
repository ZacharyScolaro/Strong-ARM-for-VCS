#include "RunAndGun.h"

#define getgrp0 _p0grp[line]
#define writeColuP0 { unsigned char sizCol = _p0col[line]; if (sizCol & 1)				{					vcsWrite5(NUSIZ0, sizCol >> 4);				}				else				{					vcsWrite5(COLUP0, sizCol);				} }

RunAndGun::RunAndGun()
{
	_scrollOffset = 0;
}


void RunAndGun::PrepareFrame()
{
	_p0initialNusiz = _player->InitialNusiz;
	_p0x = _player->X;
	for (int i = -_player->Height; i < 192; i++)
	{
		int index = i +  _player->Height - _player->Y;
		
		if (index < 0 || _player->Height <= index)
		{
			if (0 <= i && i < 192)
			{
				_p0grp[i] = _p0col[i] = _p0hmove[i] = 0;
			}
		}
		else
		{
			unsigned char colSiz = _player->ColorNusiz[index];
			unsigned char hmove = _player->Reflect ? _player->HMove[index] << 4 : _player->HMove[index];
			if (i < 0)
			{
				if (colSiz & 1)
				{
					_p0initialNusiz = colSiz >> 4;
				}
				else
				{
					_p0initialColor = colSiz;
				}
				_p0x += moveMap[hmove >> 4];
			}
			else if (i < 192)
			{
				_p0grp[i] = _player->Reflect ? reverseByte[_player->Graphic[index]] : _player->Graphic[index];
				_p0col[i] = _player->ColorNusiz[index];
				_p0hmove[i] = hmove;
			}
		}
	}
}

// _pColors must be at least height bytes
// bitmap must be at least height * 16 bytes
void RunAndGun::DrawFrame()
{
	int i, j, k;
	int pfIndex = 0;
	int line = 0;
	int cbi = -11;

	vcsWrite5(CTRLPF, 0x30);
	vcsWrite5(NUSIZ0, _p0initialNusiz);
	vcsWrite5(NUSIZ1, 6);
	vcsWrite5(VDELP1, 0);
	vcsWrite5(GRP0, 0);
	vcsWrite5(GRP1, 0);
	vcsWrite5(ENABL, 2);

	vcsWrite5(COLUBK, PlatformColumns[0]);
	vcsWrite5(COLUPF, 0);
	vcsWrite5(PF0, 0);
	vcsWrite5(PF1, 0);
	vcsWrite5(PF2, 0);

	vcsSta3(WSYNC);
	strobePosition(RESBL, 153);
	strobePosition(RESP0, _p0x);
	strobePosition(RESPone, 10);
	//
	vcsSta3(HMOVE);
	vcsSta3(WSYNC);
	vcsSta3(HMCLR);
	vcsLdy2(0);
	vcsWrite5(HMP0, 0);
	vcsWrite5(GRP1, _leftIconGraphic[0]);
	vcsWrite5(COLUP1, _leftIconColor[0]);
	vcsWrite5(COLUP0, _p0initialColor);
	vcsLdx2(_middleIconColor[0]);
	vcsWrite5(PF1, _statusBarPF1a[0]);
	vcsSta3(WSYNC);

	// Start visible frame
	for (i = 0; i < 14; )
	{
		vcsSta3(HMOVE);
		vcsWrite5(GRP0, getgrp0);
		writeColuP0;
		if (i == 0)
		{
			vcsSty3(VBLANK);
			vcsJmp3();
		}
		else
		{
			vcsWrite5(PF1, _statusBarPF1a[i]);
		}
		vcsWrite5(PF2, _statusBarPF2a[i]);
		vcsLda2(_middleIconGraphic[i]);
		vcsStx3(COLUP1);
		vcsSta3(GRP1);
		vcsWrite5(PF0, _statusBarPF0b[i]);
		vcsWrite5(GRP1, _rightIconGraphic[i]);
		vcsWrite5(COLUP1, _rightIconColor[i]);
		vcsWrite5(PF1, _statusBarPF1b[i]);
		vcsWrite5(PF2, _statusBarPF2b[i]);
		i++;
		vcsLdx2(i > 13 ? 0 : _middleIconColor[i]);
		vcsWrite5(HMP0, _p0hmove[line]);
		line++;
		vcsWrite5(GRP1, i > 13 ? 0 : _leftIconGraphic[i]);
		vcsWrite5(COLUP1, i > 13 ? 0 : _leftIconColor[i]);
		vcsSty3(PF0);
	}
	//line 15 *** starts at 1 cycle in instead of 0 ***
	vcsSta3(HMOVE);
	vcsWrite5(GRP0, getgrp0);
	writeColuP0;
	vcsSty3(PF1);
	vcsSty3(PF2);
	vcsWrite5(VDELP1, 1);
	vcsWrite5(NUSIZ1, 0);
	vcsWrite5(CTRLPF, 0x34);
	vcsWrite5(HMP0, _p0hmove[line]);
	line++;
	vcsSta3(WSYNC);
	// line 16
	vcsSta3(HMOVE);
	vcsWrite5(GRP0, getgrp0);
	writeColuP0;
	vcsJmp3();
	vcsNop2n(23);
	vcsLdx2(0xff);
	vcsWrite5(PF0, BulletPF[pfIndex++]);
	vcsWrite5(HMP0, _p0hmove[line]);
	if (_scrollOffset & 1)
	{
		// shift odd scroll values by 1 cycle
		vcsJmp3();
	}
	else
	{
		vcsNop2();
	}

	for (i = 0; i < PlatformRows; i++)
	{
		// Bullet
		vcsSta3(HMOVE);			// 3
		line++;
		int resp = 0;// TODO p1pos[i] & 0x0f;
		if (resp == 0)
		{
			vcsStx3(COLUP1);
		}
		if (resp == 1)
		{
			vcsSta3(RESPone);
		}
		vcsWrite5(GRP0, getgrp0);
		writeColuP0;
		vcsWrite5(COLUBK, PlatformColumns[0]);
		cbi += 11;
		if (resp == 2)
		{
			vcsSta3(RESPone);
		}
		vcsWrite5(PF1, BulletPF[pfIndex++]);
		if (resp == 3)
		{
			vcsSta3(RESPone);
		}
		vcsWrite5(PF2, BulletPF[pfIndex++]);
		if (resp == 4)
		{
			vcsSta3(RESPone);
		}
		vcsWrite5(PF0, BulletPF[pfIndex++]);
		if (resp == 5)
		{
			vcsSta3(RESPone);
		}
		vcsWrite5(PF1, BulletPF[pfIndex++]);
		if (resp == 6)
		{
			vcsSta3(RESPone);
		}
		vcsWrite5(HMP0, _p0hmove[line]);
		if (resp == 7)
		{
			vcsSta3(RESPone);
		}
		vcsWrite5(PF2, BulletPF[pfIndex++]);
		if (resp == 8)
		{
			vcsSta3(RESPone);
		}
		vcsWrite5(PF0, 0);
		if (resp == 9)
		{
			vcsSta3(RESPone);
		}
		vcsWrite5(GRP1, 0); // TODO  p1grp[line]);
		if (resp == 0xa)
		{
			vcsSta3(RESPone);
		}
		vcsNop2(); // TODO vcsLdx2(p1col[line]);
		if (resp == 0xb)
		{
			vcsSta3(RESPone);
		}
		vcsLdy2(0); // Y must be 0 when entering the next sub-kernel
		vcsSty3(PF1);
		if (resp == 0xc)
		{
			vcsSta3(RESPone);
		}
		vcsWrite5(HMP1, 0); // TODO  p1pos[i]);
		if (resp == 0xd)
		{
			vcsSta3(RESPone);
		}
		// Platform Top/Bottom/Middle
		vcsSty3(PF2);
		for (j = 0; j < 3; j++)
		{
			if (j == 2)
			{
				vcsSta4(HMOVE);
			}
			else
			{
				vcsSta3(HMOVE);
			}
			line++;
			vcsWrite5(GRP0, getgrp0);
			writeColuP0;
			vcsStx3(COLUP1);

			if (_scrollOffset < 3)
			{
				if (!_scrollOffset)
				{
					vcsNop2(); // TODO vcsLdx2(p1col[line]);
				}
				vcsWrite5(COLUBK, PlatformColumns[cbi]);
			}
			else
			{
				vcsJmp3();
			}

			vcsWrite5(COLUBK, PlatformColumns[cbi + 1]);
			int repeats = 0;
			for (k = 2; k < 10; k++)
			{
				if (PlatformColumns[cbi + k] == PlatformColumns[cbi + k - 1])
				{
					if (repeats == 0)
					{
						vcsWrite5(HMP0, _p0hmove[line]);
					}
					else if (repeats == 1)
					{
						vcsWrite5(GRP1, 0); // TODO  p1grp[line]);
					}
					else
					{
						vcsJmp3();
						vcsNop2();
					}
					repeats++;
				}
				else
				{
					vcsWrite5(COLUBK, PlatformColumns[cbi + k]);
				}
			}
			vcsWrite5(COLUBK, PlatformColumns[cbi + 10]);

			if (_scrollOffset > 0)
			{
				vcsLdx2(0); // TODO  p1col[line]);
				if (_scrollOffset > 2)
				{
					vcsNop2();
				}
			}

			if (j == 1)
			{
				vcsLdy2(BulletPF[pfIndex++]);
			}
			else if (j == 0)
			{
				vcsSty3(HMP1);
			}
		}
		vcsSty3(PF0);
	}

	// On cycle 0 or 1 depending on scroll offset
	// give some extra time to return from function call
	vcsNop2n(4);
}

void RunAndGun::SetScrollOffset(int offset)
{
	if (0 <= offset && offset <= 5 && _scrollOffset != offset)
	{
		_scrollOffset = offset;
	}
}