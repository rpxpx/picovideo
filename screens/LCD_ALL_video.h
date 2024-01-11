#ifndef _LCD_ALL_VIDEO_H_
#define _LCD_ALL_VIDEO_H_ 

// SDK libraries
#include "pico/stdlib.h"
#include "hardware/spi.h"

// Pico LCD libraries
#include "GUI_Paint.h"

// Compression library
#include "zlib.h"

// Data 
#include "ImageDataRaw.h"
#include "ImageDataZ.h"
#include "ImageDataBlokZ.h"

// Frame rate & overclock switch
#include "Framerate.h"


#define Font12_WIDTH 7
#define Font12_HEIGHT 12


// Function declarations
// Each type of screen needs a sperate function defined in its own file 
int LCD_1in8_video(void);

 
#endif
