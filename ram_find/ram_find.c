/*
 * Determine size of available free SRAM on Pico.  
 * Compile separately. 
 * cd build && cmake .. && make
 *
 */

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
// Pico LCD libraries
#include "GUI_Paint.h"
#include "LCD_1in8.h" // 1in8-specific render functions: LCD_1IN8_Init(), LCD_1IN8_Display().

#define LCD_1IN8_BITDEPTH 16 //RGB565


ulong ram_find(void (*LCD_Display)(UWORD *), UWORD *BlackImage, ulong start, uint unit); 


ulong ram_find(void (*LCD_Display)(UWORD *), UWORD *BlackImage, ulong start, uint unit){ 
  /*
  * Looop malloc()ing unit bytes of mem and filling with 1s, until error.
  */
  void *mem;
  uint i;
  char outstr[10];

  if (start>0){
    mem = (void *)malloc(start);
    if (mem)
      memset(mem,1,start);
    else
      return 0;
  }

  for (i=0;;){
    mem = (void *)malloc(unit);
    if (!mem || !memset(mem,1,unit))
      return i;
    i++;
//    printf("filled %ld\n",i*unit+start);
    sprintf(outstr,"%d",i*unit+start);  
    Paint_DrawString_EN(1, 1, outstr, &Font12, BLACK, WHITE);
    LCD_Display(BlackImage);
  }
}



void main(){

  if(DEV_Module_Init()!=0){
    return;
  }
  // Initialize SPK
  stdio_init_all();
  setup_default_uart();
  // LCD Init 
  LCD_1IN8_Init(HORIZONTAL);
  // Create image memory space.
  UDOUBLE Imagesize = LCD_1IN8.HEIGHT * LCD_1IN8.WIDTH * LCD_1IN8_BITDEPTH/8;
  UWORD *BlackImage;
  BlackImage = (UWORD *)malloc(Imagesize); 
  if (BlackImage==NULL){
      printf("Failed to apply for black memory.\r\n");
      exit(0);
  }
  // Create a new image cache.
  Paint_NewImage((UBYTE *)BlackImage, LCD_1IN8.WIDTH, LCD_1IN8.HEIGHT, 0, WHITE);
  Paint_SetScale(65);
  Paint_SetRotate(ROTATE_0);

  // Determine free SRAM.
  ram_find(&LCD_1IN8_Display, BlackImage, 0, 1000);
  
  //ulong f = ram_find2(210000, 1, BlackImage);
  //Module Exit 
  free(BlackImage);
  BlackImage = NULL;
  DEV_Module_Exit();
  return;
}

