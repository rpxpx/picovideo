
/* Read in raw byte array for pico LCD images. Compress with zlib. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <zlib.h> /* compress(); sytem zlib, not local rp2xx */

/* Raw byte array set of images */
#include "ImageDataRaw.h"


#define OUTHEDR "ImageDataZ.h"
#define OUTDATA "ImageDataZ.c"
#define OUTHEDRPRE "_IMAGEDATAZ_H_"
#define PRINTWIDTH 16  /* 16 bytes per line */


#define CHECK_ERR(err, msg){\
  if (err!=Z_OK){\
    fprintf(stderr, "%s error: %d\n", msg, err);\
    exit(1);\
  }\
}

int main(void){

  uLong frame_size = FRAME_WIDTH * FRAME_HEIGHT * FRAME_BITDEPTH/8;
  int frame_count = sizeof(frames)/frame_size; 
  printf("data size raw: %d frames of %ldB = %ldB\n", frame_count, (long)frame_size, (long)sizeof(frames));

  /* Create a buffer for compressed data */
  Byte *compr;

  /* zlib guarantees that compressed data will be <= raw data */
  /* comprLen is max possible size of compressed data */
  compr = (Byte *)malloc(frame_size);
  uLong comprLen; /* set to frame_size before each call to compress() */

  /* Array for compressed frame lengths */
  uLong framesZlen[frame_count];

  /* Write out all compressed data to file into a 1d array. */
  /* Store data lengths of each compressed frame in an array and write that out afterwards to file. */
  FILE *fp;
  fp = fopen(OUTDATA, "w");
  fprintf(fp, "#include \"%s\"\n", OUTHEDR);
  fprintf(fp, "const unsigned char framesZ[] = {\n");
  
  /* Cycle through frames */
  int i,j;
  int err;
  for (i = 0;i<frame_count;i++){
    comprLen = frame_size;
    err = compress(compr, &comprLen, (const Bytef*)frames[i], frame_size);
    CHECK_ERR(err, "compress");

    printf("frame %d: data size compressed: %ld\n", i, (long)comprLen);    
    
    /* Store dataZ length in array */
    framesZlen[i] = comprLen;

    /* Write out data immediately to file. dataZ length array written out afterwards. */
    /* Cycle through compressed data array */
    for (j = 0; j<comprLen; j++){
      fprintf(fp, "%#04X,",compr[j]);
      if ((j+1)%PRINTWIDTH==0 || (j+1)==comprLen)
        fprintf(fp, "\n");
    } 
    fprintf(fp,"\n"); /* testing: mark boundary between frames in ascii files */
  } 
  fprintf(fp,"};\n");
  
  /* Data lengths of compressed frames */
  fprintf(fp, "const unsigned long framesZlen[%d] = {\n", frame_count);   
  for (i = 0; i<frame_count; i++)    
    fprintf(fp, "%ld,\n", framesZlen[i]);
  fprintf(fp, "};\n");
  fclose(fp);  
  
  /* Sum array of lengths to get total data size */
  uLong framesZ_size = 0;
  for (i = 0; i<frame_count; i++)
    framesZ_size+=framesZlen[i];

  /* HEADER FILE */
  fp = fopen(OUTHEDR, "w");
  fprintf(fp, "#ifndef %s\n", OUTHEDRPRE);
  fprintf(fp, "#define %s\n\n", OUTHEDRPRE);
  fprintf(fp, "#define FRAME_WIDTH %d\n",160);
  fprintf(fp, "#define FRAME_HEIGHT %d\n",130);
  fprintf(fp, "#define FRAME_BITDEPTH %d\n",16);
  fprintf(fp, "extern const unsigned char framesZ[%ld];\n", framesZ_size);
  fprintf(fp, "extern const unsigned long framesZlen[%d];\n", frame_count);   
  fprintf(fp, "#endif\n");
  fclose(fp);
 
  printf("* Single frame compression complete.\n");
  return 0;
} 
