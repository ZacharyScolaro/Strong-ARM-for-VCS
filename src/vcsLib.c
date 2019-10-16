#include <stdint.h>

#include "ace2600.h"
#include "vcsLib.h"
#include "cartridge_io.h"

#define SetNextRomAddress(addr) next_address = addr

extern void Reset_Handler();

extern void __file_size;

extern ACEFileHeader AceHeader;

ACEFileHeader AceHeader __attribute__ ((section (".file_header"))) __attribute__ ((__used__)) =
{
	.magic_number = "ACE-2600",
	.driver_name = "Strong-ARM      ",
	.driver_version = 1,
	.rom_size = &__file_size,
	.rom_checksum = 0,
	.entry_point = (uint32_t)Reset_Handler
};


static unsigned short next_address;

void InjectRomByte(unsigned char value)
{
	while (next_address != ADDR_IN)
		;

	DATA_OUT = ((uint16_t)value) << 8;
	SET_DATA_MODE_OUT
		next_address++;
}

void YieldDataBus(unsigned short address)
{
	while (ADDR_IN != address)
		;

	SET_DATA_MODE_IN
}

unsigned char SnoopDataBus(unsigned short address)
{
	while (ADDR_IN != address)
		;

	SET_DATA_MODE_IN
	// Give peripheral time to respond
	for (int i = 0; i < 15; i++)
	{
		int x = ADDR_IN;
	}
	return DATA_IN >> 8;
}

unsigned char vcsRead4(unsigned short address)
{
	InjectRomByte(0xad);
	InjectRomByte(address & 0xff);
	InjectRomByte(address >> 8);
	return SnoopDataBus(address);
}


void vcsJmp3()
{
	InjectRomByte(0x4c);
	InjectRomByte(0x00);
	InjectRomByte(0x10);
	SetNextRomAddress(0x1000);
}

void StartOverblank()
{
	InjectRomByte(0x4c);
	InjectRomByte(0x80);
	InjectRomByte(0x00);
	YieldDataBus(0x0080);
}

void EndOverblank()
{
	SetNextRomAddress(0x1fff);
	InjectRomByte(0x00);
	YieldDataBus(0x00ac);
	SetNextRomAddress(0x1000);
}

void vcsSetMasks(unsigned char * aMask, unsigned char * xMask, unsigned char * yMask)
{
	*aMask = *xMask = *yMask = 0xff;
}

void vcsWrite3(unsigned char ZP, unsigned char data)
{
	InjectRomByte(0x85);
	InjectRomByte(ZP);

	// Stuff in the data over what's there
	while (ZP != ADDR_IN)
		;

	DATA_OUT = ((uint16_t)data) << 8;
	SET_DATA_MODE_OUT
}

void vcsWrite5(unsigned char ZP, unsigned char data)
{
	InjectRomByte(0xa9);
	InjectRomByte(data);
	InjectRomByte(0x85);
	InjectRomByte(ZP);
	YieldDataBus(ZP);
}

void vcsLda2(unsigned char data)
{
	InjectRomByte(0xa9);
	InjectRomByte(data);
}

void vcsLdx2(unsigned char data)
{
	InjectRomByte(0xa2);
	InjectRomByte(data);
}

void vcsLdy2(unsigned char data)
{
	InjectRomByte(0xa0);
	InjectRomByte(data);
}

void vcsSta3(unsigned char ZP)
{
	InjectRomByte(0x85);
	InjectRomByte(ZP);
	YieldDataBus(ZP);
}

void vcsSta4(unsigned char ZP)
{
	InjectRomByte(0x8d);
	InjectRomByte(ZP);
	InjectRomByte(00);
	YieldDataBus(ZP);
}

void vcsStx3(unsigned char ZP)
{
	InjectRomByte(0x86);
	InjectRomByte(ZP);
	YieldDataBus(ZP);
}

void vcsSty3(unsigned char ZP)
{
	InjectRomByte(0x84);
	InjectRomByte(ZP);
	YieldDataBus(ZP);
}

void vcsTxs2()
{
	InjectRomByte(0x9a);
}

void vcsJsr6(unsigned short target)
{
	InjectRomByte(0x20);
	InjectRomByte(target & 0xff);

	//Stack operations
	while (ADDR_IN & 0x1000)
		;
	SET_DATA_MODE_IN

	InjectRomByte(target >> 8);
	SetNextRomAddress(target & 0x1fff);
}

void vcsLibInit()
{
	// Signal ZP load routine to transfer control back to ROM
	SetNextRomAddress(0x1000);
	InjectRomByte(0xd8);
	SetNextRomAddress(0x1fef);
	InjectRomByte(0xff);
	SetNextRomAddress(0x1ffc);
	InjectRomByte(0x00);
	InjectRomByte(0x10);
	SetNextRomAddress(0x1000);
}

void vcsNop2()
{
	InjectRomByte(0xea);
}

// Puts nop on bus for n * 2 cycles
// Use this to perform lengthy calculations
void vcsNop2n(int n)
{
	InjectRomByte(0xea);
	next_address += n-1;
}
