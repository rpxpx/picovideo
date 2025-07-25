
#include "LCD_video_ALL.h" /* Generic header for all LCD_*_video.c files. */
#include "LCD_tools_ALL.h" /* New addtional paint/draw functions. */

#include "LCD_1in8.h" /* 1in8-specific render functions from PICO_LCD library: LCD_1IN8_Init(), LCD_1IN8_Display().*/

/* Data types
 *
 * In: Pico_LCD_code/c/lib/Config/DEV_Config.h :
 * UBYTE   uint8_t   255
 * UWORD   uint16_t  65534
 * ULONG   uint32_t  4294967295
 */

#define LCD_1IN8_BITDEPTH 16 /* RGB565 */

/* LCD_1IN8.WIDTH, LCD_1IN8.HEIGHT */
/* These symbolic constants are defined in LCD_1in8.h */
/* WIDTH 128, HEIGHT 160 */
/* are swapped during the HORIZONTAL call to LCD_1IN8_Init() */

#define SPLASH_DELAY_MS 100
#define SPLASH_RAD 55
#define SPLASH_Y 60
#define SPLASH_READY 500
#define DATA_DISP_DELAY_US 8000000


/* bool reserved_addr(uint8_t addr) { */
/*   return (addr & 0x78) == 0 || (addr & 0x78) == 0x78; */
/* } */

#define CHECK_ERR(err, msg){\
  if (err!=Z_OK){\
    fprintf(stderr, "%s error: %d\n", msg, err);\
    exit(1);\
  }\
}


