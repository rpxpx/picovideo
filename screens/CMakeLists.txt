
aux_source_directory(. DIR_screens_SRCS)

include_directories(../lib/Config)
include_directories(../lib/GUI)
include_directories(../lib/LCD)
include_directories(../lib/Fonts)
include_directories(../)

add_library(screens ${DIR_screens_SRCS})
target_link_libraries(screens PUBLIC Config GUI Fonts LCD zlib)

