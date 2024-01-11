
#include "LCD_ALL_tools.h"


// Can speed this up: calcs x,y twice, as end point and start point.
void Paint_DrawPolygon(UWORD sides, UWORD rad, UWORD x, UWORD y, float offset, UWORD color, DOT_PIXEL line_width, LINE_STYLE line_style){
  float a1,a2;
  float theta = 2*PI/sides; // Angle increment.
  for (;sides>0; sides--){
    a1 = sides*theta+offset;
    a2 = (sides-1)*theta+offset;
    Paint_DrawLine(round(x-rad*sin(a1)), round(y-rad*cos(a1)), round(x-rad*sin(a2)), round(y-rad*cos(a2)), color, line_width, line_style); 
  }
  return;
}

void Paint_InflatePolygon(void (*LCD_Display)(UWORD *), UWORD *BlackImage, UWORD sides, UWORD rad, UWORD x, UWORD y, float offset, UWORD delay, UWORD color, UWORD color_bg, DOT_PIXEL line_width, LINE_STYLE line_style){
  uint s;
  for (s=2; s<sides; s++){
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
  // Assumes bitdepth 16: 2 bytes per pixel.
  //
//  uint bpp = 2; // Bytes per pixel    
//  Byte pixel[bpp];
  int i,j,k; 
  int frame_i;
  int weaveset1, weaveset2;
	for(j = 0; j < H_Image; j++){
  	for(i = 0; i < W_Image; i++){
			if(xStart+i < Paint.WidthMemory  &&  yStart+j < Paint.HeightMemory){//Exceeded part does not display
        frame_i = 2*(j*W_Image+i); // frame_i = bpp*(2*W_Image+i);
        weaveset1 = (frame_i)/grain;
//        pixel[0] = *(weave + set*grain*num + n*grain + (byt)%grain);
        weaveset2 = (frame_i+1)/grain;
//        pixel[1] = *(weave + set*grain*num + n*grain + (byt+1)%grain);
				Paint_SetPixel(xStart+i, yStart+j, (*(weave + weaveset2*grain*num + n*grain + (frame_i+1)%grain)<<8 | (*(weave + weaveset1*grain*num + n*grain + (frame_i)%grain))));
				//Using arrays is a property of sequential storage, accessing the original array by algorithm
				//j*W_Image*2 			   Y offset
				//i*2              	   X offset
      }
		}
	} 
}