int LCD_video_1in8(void){

  if(DEV_Module_Init()!=0){
    return -1;
  }
    
  /* Initialize SDK */
  stdio_init_all();
  setup_default_uart();

  /* Initialize LCD */
  LCD_1IN8_Init(HORIZONTAL);
  /* LCD_1IN8_SetBacklight(1023); */

  /* Create image memory space. */
  UDOUBLE Imagesize = LCD_1IN8.HEIGHT * LCD_1IN8.WIDTH * LCD_1IN8_BITDEPTH/8;
  UWORD *BlackImage;
  BlackImage = (UWORD *)malloc(Imagesize); 
  if (BlackImage==NULL){
      printf("Failed to apply for black memory.\r\n");
      exit(0);
  }
  
  /* Create a new image cache. */
  Paint_NewImage((UBYTE *)BlackImage, LCD_1IN8.WIDTH, LCD_1IN8.HEIGHT, 0, WHITE);
  Paint_SetScale(65);
  Paint_SetRotate(ROTATE_0);
  
  /* Splash screen polygon animation: Clearlight Systems / Jasnasvetlost Sistemi */
  sleep_ms(SPLASH_DELAY_MS);
  Paint_DrawRectangle(1,1, LCD_1IN8.WIDTH, LCD_1IN8.HEIGHT, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL); /* Paint it black.*/
  LCD_1IN8_Display(BlackImage);
  
  Paint_InflatePolygon(&LCD_1IN8_Display,BlackImage,6,SPLASH_RAD,80,SPLASH_Y,PI,2*SPLASH_DELAY_MS,WHITE,BLACK,DOT_PIXEL_1X1,DRAW_FILL_EMPTY);
  Paint_InflatePolygon(&LCD_1IN8_Display,BlackImage,3,SPLASH_RAD,80,SPLASH_Y,PI,2*SPLASH_DELAY_MS,WHITE,BLACK,DOT_PIXEL_1X1,DRAW_FILL_EMPTY);
  Paint_DrawPolygon(6,SPLASH_RAD,80,SPLASH_Y,PI,WHITE,DOT_PIXEL_1X1,DRAW_FILL_EMPTY);
  LCD_1IN8_Display(BlackImage);

  /*char logotext[] = "rpx@clearlight.systems";*/
  /*char logotext[] = "JASNASVETLOST SISTEMI";*/
  char logotext[] = "clearlight.systems";
  int tab = ((LCD_1IN8.WIDTH+1) - strlen(logotext)*Font12_WIDTH)/2; 
  Paint_DrawString_EN(tab, LCD_1IN8.HEIGHT-Font12_HEIGHT, logotext, &Font12, BLACK, RED);
  LCD_1IN8_Display(BlackImage);
  
  sleep_ms(2300);
  
  /* Raw data framesize from constants in data .h */
  uLong frame_size = FRAME_WIDTH * FRAME_HEIGHT * FRAME_BITDEPTH/8;

  /* On first pass through image render loop, calculate mean render and decompression times.*/
  absolute_time_t start, end, end_testrun;
  uint frames_total = 0;
  char firstpass = 1;
  uint64_t delay_us;

  
  /* Some preprocessor directives for conditional compilation.  */
  /* Compile decompression and rendering code according to which data header files are present. */
  /* This is determined in makevideo bash script. */
  
  /* 1. _IMAGEDATABLOKZ_H_   blokweave compression */
  /* 2. _IMAGEDATAZ_H_       simple, single frame compression */
  /* 3. _IMAGEDATARAW_H_     uncompressed frames */
  
  
  /* *** 1. BLOKWEAVE *** */
  #if defined _IMAGEDATABLOKZ_H_

  /* Total number of bloks. */
  uint bloks = sizeof(bloksZlen)/sizeof(bloksZlen[0]);

  /* Mem size (B) of blok. */
  uLong bloklen = frame_size*(NUM_MAX);
  
  /* Buffer to decompress blok to. */
  /* No error-checking: Pico malloc() fails if unsuccessful; no return value. */
  Byte *blok = (Byte *)malloc(bloklen);  

  uint bloks_i,frames_i;
  const unsigned char *bloksZp = bloksZ;
  uint num;
 
  /* Time decompression and rendering to get true framerate. */
  /* Unit: microseconds (us) */
  uint64_t decompr_us = 0;
  uint64_t imgweav_us = 0;
  uint64_t u_decompr_us;
  uint64_t u_imgweav_us;
  uint64_t delay_midblok_us;
  /* total time for testrun and data display: important if attempting to sync start of several picovideos*/
  uint64_t testrun_us; 
  
  
  /* Loop through bloks.*/
  bloks_i = 0;
  for (;;){
    /* we calculate necessary frame delay from mean decompression and render times. */
    if (firstpass) 
      start = get_absolute_time();
    
    bloklen = frame_size*(NUM_MAX);
    /* Decompress the blok.*/
    /* No error-checking: rp2xx uncompress() fails if unsuccessful: does not return err. */
    uncompress(blok, &bloklen, bloksZp, bloksZlen[bloks_i]);
    /* Number of frames in this blok. */
    num = bloklen/frame_size; 
    
    if (firstpass){
      end = get_absolute_time(); 
      decompr_us += absolute_time_diff_us(start,end);
      frames_total += num;
    }
    
    /* Loop through frames in blok. */
    for (frames_i=0; frames_i<num; frames_i++){
      if (firstpass)
        start = get_absolute_time();
      else{
        if (frames_i==0){
	  sleep_us(delay_us);
        }else{
          sleep_us(delay_midblok_us);
        }
      }
      
      /* Draw this frame. */      
      /* Each frame is picked out of the weave with four parameters: index, grain, period, number. */
      /* grain is the size (b) of each chunk */
      Paint_DrawImageWeave((Byte*)blok, 0, 0, LCD_1IN8.WIDTH, LCD_1IN8.HEIGHT, frames_i, blokweave_grain[bloks_i], frame_size, num); 
      LCD_1IN8_Display(BlackImage);
      
      if (firstpass){
        end = get_absolute_time();
        imgweav_us += absolute_time_diff_us(start, end);
      }
    }
    
    /* If this is the last blok, prepare to loop back to start. */
    if (bloks_i==bloks-1){ 
      bloksZp = bloksZ;
      bloks_i = 0;
      
      /* If this was the first pass through the bloks, determine delay. */
      if (firstpass){
        /* Convert sums to integer means. */
        u_decompr_us = round(decompr_us/(bloks*1.0));
        u_imgweav_us = round(imgweav_us/(frames_total*1.0));
	
	uint64_t requested = 1000000.0/FRAMERATE;
	/* mean render of first frame in blok: requires blok depcompression then imgweav */
	uint64_t render_us = u_decompr_us + u_imgweav_us;
	if (render_us > requested)
	  delay_us = 0;
	else
	  delay_us = requested - render_us;	
	delay_midblok_us = delay_us + u_decompr_us;
	
	
        /* Paint_DrawRectangle(1,1, LCD_1IN8.WIDTH, LCD_1IN8.HEIGHT, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);*/
        /* LCD_1IN8_Display(BlackImage);*/
	
        char *outs[5];
	uint i;
        for (i=0; i<5; i++)
          outs[i] = (char *)malloc(23);
        sprintf(outs[0],"u decompr us: %ld",u_decompr_us); 
        sprintf(outs[1],"u wvrendr us: %ld",u_imgweav_us); 
        sprintf(outs[2],"MAX F.RATE: %.6f",pow(10.0,6)/(render_us)); 
        sprintf(outs[3]," requested: %.6f", (float) FRAMERATE);
        sprintf(outs[4],"  f delay us: %ld", delay_us);
        for (i=0; i<5; i++){
          Paint_DrawString_EN(1, 16*(i+1), outs[i], &Font12, BLACK, WHITE);
          free(outs[i]);
        }
        LCD_1IN8_Display(BlackImage);
	firstpass = 0;
	//end_testrun = get_absolute_time();
        sleep_us(DATA_DISP_DELAY_US - absolute_time_diff_us(start, end_testrun));	
      }
    }else{
      bloksZp += bloksZlen[bloks_i]; 
      bloks_i++;
    }
  }
  
  
  /* *** 2. SINGL *** single frame compression */
#elif defined _IMAGEDATAZ_H_
  
  /* Create decompress buffer */
  Byte *frame = (Byte *)malloc(frame_size);
  
  frames_total = sizeof(framesZlen)/sizeof(uLong);
  
  /* If there is only one frame: no looping, no delay. */
  if (frames_total==1){
    /* No error-checking: rp2xx uncompress() fails if unsuccessful: does not return err. */
    uncompress(frame, &frame_size, framesZ, framesZlen[0]);
    Paint_DrawImage((char *)frame, 1, 1, LCD_1IN8.WIDTH, LCD_1IN8.HEIGHT);
    LCD_1IN8_Display(BlackImage);
  }else{
    /* Time (microsecs) decompression and rendering to determine max framerate. */
    uint64_t render_us = 0;  /* total for all frames */
    uint64_t u_render_us;    /* mean */
    
    /* Copy the data pointer */
    const unsigned char *framesZp = framesZ;
    uint frames_i = 0;
    
    
    for (;;){
      /* If first pass, take times to calc necess frame delays. */
      if (firstpass)
        start = get_absolute_time();
      
      uncompress(frame, &frame_size, framesZp, framesZlen[frames_i]);
      Paint_DrawImage((char*)frame, 1, 1, LCD_1IN8.WIDTH, LCD_1IN8.HEIGHT);
      LCD_1IN8_Display(BlackImage);
      
      if (firstpass){
	end = get_absolute_time();
        render_us += absolute_time_diff_us(start, end);
      }else{
        sleep_us(delay_us);
      }
      
      if (frames_i<(frames_total-1)){
        framesZp += framesZlen[frames_i++];
      }else{
        frames_i = 0; 
        framesZp = framesZ;
	
	/* If first pass */
	/* Determine mean render time; use this to calc frame render delay  */
        if (firstpass){
	  uint64_t requested = round(pow(10.0,6)/FRAMERATE);
	  u_render_us = round(render_us/(frames_total*1.0));
	  if (u_render_us > requested)
	    delay_us = 0;
	  else
	    delay_us = requested - u_render_us;
	  
          char *outs[4];
	  uint i;
          for (i=0; i<4; i++)
            outs[i] = (char *)malloc(23);
          sprintf(outs[0],"u render us: %ld", u_render_us); 
          sprintf(outs[1]," MAX F.RATE: %.6f", pow(10.0,6)/(u_render_us));
          sprintf(outs[2],"  requested: %.6f", (float) FRAMERATE);
          sprintf(outs[3],"fr delay us: %ld", delay_us);
          for (i=0; i<4; i++){
            Paint_DrawString_EN(1, 16*(i+1), outs[i], &Font12, BLACK, WHITE);
            free(outs[i]);
          }
          LCD_1IN8_Display(BlackImage);
	  firstpass = 0;

	  /* Taking absolute time again here to set a common start point */
	  /* across several frames doesn't work. First frames start staggered. */
	  /* Why...? That should be more accurate. */
	  //end_testrun = get_absolute_time();
	  //render_us = absolute_time_diff_us(start, end_testrun);
	  //render_us = absolute_time_diff_us(start, end_testrun);
          sleep_us(DATA_DISP_DELAY_US - render_us);
	}
      }
    }
  }
  
  
  /* *** 3. RAW *** uncompressed frames. */
  #else
  
  frames_total = sizeof(frames)/frame_size; 
  
  if (frames_total==1){
    Paint_DrawImage(frames[0],1,1, LCD_1IN8.WIDTH, LCD_1IN8.HEIGHT);
    LCD_1IN8_Display(BlackImage);
  }else{
    /* LOOP VIDEO */
    /* Determine render t first for delay. */
    uint i;      
    start = get_absolute_time();
    Paint_DrawImage(frames[0],1,1, LCD_1IN8.WIDTH, LCD_1IN8.HEIGHT);
    LCD_1IN8_Display(BlackImage);

    uint64_t render_us = absolute_time_diff_us(start, get_absolute_time());    
    uint64_t requested = round(pow(10.0,6)/FRAMERATE);
    
    if (render_us > requested)
      delay_us = 0;
    else
      delay_us = requested - render_us;
    
    char *outs[4];
    for (i=0; i<4; i++)
      outs[i] = (char *)malloc(23);
    sprintf(outs[0]," render us: %ld", render_us); 
    sprintf(outs[1],"MAX F.RATE: %.6f", pow(10.0,6)/(render_us)); 
    sprintf(outs[2]," requested: %.6f", (float) FRAMERATE); 
    sprintf(outs[3],"f delay us: %ld", delay_us); 
    for (i=0; i<4; i++){
      Paint_DrawString_EN(1, 16*(i+1), outs[i], &Font12, BLACK, WHITE);
      free(outs[i]);
    }
    LCD_1IN8_Display(BlackImage);
    //render_us = absolute_time_diff_us(start, get_absolute_time());
    sleep_us(DATA_DISP_DELAY_US - render_us);
    
    uint frames_i = 0;
    for (;;){
      Paint_DrawImage(frames[frames_i],1,1, LCD_1IN8.WIDTH, LCD_1IN8.HEIGHT);
      LCD_1IN8_Display(BlackImage);      
      sleep_us(delay_us);
      frames_i = (frames_i+1)%frames_total;
    }
  }
  #endif
  
  /* Module Exit */
  free(BlackImage);
  BlackImage = NULL;
  DEV_Module_Exit();
  return 0;
}
