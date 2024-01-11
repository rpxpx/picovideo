
#include "LCD_ALL_video.h"

// For overclok funcs. 
#include "pico/stdlib.h" 
#include "hardware/vreg.h"


//uint valid_clocks[] = {100000,130000,133000,160000,190000,220000,260000,290000,0}; // Zero-terminates array.

int main(void){

  if (OVERCLOCK){
    vreg_set_voltage(VREG_VOLTAGE_1_30);
    set_sys_clock_khz(225000,true);
  }

  LCD_1in8_video();
  
  // Other screen video functions here.
  // LCD_1in3_video();
  // etc.
  
  return 0;
}
