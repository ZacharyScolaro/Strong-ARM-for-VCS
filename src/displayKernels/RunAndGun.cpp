#include "RunAndGun.h"

#define getgrp0 _p0grp[line]
#define writeColuP0 { unsigned char sizCol = _p0col[line]; if (sizCol & 1)				{					vcsWrite5(NUSIZ0, sizCol >> 4);				}				else				{					vcsWrite5(COLUP0, sizCol);				} }

static const unsigned char evenPositionTable[] =
{
	0x41, 0x31, 0x21, 0x11, 0x01, 0xf1, 0xe1, 0xd1, 0xc1, 0xb1, 0xa1, 0x91, 0x81,
	0xe2, 0xd2, 0xc2, 0xb2, 0xa2, 0x92, 0x82,
	0xb3, 0xa3, 0x93, 0x83,
	0x64, 0x54, 0x44, 0x34, 0x24, 0x14, 0x04, 0xf4, 0xe4, 0xd4, 0xc4, 0xb4, 0xa4, 0x94, 0x84,
	0x65, 0x55, 0x45, 0x35, 0x25, 0x15, 0x05, 0xf5, 0xe5, 0xd5, 0xc5, 0xb5, 0xa5, 0x95, 0x85,
	0x66, 0x56, 0x46, 0x36, 0x26, 0x16, 0x06, 0xf6, 0xe6, 0xd6, 0xc6, 0xb6, 0xa6, 0x96, 0x86,
	0x67, 0x57, 0x47, 0x37, 0x27, 0x17, 0x07, 0xf7, 0xe7, 0xd7, 0xc7, 0xb7, 0xa7, 0x97, 0x87,
	0x68, 0x58, 0x48, 0x38, 0x28, 0x18, 0x08, 0xf8, 0xe8, 0xd8, 0xc8, 0xb8, 0xa8, 0x98, 0x88,
	0x69, 0x59, 0x49, 0x39, 0x29, 0x19, 0x09, 0xf9, 0xe9, 0xd9, 0xc9, 0xb9, 0xa9, 0x99, 0x89,
	0x6a, 0x5a, 0x4a, 0x3a, 0x2a, 0x1a, 0x0a, 0xfa, 0xea, 0xda, 0xca, 0xba, 0xaa, 0x9a, 0x8a,
	0xdb, 0xcb, 0xbb, 0xab, 0x9b, 0x8b,
	0x6c, 0x5c, 0x4c, 0x3c, 0x2c, 0x1c, 0x0c, 0xfc, 0xec, 0xdc, 0xcc, 0xbc, 0xac, 0x9c, 0x8c,
	0x6d, 0x5d, 0x4d, 0x3d, 0x2d, 0x1d, 0x0d, 0xfd, 0xed, 0xdd
};

static const unsigned char oddPositionTable[] =
{
	0x41, 0x31, 0x21, 0x11, 0x01, 0xf1, 0xe1, 0xd1, 0xc1, 0xb1, 0xa1, 0x91, 0x81,
	0xe2, 0xd2, 0xc2, 0xb2, 0xa2, 0x92, 0x82,
	0xe3, 0xd3, 0xc3, 0xb3, 0xa3, 0x93, 0x83,
	0x64, 0x54, 0x44, 0x34, 0x24, 0x14, 0x04, 0xf4, 0xe4, 0xd4, 0xc4, 0xb4, 0xa4, 0x94, 0x84,
	0x65, 0x55, 0x45, 0x35, 0x25, 0x15, 0x05, 0xf5, 0xe5, 0xd5, 0xc5, 0xb5, 0xa5, 0x95, 0x85,
	0x66, 0x56, 0x46, 0x36, 0x26, 0x16, 0x06, 0xf6, 0xe6, 0xd6, 0xc6, 0xb6, 0xa6, 0x96, 0x86,
	0x67, 0x57, 0x47, 0x37, 0x27, 0x17, 0x07, 0xf7, 0xe7, 0xd7, 0xc7, 0xb7, 0xa7, 0x97, 0x87,
	0x68, 0x58, 0x48, 0x38, 0x28, 0x18, 0x08, 0xf8, 0xe8, 0xd8, 0xc8, 0xb8, 0xa8, 0x98, 0x88,
	0x69, 0x59, 0x49, 0x39, 0x29, 0x19, 0x09, 0xf9, 0xe9, 0xd9, 0xc9, 0xb9, 0xa9, 0x99, 0x89,
	0x6a, 0x5a, 0x4a, 0x3a, 0x2a, 0x1a, 0x0a, 0xfa, 0xea, 0xda, 0xca, 0xba, 0xaa, 0x9a, 0x8a,
	0xdb, 0xcb, 0xbb, 0xab, 0x9b, 0x8b,
	0x6c, 0x5c, 0x4c, 0x3c, 0x2c, 0x1c, 0x0c, 0xfc, 0xec, 0xdc, 0xcc, 0xbc, 0xac, 0x9c, 0x8c,
	0x6d, 0x5d, 0x4d, 0x3d, 0x2d, 0x1d, 0x0d
};


