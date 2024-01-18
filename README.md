
# picovideo

Drive short looping gif-like videos with the Pico microcontroller on small LCD displays. Working on Nokia-like 1.8" LCD screens.
Supply video frames as a set of .png images. Compiles frames into a binary that is dropped onto Pico flash. Contains a compression algorithm that finds the optimum weave of images into blocks for compression with zlib.


## INSTALLATION

- Dependencies: python3 gcc make cmake gcc-arm-none-eabi libnewlib-arm-none-eabi libstdc++-arm-none-eabi-newlib

- Libraries for python3: numpy matplotlib

- Install Pico SDK:
Library folder should be named pico-sdk, not e.g. pico-sdk-master
[https://github.com/raspberrypi/pico-sdk](https://github.com/raspberrypi/pico-sdk)

- Install Wavepath Pico LCD libraries:
Library folder should be named Pico_LCD_code
[https://files.waveshare.com/upload/2/28/Pico_code.7z](https://files.waveshare.com/upload/2/28/Pico_code.7z)
For technical info, see: [https://www.waveshare.com/wiki/Pico-LCD-1.8](https://www.waveshare.com/wiki/Pico-LCD-1.8)

- Install zlib for rp2xx:
Library folder should be named zlib-rp2xx
[https://github.com/kripton/zlib/tree/rp2xx](https://github.com/kripton/zlib/tree/rp2xx)

- Define directory constants for those libraries in env_vars: 
`PICO_SDK_PATH=/path/to/pico-sdk
 PICO_LCD_PATH=/path/to/Pico_LCD_code
 ZLIB_RP2XX=/path/to/zlib-rp2xx`

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

---

Maximum binary size is ~4.2MB. Video compilation will fail if binary size would exceed this.
In mode 0, with no compression, 49 160x130 pixel frames can be compiled into video.
Highest possible framerate is 16.36 fps, in compression mode 0, and with overclocking on.
