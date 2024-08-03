
#include <stdio.h>
#include <stdlib.h>
/* #include "primed.h" */
#include "weave.h"


int main(void){
  Byte blok[] = "abcdefABCDEF123456";
  int period = 6;
  int num = 3;

  /* Test weave() */
  Byte *blokweave;
  blokweave = (Byte *)malloc(sizeof(Byte)*period*num + 1);
  *(blokweave+period*num) = '\0'; /* terminating null for simple output with printf() */

  
  /* /\* Detemine all possible grain sizes from given period *\/ */
  /* /\* Uses types and functions defined in primed.h, libprimed.a *\/ */
  /* inta grns = divizors(period); */
  /* sort_inta(grns); */
  /* print_inta(grns); */
  /* int *grains = grns.a; */
  /* int grains_len = grns.l; */
  
  int grains[] = {1,2,3,6};
  int grains_len = 4;
  
  
  int i;
  
  printf("\nInitial blok: %s\n\n", blok);
  printf("test weave()\n");
  for (i=0; i<grains_len; i++){
    weave(blokweave, blok, grains[i], period, num);
    printf("blokweave, grain %d:\t%s\n", grains[i], blokweave);
  }  
  printf("\n");

  printf("test unweave()\n");
  int grain = 2;
  weave(blokweave, blok, grain, period, num);
  printf("blokweave, grain %d: %s\n", grain, blokweave);
  Byte *blokunweave = (Byte *)malloc(sizeof(Byte)*period*num + 1);
  int j;
  for (j=num; j; j--){
    for (i=j; i; i--){
      unweave(blokunweave, (blokweave + (num-j)*grain), grain, period, num, i);
      *(blokunweave + i*period) = '\0';
      printf("unwoven %d frame%s from start %c:\t%s\n",i,(i>1)?("s"):(" "),*(blokweave+(num-j)*grain), blokunweave);
    }
  }
  printf("\n");
  
  printf("test unweave_one()\n");
  for (i=0; i<period; i++){
    printf("byte %d of each frame:", i);
    for (j=0; j<num; j++){
      printf("\t%c", unweave_one(blokweave, i, j, grain, period, num));
    }
    printf("\n");
  }
  printf("\n");
  return 0;
}
