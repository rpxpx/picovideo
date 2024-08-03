#ifndef _LCD_VIDEO_ALL_H_
#define _LCD_VIDEO_ALL_H_ 


/* Preparing data text output for screen.*/
#include <stdio.h>  /* sprintf() */
#include <string.h> /* strlen() */
/* Timing decompr & render operations for framerate.*/
#include "pico/time.h" /* get_absolute_timei(), absolute_time_diff_usi(). */


/* SDK libraries */
#include "pico/stdlib.h"
#include "hardware/spi.h"

/* Pico LCD libraries */
#include "GUI_Paint.h"

/* zlib compression, rp2xx version */
#include "zlib.h"

/* Data */
#include "ImageDataRaw.h"
#include "ImageDataZ.h"
#include "ImageDataBlokZ.h"

/* Frame rate and overclock switch */
#include "Framerate.h"


#define Font12_WIDTH 7
#define Font12_HEIGHT 12


/* Function declarations */
/* Each type of screen needs a sperate function defined in its own file. */
int LCD_video_1in8(void);
 
#endif
