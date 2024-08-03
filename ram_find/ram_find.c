
/* Determine size of available free SRAM on Pico. */

#include <stdio.h>
#include <string.h>

#include "pico/stdlib.h"


/* Pico LCD libraries */
#include "GUI_Paint.h"
/* 1in8-specific render functions: */
/* LCD_1IN8_Init(), LCD_1IN8_Display() */
#include "LCD_1in8.h" 

#define LCD_1IN8_BITDEPTH 16 /* RGB565 */


/* Loop malloc()ing unit bytes of mem and filling with 1s, until error. */
ulong ram_find(void (*LCD_Display)(UWORD *), UWORD *BlackImage, ulong start, uint unit); 

ulong ram_find(void (*LCD_Display)(UWORD *), UWORD *BlackImage, ulong start, uint unit){   
  void *mem;
  if (start>0){
    mem = (void *)malloc(start);
    if (mem)
      memset(mem,1,start);
    else
      return 0;
  }
  uint i;
  char outstr[10];
  for (i=0;;){
    mem = (void *)malloc(unit);
    if (!mem || !memset(mem, 1, unit))
      return i;
    i++;
    sprintf(outstr, "%d", i*unit+start);  
    Paint_DrawString_EN(1, 1, outstr, &Font12, BLACK, WHITE);
    LCD_Display(BlackImage);
  }
}


void main(void){
  if (DEV_Module_Init()){
    return;
  }
  
  /* Initialize SDK */
  stdio_init_all();
  setup_default_uart();
  
  /* LCD Init */
  LCD_1IN8_Init(HORIZONTAL);
  
  /* Create image memory space.*/
  UDOUBLE Imagesize = LCD_1IN8.HEIGHT * LCD_1IN8.WIDTH * LCD_1IN8_BITDEPTH/8;
  UWORD *BlackImage;
  BlackImage = (UWORD *)malloc(Imagesize); 
  if (BlackImage==NULL){
      printf("Failed to apply for black memory.\r\n");
      exit(0);
  }
  
  /* Create a new image cache */
  Paint_NewImage((UBYTE *)BlackImage, LCD_1IN8.WIDTH, LCD_1IN8.HEIGHT, 0, WHITE);
  Paint_SetScale(65);
  Paint_SetRotate(ROTATE_0);

  /* Determine free SRAM. */
  ram_find(&LCD_1IN8_Display, BlackImage, 0, 1000);
  
  free(BlackImage);
  BlackImage = NULL;
  DEV_Module_Exit();
  return;
}