RunAndGun::RunAndGun()
{
	_scrollOffset = 0;
}

void RunAndGun::PrepareFrame()
{
	_p0initialNusiz = _player.Sprite->InitialNusiz;
	_p0x = _player.X + (_player.Reflect ? _player.Sprite->RightXIndent : _player.Sprite->LeftXIndent);
	for (int i = -_player.Sprite->Height; i < 192; i++)
	{
		int index = i +  _player.Sprite->Height - _player.Y;
		
		if (index < 0 || _player.Sprite->Height <= index)
		{
			if (0 <= i && i < 192)
			{
				_p0grp[i] = _p0col[i] = _p0hmove[i] = 0;
			}
		}
		else
		{
			unsigned char colSiz = _player.Sprite->ColorNusiz[index];
			unsigned char hmove = _player.Reflect ? _player.Sprite->HMove[index] << 4 : _player.Sprite->HMove[index];
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
				_p0grp[i] = _player.Reflect ? reverseByte[_player.Sprite->Graphic[index]] : _player.Sprite->Graphic[index];
				_p0col[i] = _player.Sprite->ColorNusiz[index];
				_p0hmove[i] = hmove;
			}
		}
	}


	for (int i = 0; i < 44; i++)
	{
		_p1Position[i] = 0;
	}
	for (int i = 0; i < 192; i++)
	{
		_p1Graphic[i] = 0;
		_p1ColorNusiz[i] = 0;
	}
	for (int i = 0; i < 44; i++)
	{
		if(_p1[i].Enabled)
		{
			_p1Position[i] = (_scrollOffset & 1) ? oddPositionTable[_p1[i].X] : evenPositionTable[_p1[i].X];
			int y = i * 4 + 16 + _p1[i].Y;
			_p1ColorNusiz[y++] = (_p1[i].Nusiz << 4) | 1;
			for (int j = 0; j < _p1[i].Sprite->Height; j++)
			{
				_p1Graphic[j + y] = _p1[i].Sprite->Graphic[j];
				_p1ColorNusiz[j+y+1] = _p1[i].Sprite->ColorNusiz[j];
			}
		}
	}

	// find initial P1 size
	_p1InitialNusiz = 0;
	for (int i = 0; i < 192; i++)
	{
		if (_p1ColorNusiz[i] & 1)
		{
			_p1InitialNusiz = _p1ColorNusiz[i] >> 4;
			break;
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
	vcsWrite5(NUSIZ1, _p1InitialNusiz);
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
		int resp = _p1Position[i] & 0x0f;
		if (resp == 0)
		{
			if (_p1ColorNusiz[line] & 1)
			{
				vcsStx3(NUSIZ1);
			}
			else
			{
				vcsStx3(COLUP1);
			}
		}
		if (resp == 1)
		{
			vcsSta3(RESPone);
		}
		line++;
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
		vcsWrite5(GRP1, _p1Graphic[line]);
		if (resp == 0xa)
		{
			vcsSta3(RESPone);
		}
		vcsLdx2((_p1ColorNusiz[line] & 1) ? _p1ColorNusiz[line] >> 4 : _p1ColorNusiz[line]);
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
		vcsWrite5(HMP1, _p1Position[i]);
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
			if (_p1ColorNusiz[line] & 1)
			{
				vcsStx3(NUSIZ1);
			}
			else
			{
				vcsStx3(COLUP1);
			}
			line++;
			vcsWrite5(GRP0, getgrp0);
			writeColuP0;

			if (_scrollOffset < 3)
			{
				if (!_scrollOffset)
				{
					vcsLdx2((_p1ColorNusiz[line] & 1) ? _p1ColorNusiz[line] >> 4 : _p1ColorNusiz[line]);
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
						vcsWrite5(GRP1, _p1Graphic[line]);
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
				vcsLdx2((_p1ColorNusiz[line] & 1) ? _p1ColorNusiz[line] >> 4 : _p1ColorNusiz[line]);
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

