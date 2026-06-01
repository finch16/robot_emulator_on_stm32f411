#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/crc.h>
#include <stdint.h>
#include <stddef.h>

void init_crc()
{
    rcc_periph_clock_enable(RCC_CRC);

    crc_reset();
}

uint32_t get_crc(uint32_t* block, size_t size)
{
    return crc_calculate_block(block, size);
}

uint32_t frame_crc32(const uint8_t *data, size_t len)
{
    crc_reset();
    size_t i = 0;
    while(i + 4 <= len)
    {
        uint32_t w = ((uint32_t)data[i]   << 24) |
                     ((uint32_t)data[i+1] << 16) |
                     ((uint32_t)data[i+2] <<  8) |
                     ((uint32_t)data[i+3]);
        crc_calculate(w);
        i += 4;
    }
    if(i < len)
    {
        uint32_t w = 0; unsigned sh = 24;
        while (i < len) { w |= (uint32_t)data[i++] << sh; sh -= 8; }
        crc_calculate(w);
    }
    return CRC_DR;
}
