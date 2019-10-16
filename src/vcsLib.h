#ifndef VCSLIB_H
#define VCSLIB_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "vcs.h"

unsigned char vcsRead4(unsigned short address);

 void vcsNop2();


// Puts nop on bus for n * 2 cycles
// Use this to perform lengthy calculations
void vcsNop2n(int n);

void vcsJmp3();

void StartOverblank();

void EndOverblank();

void vcsWrite3(unsigned char ZP, unsigned char data);

void vcsWrite5(unsigned char ZP, unsigned char data);

void vcsLda2(unsigned char data);

void vcsLdx2(unsigned char data);

void vcsLdy2(unsigned char data);

void vcsSta3(unsigned char ZP);

void vcsSta4(unsigned char ZP);

void vcsStx3(unsigned char ZP);

void vcsSty3(unsigned char ZP);

void vcsTxs2();

void vcsJsr6(unsigned short target);

void vcsLibInit();

void vcsSetMasks(unsigned char * aMask, unsigned char * xMask, unsigned char * yMask);

#ifdef __cplusplus
}
#endif

#endif // VCSLIB_H
