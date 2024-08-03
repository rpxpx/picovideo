
# Fill out empty or badly defined fields in LIB_PATHS

#!/bin/bash

echo "Pathfinder invoked..."

if [[ -e LIB_PATHS ]]; then
    export $(grep LIBPRIMED_PATH LIB_PATHS)
    export $(grep PICO_SDK_PATH LIB_PATHS)
    export $(grep PICO_LCD_PATH LIB_PATHS)
    export $(grep ZLIB_RP2XX_PATH LIB_PATHS)
    LIB_PATHS_EDIT=0
fi

# If undefined or not a DIR, try to find with find() in $HOME

if [[ -z ${LIBPRIMED_PATH+x} || ! -d $LIBPRIMED_PATH ]]; then
    LIB_PATHS_EDIT=1
    echo "LIBPRIMED undefined, attempting to find in $HOME..."    
    export LIBPRIMED_PATH=$(./shortest.sh $(find ~ -type d -name primed 2>/dev/null))      
fi
if [[ -z ${PICO_SDK_PATH+x} || ! -d $PICO_SDK_PATH ]]; then
    LIB_PATHS_EDIT=1
    echo "PICO_SDK_PATH undefined, attempting to find in $HOME..."    
    export PICO_SDK_PATH=$(./shortest.sh $(find ~ -type d -name pico-sdk 2>/dev/null))      
fi
if [[ -z ${PICO_LCD_PATH+x} || ! -d $PICO_LCD_PATH ]]; then
    LIB_PATHS_EDIT=1
    echo "PICO_LCD_PATH undefined, attempting to find in $HOME..."
    export PICO_LCD_PATH=$(./shortest.sh $(find ~ -type d -name Pico_LCD_code 2>/dev/null))
fi 
if [[ -z ${ZLIB_RP2XX_PATH+x} || ! -d $ZLIB_RP2XX_PATH ]]; then
    LIB_PATHS_EDIT=1
    echo "ZLIB_RP2XX_PATH undefined, attempting to find in $HOME..."
    export ZLIB_RP2XX_PATH=$(./shortest.sh $(find ~ -type d -name zlib-rp2xx 2>/dev/null))
fi 

if [[ ! -d $PICO_SDK_PATH || ! -d $PICO_LCD_PATH || ! -d $ZLIB_RP2XX_PATH ]]; then
    echo "error: Library paths undefined"
    exit 1
fi
if [[ ! -d $LIBPRIMED_PATH ]]; then
    echo "* warning: path to libprimed undefined."
    echo "* blokweave compression will fail unless libprimed is installed as system library."
fi

echo "Paths found:"
echo "LIBPRIMED_PATH:  "$LIBPRIMED_PATH
echo "PICO_SDK_PATH:   "$PICO_SDK_PATH
echo "PICO_LCD_PATH:   "$PICO_LCD_PATH
echo "ZLIB_RP2XX_PATH: "$ZLIB_RP2XX_PATH

# Write results to set_vars
if [[ $LIB_PATHS_EDIT == 1 ]]; then    
    echo "LIBPRIMED_PATH=$LIBPRIMED_PATH" >LIB_PATHS
    echo "PICO_SDK_PATH=$PICO_SDK_PATH" >>LIB_PATHS
    echo "PICO_LCD_PATH=$PICO_LCD_PATH" >>LIB_PATHS
    echo "ZLIB_RP2XX_PATH=$ZLIB_RP2XX_PATH" >>LIB_PATHS
fi
