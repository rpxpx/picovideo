
/* Read in raw byte array for Pico LCD images. Compress with zlib. 
 * Weave images into bloks, compress the bloks. 
 * Use brute-force optimization to determine best grain size for weaving bloks.
 */

#include <stdio.h>  /* printf() output during compression. */
#include <stdlib.h> /* malloc(), free() */
#include <zlib.h>   /* compress(); sytem zlib, not local rp2xx */

#include "primed.h" /* struct inta, divizors(). Gives all possible grain sizes for weave tests. */

#include "weave.h"  /* weave() */
#include "Pico_spec.h" /* Total and available free SRAM. Free available SRAM found by compiling and running ram_find.c */
#include "ImageDataRaw.h" /* Data as #include in compilation. Raw byte array set of images. */


#define OUTHEDR "ImageDataBlokZ.h"
#define OUTDATA "ImageDataBlokZ.c"
#define OUTHEDRPRE "_IMAGEDATABLOKZ_H_"
#define PRINTWIDTH 16 /* 16 bytes per line. */


#define CHECK_ERR(err, msg){\
  if (err!=Z_OK){\
    fprintf(stderr, "%s error: %d\n", msg, err);\
    exit(1);\
  }\
}


int main(){

  uint frame_size, frames_len, bloks, num_max;
  frame_size = sizeof(frames[0]); /* period in weave. */
  frames_len = sizeof(frames)/frame_size; /* Total number of frames. Length of frames[] array. */
  num_max = FREE_SRAM/frame_size; /* Max frames in a blok. Leave room in RAM to unweave 1 frame to. FREE_SRAM defined in Pico_spec.h */
  bloks = ceil(frames_len/(num_max*1.0)); /* Number of bloks needed for all frames. */
  
  inta divs = divizors(frame_size);
  sort_inta(divs);
  
  printf("Assuming free SRAM: %d\n", FREE_SRAM);
  printf("max frames in each blok: %d\n", num_max);
  printf("frame size (B): %d; frame count: %d\n", frame_size, frames_len);

  Byte *blokweave = malloc(frame_size*num_max);
  Byte *blokweaveZ = malloc(frame_size*num_max);

  uLong zLen, zLen_best, zLen_total;
  uint grain, grain_best, num; 

  uLong bloksZlen[bloks];
  uint blokweave_grain[bloks];

  zLen_total = 0;
 
  /* *** DATA FILE *** */
  /* Prep data file. (Data written out to OUTHEDR,OUTDATA) */
  FILE *fp; 
  fp = fopen(OUTDATA, "w");
  fprintf(fp, "#include \"%s\"\n", OUTHEDR);
  fprintf(fp, "const unsigned char bloksZ[] = {\n");

  uint i,blok_i;
  for (blok_i=0; blok_i<bloks; blok_i++){
    /* Frames in this blok.*/
    num = min(num_max,(frames_len-blok_i*num_max)); 
    printf("Blok %d. Frames in blok: %d\n",blok_i,num);
    printf("Frames [%d..%d]\n",blok_i*num_max,blok_i*num_max+num-1); /* Integer interval. */
    
    
    /* Find best grain for weave. */
    /* Brute force optimization: Cycle through all divisors of period. */
    
    if (num==1){
      zLen = frame_size*num; 
      compress(blokweaveZ, &zLen, (Byte *)frames[blok_i*num_max], frame_size);
      printf("Single frame compressed. Compressed size: %ld B\n",zLen);
      grain_best = frame_size;
      zLen_best = zLen;
    }else{
      for (i=0; i<divs.l; i++){
        grain = *(divs.a+i);
        weave(blokweave, (Byte *)frames[blok_i*num_max], grain, frame_size, num);
        /* Compress the weave */
        zLen = frame_size*num; 
        compress(blokweaveZ, &zLen, (Bytef*)blokweave, frame_size*num);
        if (i==0 || zLen<zLen_best){
          grain_best = grain;
          zLen_best = zLen;
        }
      }
    }
    printf("Optimum grain: %d : compresses to: %ld B\n", grain_best, zLen_best);
    
    /* Record blok results. */
    bloksZlen[blok_i] = zLen_best;
    blokweave_grain[blok_i] = grain_best;
    zLen_total += zLen_best;

    /* Reweave on best grain and recompress. */
    weave(blokweave, (Byte *)frames[blok_i*num_max], grain_best, frame_size, num);
    zLen = frame_size*num; 
    compress(blokweaveZ, &zLen, (Bytef*)blokweave, frame_size*num);
    
    /* Write out data immediately to file. dataZ length array written out afterwards. */
    /* Cycle through compressed data array. */
    for (i=0; i<zLen; i++){
      fprintf(fp, "%#04X,",*(blokweaveZ+i));
      if ((i+1)%PRINTWIDTH==0 || (i+1)==zLen)
        fprintf(fp, "\n");
    } 
    fprintf(fp,"\n"); /* Mark boundary between bloks in ascii files. */
  }
  fprintf(fp,"};\n");
  
  /* Free memory */
  free(blokweave); 
  free(blokweaveZ);
  
  /* Write to file data lengths and weave grains of compressed bloks. */
  /* Data lengths. */
  fprintf(fp, "const unsigned long bloksZlen[%d] = {\n", bloks);   
  for (i=0; i<bloks; i++)    
    fprintf(fp, "%ld,\n", bloksZlen[i]);
  fprintf(fp, "};\n");
  
  /* Weave grains. */
  fprintf(fp, "const unsigned int blokweave_grain[%d] = {\n", bloks);   
  for (i=0; i<bloks; i++)    
    fprintf(fp, "%d,\n", blokweave_grain[i]);
  fprintf(fp, "};\n");
  fclose(fp);
  
  
  /* *** HEADER FILE *** */
  
  fp = fopen(OUTHEDR, "w");
  fprintf(fp, "#ifndef %s\n", OUTHEDRPRE);
  fprintf(fp, "#define %s\n\n", OUTHEDRPRE);
  fprintf(fp, "#define FRAME_WIDTH %d\n",FRAME_WIDTH);
  fprintf(fp, "#define FRAME_HEIGHT %d\n",FRAME_HEIGHT);
  fprintf(fp, "#define FRAME_BITDEPTH %d\n",FRAME_BITDEPTH);
  fprintf(fp, "#define NUM_MAX %d\n",num_max);
  fprintf(fp, "extern const unsigned char bloksZ[%ld];\n", zLen_total);
  fprintf(fp, "extern const unsigned long bloksZlen[%d];\n", bloks);   
  fprintf(fp, "extern const unsigned int blokweave_grain[%d];\n", bloks);   
  fprintf(fp, "#endif\n");
  fclose(fp);
 
  printf("Blokweave compression complete. Total size: %ld.\n", zLen_total);
  return 0;
} 
