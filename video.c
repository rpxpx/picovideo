
#include "LCD_video_ALL.h"

/* For overclock functions */
#include "pico/stdlib.h" 
#include "hardware/vreg.h"


/* Theoretically valid clock speeds: */
/* 100000,130000,133000,160000,190000,220000,260000,290000 */
/* In my experiments, 225000 was the highest functioning rate when driving the 1in8 LCD. */


int main(void){

  if (OVERCLOCK){
    vreg_set_voltage(VREG_VOLTAGE_1_30);
    set_sys_clock_khz(225000, true);
  }

  LCD_video_1in8();
  
  /* Other screen video functions here. */
  /* LCD_video_1in3(); */
  /* etc. */
  
  return 0;
}
