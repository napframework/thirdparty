cmake_minimum_required(VERSION 3.5)

set(ARTNET_ROOT ..)
set(ARTNET_SRC ${ARTNET_ROOT}/artnet)

include_directories(${ARTNET_ROOT} ${ARTNET_SRC}/android)


add_library(artnet SHARED 
            ${ARTNET_SRC}/artnet.c 
            ${ARTNET_SRC}/private.h 
            ${ARTNET_SRC}/misc.c 
            ${ARTNET_SRC}/network.c 
            ${ARTNET_SRC}/receive.c
            ${ARTNET_SRC}/transmit.c 
            ${ARTNET_SRC}/tod.c 
            )

target_compile_definitions(artnet PRIVATE HAVE_CONFIG_H=1)