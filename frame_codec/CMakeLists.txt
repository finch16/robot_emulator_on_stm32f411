cmake_minimum_required(VERSION 3.15)
project(frame_codec C)

add_library(frame_codec STATIC
    src/frame_codec.c
    src/cobs.c
)



