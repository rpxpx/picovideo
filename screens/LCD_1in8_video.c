
#include "LCD_ALL_video.h" // Generic header for LCD_*_video.c files.
#include "LCD_1in8.h" // 1in8-specific render functions: LCD_1IN8_Init(), LCD_1IN8_Display().

#include "LCD_ALL_tools.h" // New addtional paint/draw functions.

#include <stdio.h>  // sprintf(), preparing data text output for screen.
#include <string.h> // strlen() for text display.
#include "pico/time.h" // get_absolute_timei(), absolute_time_diff_usi(). Timing decompr & render operations for framerate.


/*
* data types
*
* In: Pico_LCD_code/c/lib/Config/DEV_Config.h :
* UBYTE   uint8_t   255
* UWORD   uint16_t  65534
* ULONG   uint32_t  4294967295
*/

#define LCD_1IN8_BITDEPTH 16 //RGB565

// LCD_1IN8.WIDTH, LCD_1IN8.HEIGHT
// These symbolic constants are defined in LCD_1in8.h
// WIDTH 130, HEIGHT 160
// are swapped during the HORIZONTAL call to LCD_1IN8_Init()

#define SPLASH_DELAY 100
#define SPLASH_RAD 55
#define SPLASH_Y 60
#define SPLASH_READY 500
#define DATA_DISP_DELAY 5000


//bool reserved_addr(uint8_t addr) {
//  return (addr & 0x78) == 0 || (addr & 0x78) == 0x78;
//}

//#define CHECK_ERR(err, msg){\
//  if (err!=Z_OK){\
//    fprintf(stderr, "%s error: %d\n", msg, err);\
//    exit(1);\
//  }\
//}

