
#include "weave.h"


int weave(Byte *w, Byte *d, uInt grain, uInt period, uInt num){
  if (period%grain > 0){
    return 1;
  }
  uInt i,j,k;
  for (i=0; i<period; i+=grain)
    for (j=0; j<num; j++)
      for (k=0; k<grain; k++)
        *(w + j*grain + i*num + k) = *(d + j*period + i + k);
  return 0;
}


int unweave(Byte *d, Byte *w, uInt grain, uInt period, uInt num, uInt count){
  if (period%grain > 0){
    return 1;
  }
  uInt i,j,k;
  /* Input error correction */
  if (count>num)
    count = num;

  for (i=0; i<period; i+=grain)
    for (j=0; j<count; j++)
      for (k=0; k<grain; k++)
        *(d + j*period + i + k) = *(w + j*grain + i*num + k);
  return 0;
}


Byte unweave_one(Byte *w, uInt i, uInt n, uInt grain, uInt period, uInt num){
  if (i>=period || n>=num)
    return 0;
  int set = i/grain; /* Integer part, Euclidian div */
  return *(w + set*grain*num + n*grain + i%grain);
}

