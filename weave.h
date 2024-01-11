/* 
* Weave frames of data together.
* A blok is a sequence of frames.
*
*/

#ifndef _WEAVE_H_
#define _WEAVE_H_

#include "zlib.h" // Contains def of Byte. Using here for coherence with compression functions.


int weave(Byte *w, Byte *d, uint grain, uint period, uint num);
int unweave(Byte *d, Byte *w, uint grain, uint period, uint num, uint count);
Byte unweave_one(Byte *w, uint i, uint count, uint grain, uint period, uint num);


#endif
