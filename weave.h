
/* Weave and unweave frames of data.
 * A blok is a contiguous sequence of frames.
 * (grain) is the size in bytes of the units taken from each successive frame to form the weave.
 * (period) is the size in bytes of each frame, which must be identical.
 * (num) is the number of frames in the blok.
 * (count) is the number of frames to unweave.
 *
 * e.g.
 * grain = 2; period = 6; num = 3;
 * abcdefABCDEF123456 --weave--> abAB12cdCD34efEF56
 *
 * grain = 2; period = 6; num = 3; count = 2;
 * abAB12cdCD34efEF56 --unweave--> abcdefABCDEF
 *
 * grain = 2; period = 6; num = 3; count = 1;
 * 12cdCD34efEF56 --unweave--> 123456
 *
 */

/* TO DO:
 * datatype Byte: what is it, why? why not uchar? should I cast char * into Byte?
 */

#ifndef _WEAVE_H_
#define _WEAVE_H_

#include "zlib.h" /* Defn Byte, uInt. Using here for coherence with compression functions. */


/* Weave data in d into w. */
int weave(Byte *w, Byte *d, uInt grain, uInt period, uInt num);

/* Unweave woven data in w into d. */
int unweave(Byte *d, Byte *w, uInt grain, uInt period, uInt num, uInt count);

/* Pick out one byte from weave: i-th byte of frame n. */
Byte unweave_one(Byte *w, uInt i, uInt n, uInt grain, uInt period, uInt num);


#endif
