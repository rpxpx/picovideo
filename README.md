
# picovideo

Drive short looping gif-like video with Pico on LCD display.


## INSTALLATION

- Dependencies: python3 gcc make cmake gcc-arm-none-eabi libnewlib-arm-none-eabi libstdc++-arm-none-eabi-newlib

- Libraries for python3: numpy matplotlib

- Install Pico SDK:
[https://github.com/raspberrypi/pico-sdk](https://github.com/raspberrypi/pico-sdk)

- Install Wavepath Pico LCD libraries:
[https://files.waveshare.com/upload/2/28/Pico_code.7z](https://files.waveshare.com/upload/2/28/Pico_code.7z)
For technical info, see: [https://www.waveshare.com/wiki/Pico-LCD-1.8](https://www.waveshare.com/wiki/Pico-LCD-1.8)

- Install zlib for rp2xx:
[https://github.com/kripton/zlib/tree/rp2xx](https://github.com/kripton/zlib/tree/rp2xx)

- Define directory constants for those libraries in env_vars: 
`PICO_SDK_PATH=/path/to/pico-sdk`
`PICO_LCD_PATH=/path/to/Pico_LCD_code`
`ZLIB_RP2XX=/path/to/zlib-rp2xx`

---

## USAGE

>$ ./makevideo FRAMERATE COMPRESSION-MODE [OVERCLOCK] [images]

Output appears in picovdeo as video.uf2. Copy this to Pico flash.

Compression modes:
0: Raw data. Largest binary size, highest framerate.
1: Single frame compression.
2: Blokweave compression. Highest possible compression. Smallest binary, lowest framerate.

Overclock modes:
0: Standard processor speed: 133 MHz.
1: Overclock processor: 225 MHz.
