
#include "LCD_tools_ALL.h"


void Paint_DrawPolygon(UWORD sides, UWORD rad, UWORD x, UWORD y, float offset, UWORD color, DOT_PIXEL line_width, LINE_STYLE line_style){
  float a;
  int sx, sy, ex, ey;
  float theta = 2*PI/sides; /* Angle increment. */

  a = sides*theta + offset;
  sx = round(x - rad*sin(a));
  sy = round(y - rad*cos(a));
  while (sides--){
    a = sides*theta + offset;
    ex = round(x - rad*sin(a));
    ey = round(y - rad*cos(a));
    Paint_DrawLine(sx, sy, ex, ey, color, line_width, line_style);
    sx = ex;
    sy = ey;
  }
  return;
}


void Paint_InflatePolygon(void (*LCD_Display)(UWORD *), UWORD *BlackImage, UWORD sides, UWORD rad, UWORD x, UWORD y, float offset, UWORD delay, UWORD color, UWORD color_bg, DOT_PIXEL line_width, LINE_STYLE line_style){
  uint s;
  for (s=2; s < sides; s++){
    Paint_DrawPolygon(s,rad,x,y,offset,color,line_width,line_style);
    LCD_Display(BlackImage);
    DEV_Delay_ms(delay);
    Paint_DrawPolygon(s,rad,x,y,offset,color_bg,line_width,line_style);
    LCD_Display(BlackImage);
  }
  Paint_DrawPolygon(sides,rad,x,y,offset,color,line_width,line_style);
  LCD_Display(BlackImage);
  return;
}


void Paint_DrawImageWeave(Byte *weave, UWORD xStart, UWORD yStart, UWORD W_Image, UWORD H_Image, uint n, uint grain, uint period, uint num){
  /* uint bpp = 2;  /* Bytes per pixel */
  /* Byte pixel[bpp]; */
  /* Still work in progress; */
  /* Assume bitdepth 16: 2 bytes per pixel. */
  
  int i,j,k; 
  int frame_i;
  int weaveset1, weaveset2;
  for (j=0; j < H_Image; j++){
    for (i=0; i < W_Image; i++){
      if (xStart+i < Paint.WidthMemory  &&  yStart+j < Paint.HeightMemory){ /* Exceeded part does not display */
        frame_i = 2*(j*W_Image+i); 
        weaveset1 = (frame_i)/grain;
        weaveset2 = (frame_i+1)/grain;
	Paint_SetPixel(xStart+i, yStart+j, (*(weave + weaveset2*grain*num + n*grain + (frame_i+1)%grain)<<8 | (*(weave + weaveset1*grain*num + n*grain + (frame_i)%grain))));
      }
    }
  } 
}


