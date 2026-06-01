cmake_minimum_required(VERSION 3.16)

if(DEFINED ENV{LIBOPENCM3_DIR})
    set(LIBOPENCM3_DIR "$ENV{LIBOPENCM3_DIR}" CACHE PATH "Path to libopencm3 root")
else()
    message(FATAL_ERROR "LIBOPENCM3_DIR environment variable is not set")
endif()

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_LIST_DIR}/arm-none-eabi.cmake"
    CACHE FILEPATH "Toolchain file")

project(blink C ASM)

# --- Генерація .ld ---
set(DEVICE "stm32f411ceu6")
find_package(Python3 REQUIRED COMPONENTS Interpreter)
set(GENERATED_LD "${CMAKE_CURRENT_BINARY_DIR}/generated.ld")
set(LD_TEMPLATE "${LIBOPENCM3_DIR}/ld/linker.ld.S")

# Отримуємо DEFS на етапі configure (один раз)
execute_process(
    COMMAND ${Python3_EXECUTABLE}
            "${LIBOPENCM3_DIR}/scripts/genlink.py"
            "${LIBOPENCM3_DIR}/ld/devices.data"
            "${DEVICE}"
            DEFS
    OUTPUT_VARIABLE GENLINK_DEFS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Прибираємо % який може з'явитись в кінці (zsh prompt)
string(REPLACE "%" "" GENLINK_DEFS "${GENLINK_DEFS}")
# Розбиваємо рядок на список аргументів
separate_arguments(GENLINK_DEFS_LIST UNIX_COMMAND "${GENLINK_DEFS}")

add_custom_command(
    OUTPUT  "${GENERATED_LD}"
    COMMAND arm-none-eabi-cpp
            -P
            ${GENLINK_DEFS_LIST}
            -I "${LIBOPENCM3_DIR}/ld"
            "${LD_TEMPLATE}"
            -o "${GENERATED_LD}"
    DEPENDS "${LD_TEMPLATE}"
            "${LIBOPENCM3_DIR}/ld/devices.data"
    COMMENT "Generating linker script for ${DEVICE}"
)

add_custom_target(generate_ld DEPENDS "${GENERATED_LD}")
# ---------------------

add_executable(blink
    main.c
    init.c
    crc.c
    key.c
    uart_driver.c
    analog_input.c
    7sddriver.c
    event_for_uart.c
    frame_codec/src/frame_codec.c
    frame_codec/src/cobs.c
)
add_dependencies(blink generate_ld)

target_include_directories(blink PRIVATE
    frame_codec/include
    frame_codec/src
)

target_include_directories(blink PRIVATE
    "${LIBOPENCM3_DIR}/include"
)

target_link_directories(blink PRIVATE
    "${LIBOPENCM3_DIR}/lib"
)

target_compile_options(blink PRIVATE
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
    -Os
    -Wall
    -fno-common
    -ffunction-sections
    -fdata-sections
)
target_compile_definitions(blink PRIVATE
    STM32F4
)
target_link_options(blink PRIVATE
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
    -nostartfiles
    -Wl,--gc-sections
    -T${CMAKE_CURRENT_BINARY_DIR}/generated.ld
)
target_link_libraries(blink PRIVATE
    opencm3_stm32f4
    c
    m
    nosys
)

add_custom_command(TARGET blink POST_BUILD
    COMMAND arm-none-eabi-objcopy -O binary $<TARGET_FILE:blink> blink.bin
    COMMAND arm-none-eabi-objcopy -O ihex  $<TARGET_FILE:blink> blink.hex
    COMMENT "Generating blink.bin and blink.hex"
)
