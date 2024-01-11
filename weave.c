#include "weave.h"

// Weave data in d into w.
int weave(Byte *w, Byte *d, uint grain, uint period, uint num){
  if (period%grain > 0){
    return 1;
  }
  uint i,j,k;
  for (i=0; i<period; i+=grain)
    for (j=0; j<num; j++)
      for (k=0; k<grain; k++)
        //printf("%c\n", )
        *(w+(j*grain+i*num+k))=*(d+(j*period)+i+k);
  return 0;
}


int unweave(Byte *d, Byte *w, uint grain, uint period, uint num, uint count){
  if (period%grain > 0){
    return 1;
  }
  uint i,j,k;
  // Input error correction
  if (count>num)
    count = num;

  for (i=0; i<period; i+=grain)
    for (j=0; j<count; j++)
      for (k=0; k<grain; k++)
        *(d+(j*period+i+k)) = *(w+(j*grain+i*num+k));
  return 0;
}


Byte unweave_one(Byte *w, uint i, uint n, uint grain, uint period, uint num){
  if (i>=period || n>=num)
    return 0;
  int set = i/grain; // Quotient.
  return *(w + set*grain*num + n*grain + i%grain);
}

