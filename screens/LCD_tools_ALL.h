
#ifndef _LCD_TOOLS_ALL_H_
#define _LCD_TOOLS_ALL_H_

#include <math.h>
#include <zlib.h> /* Defn of Byte type used in weave functions. */

#include "GUI_Paint.h"


#define PI 3.141592653589793
/* #define UWORD uint16_t */

void Paint_DrawPolygon(UWORD sides, UWORD rad, UWORD x, UWORD y, float offset, UWORD color, DOT_PIXEL line_width, LINE_STYLE line_style);
void Paint_InflatePolygon(void (*LCD_Display)(UWORD *), UWORD *BlackImage, UWORD rad, UWORD sides, UWORD x, UWORD y, float offset, UWORD delay, UWORD color, UWORD color_bg, DOT_PIXEL line_width, LINE_STYLE line_style);
void Paint_DrawImageWeave(Byte *weave, UWORD xStart, UWORD yStart, UWORD W_Image, UWORD H_Image, uint n, uint grain, uint period, uint num);

#endif