int LCD_1in8_video(void){

  if(DEV_Module_Init()!=0){
    return -1;
  }
    
  // Initialize SPK
  stdio_init_all();
  setup_default_uart();

  // LCD Init 
  LCD_1IN8_Init(HORIZONTAL);
  //LCD_1IN8_SetBacklight(1023);

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


  // Splash screen animation.
  DEV_Delay_ms(SPLASH_DELAY); // Exhibit some randomness.
  // Black background.
  Paint_DrawRectangle(1,1, LCD_1IN8.WIDTH, LCD_1IN8.HEIGHT, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL); // Paint it black.
  LCD_1IN8_Display(BlackImage);

  Paint_InflatePolygon(&LCD_1IN8_Display,BlackImage,6,SPLASH_RAD,80,SPLASH_Y,PI,2*SPLASH_DELAY,WHITE,BLACK,DOT_PIXEL_1X1,DRAW_FILL_EMPTY);
  Paint_InflatePolygon(&LCD_1IN8_Display,BlackImage,3,SPLASH_RAD,80,SPLASH_Y,PI,2*SPLASH_DELAY,WHITE,BLACK,DOT_PIXEL_1X1,DRAW_FILL_EMPTY);
  Paint_DrawPolygon(6,SPLASH_RAD,80,SPLASH_Y,PI,WHITE,DOT_PIXEL_1X1,DRAW_FILL_EMPTY);
  LCD_1IN8_Display(BlackImage);

  //char logotext[] = "rpx@clearlight.systems";
  //char logotext[] = "JASNASVETLOST SISTEMI";
  char logotext[] = "clearlight.systems";
  int tab = ((LCD_1IN8.WIDTH+1) - strlen(logotext)*Font12_WIDTH)/2; 
  Paint_DrawString_EN(tab, LCD_1IN8.HEIGHT-Font12_HEIGHT, logotext, &Font12, BLACK, RED);
  LCD_1IN8_Display(BlackImage);

  // 23 deciseconds' delay
  DEV_Delay_ms(2300);

  // Raw data framesize from constants in data .h
  uLong frame_size = FRAME_WIDTH * FRAME_HEIGHT * FRAME_BITDEPTH/8;

  // On first pass through main blok loop, calculate mean vals for decompression and/or render times.
  absolute_time_t start, end;
  uint frames_total = 0;
  float delay_ms = -1;

 
  // Some preprocessor directives for conditional compilation. 
  // Compile decompression and rendering code according to data header files present.
  // 1. _IMAGEDATABLOKZ_H_   blokweave compression
  // 2. _IMAGEDATAZ_H_       simple, single frame compression      
  // 3. _IMAGEDATARAW_H_     uncompressed frames


  // *** 1. BLOKWEAVE ***
  #if defined _IMAGEDATABLOKZ_H_

  // Total number of bloks.
  uint bloks = sizeof(bloksZlen)/sizeof(bloksZlen[0]);

  // Mem size (B) of blok.
  uLong bloklen = frame_size*(NUM_MAX);

  // Buffer to uncompress blok to.
  Byte *blok = (Byte *)malloc(bloklen);  // No error-checking: malloc() on Pico fails if unsuccessful; does not return 0.

  uint bloks_i,frames_i;
  const unsigned char *bloksZp = bloksZ;
  uint num;
 
  // Time decompression and rendering to get true framerate.
  int64_t decompr_us = 0;
  int64_t imgweav_us = 0;


  bloks_i = 0;
  for (;;){ // Loop through bloks.
    if (delay_ms<0) // If delay_ms negative, this is first pass through bloks: calculate necessary delay from decompression & render times.
      start = get_absolute_time();

    bloklen = frame_size*(NUM_MAX); 
    uncompress(blok, &bloklen, bloksZp, bloksZlen[bloks_i]); // No error-checking: uncompress() on Pico fails if unsuccessful; does not return 0.
    num = bloklen/frame_size; // Number of frames in this current blok.

    if (delay_ms<0){
      end = get_absolute_time(); 
      decompr_us += absolute_time_diff_us(start,end);
      frames_total += num;
    }

    for (frames_i=0; frames_i<num; frames_i++){
      if (delay_ms<0)
        start = get_absolute_time();
      else{
        if (frames_i==0){
          if (delay_ms>0)
            DEV_Delay_ms(delay_ms);
        }else{
          DEV_Delay_ms(delay_ms+(decompr_us/1000.0));
        }
      }

      // Draw image
      Paint_DrawImageWeave((Byte*)blok, 0, 0, LCD_1IN8.WIDTH, LCD_1IN8.HEIGHT, frames_i, blokweave_grain[bloks_i], frame_size, num); //grain, period, num
      LCD_1IN8_Display(BlackImage);

      if (delay_ms<0){
        end = get_absolute_time();
        imgweav_us += absolute_time_diff_us(start,end);
      }
    }
    
    if (bloks_i==bloks-1){ // Completed last blok.
      bloksZp = bloksZ;
      bloks_i = 0;
      if (delay_ms<0){
        // Convert sums to average.
        decompr_us = round(decompr_us/(bloks*1.0));
        imgweav_us = round(imgweav_us/(frames_total*1.0));
        delay_ms = 1000.0/FRAMERATE - decompr_us/1000.0 - imgweav_us/1000.0; // Conv delay times from us to ms. FRAMERATE specified in fps.
        (delay_ms<0) && (delay_ms = 0);

        //Paint_DrawRectangle(1,1, LCD_1IN8.WIDTH, LCD_1IN8.HEIGHT, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
        //LCD_1IN8_Display(BlackImage);

        char *outs[5];
        for (int i=0;i<5;i++)
          outs[i] = (char *)malloc(23);
        sprintf(outs[0],"decompr t:%ld mcs",decompr_us); 
        sprintf(outs[1],"wvrendr t:%ld mcs",imgweav_us); 
        sprintf(outs[2],"MAX F.RATE: %.2f fps",pow(10,6)/(decompr_us+imgweav_us)); 
        sprintf(outs[3],"selected: %d", FRAMERATE);
        sprintf(outs[4],"fr delay: %.3f ms",delay_ms); 
        for (int i=0;i<5;i++){
          Paint_DrawString_EN(1, 16*(i+1), outs[i], &Font12, BLACK, WHITE);
          free(outs[i]);
        }
        LCD_1IN8_Display(BlackImage);
        DEV_Delay_ms(DATA_DISP_DELAY);
      }
    }else{
      bloksZp += bloksZlen[bloks_i]; 
      bloks_i++;
    }
  }

  
  // *** 2. SINGL *** single frame compression.
  #elif defined _IMAGEDATAZ_H_

  // Create decompress buffer
  Byte *frame = (Byte *)malloc(frame_size);

  frames_total = sizeof(framesZlen)/sizeof(uLong); // Total number of frames.
  
  // If there is only one frame. No looping, no delay.
  if (frames_total==1){
    uncompress(frame, &frame_size, framesZ, framesZlen[0]); // No error-checking: uncompress() on Pico fails if unsuccessful; does not return 0.
    Paint_DrawImage((char*)frame, 1, 1, LCD_1IN8.WIDTH, LCD_1IN8.HEIGHT);
    LCD_1IN8_Display(BlackImage);

  }else{
    // Time (microsecs) decompression and rendering to determine max framerate. 
    int64_t render_us = 0;
    
    // Copy the data pointer
    const unsigned char *framesZp = framesZ;
    uint frames_i = 0;

    for (;;){
      // If 
      if (delay_ms<0){
        start = get_absolute_time();
      }

      uncompress(frame, &frame_size, framesZp, framesZlen[frames_i]);
      Paint_DrawImage((char*)frame, 1, 1, LCD_1IN8.WIDTH, LCD_1IN8.HEIGHT);
      LCD_1IN8_Display(BlackImage);
      
      if (delay_ms<0){
        end = get_absolute_time();
        render_us += absolute_time_diff_us(start,end);  
      }else if(delay_ms>0){
        DEV_Delay_ms(delay_ms);
      }

      if (frames_i<(frames_total-1)){
        framesZp += framesZlen[frames_i++];
      }else{
        frames_i = 0; 
        framesZp = framesZ;
        if (delay_ms<0){
          render_us = round(render_us/(frames_total*1.0));
          delay_ms = 1000.0/FRAMERATE - render_us/1000.0;
          (delay_ms<0) && (delay_ms = 0);

//          Paint_DrawRectangle(1,1, LCD_1IN8.WIDTH, LCD_1IN8.HEIGHT, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
//          LCD_1IN8_Display(BlackImage);

          char *outs[4];
          for (int i=0;i<4;i++)
            outs[i] = (char *)malloc(23);
          sprintf(outs[0],"render t:%ld mcs",render_us); 
          sprintf(outs[1],"MAX F.RATE: %.2f fps",pow(10,6)/(render_us)); 
          sprintf(outs[2],"selected: %d",FRAMERATE); 
          sprintf(outs[3],"fr delay: %0.3f ms",delay_ms); 
          for (int i=0;i<4;i++){
            Paint_DrawString_EN(1, 16*(i+1), outs[i], &Font12, BLACK, WHITE);
            free(outs[i]);
          }
          LCD_1IN8_Display(BlackImage);
          DEV_Delay_ms(DATA_DISP_DELAY);
        }
      }
    }
  }


  // *** 3. RAW *** uncompressed frames.
  #else

  frames_total = sizeof(frames)/frame_size; // Total number of frames.

  if (frames_total==1){
    Paint_DrawImage(frames[0],1,1, LCD_1IN8.WIDTH, LCD_1IN8.HEIGHT);
    LCD_1IN8_Display(BlackImage);
  }else{ // LOOP VIDEO
    // Determine render t first for delay.
    start = get_absolute_time();
    Paint_DrawImage(frames[0],1,1, LCD_1IN8.WIDTH, LCD_1IN8.HEIGHT);
    LCD_1IN8_Display(BlackImage);
    end = get_absolute_time();
    int64_t render_us = absolute_time_diff_us(start,end);
    delay_ms = 1000.0/FRAMERATE - render_us/1000.0;
    (delay_ms<0) && (delay_ms = 0);

    char *outs[4];
    for (int i=0;i<4;i++)
      outs[i] = (char *)malloc(23);
      sprintf(outs[0],"render t:%ld mcs",render_us); 
      sprintf(outs[1],"MAX F.RATE: %.2f fps",pow(10,6)/(render_us)); 
      sprintf(outs[2],"selected: %d",FRAMERATE); 
      sprintf(outs[3],"fr delay: %0.3f ms",delay_ms); 
      for (int i=0;i<4;i++){
        Paint_DrawString_EN(1, 16*(i+1), outs[i], &Font12, BLACK, WHITE);
        free(outs[i]);
      }
    LCD_1IN8_Display(BlackImage);
    DEV_Delay_ms(DATA_DISP_DELAY);
    
    uint frames_i = 0;
    for (;;){
      Paint_DrawImage(frames[frames_i],1,1, LCD_1IN8.WIDTH, LCD_1IN8.HEIGHT);
      LCD_1IN8_Display(BlackImage);
    
    if (delay_ms>0)
      DEV_Delay_ms(delay_ms);
    frames_i = (frames_i+1)%frames_total;
    }
  }
  #endif

  
  // Module Exit 
  free(BlackImage);
  BlackImage = NULL;
  DEV_Module_Exit();
  return 0;
}
