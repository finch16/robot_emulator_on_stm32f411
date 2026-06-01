#include <libopencm3/stm32/rcc.h>

void init()
{
    rcc_clock_setup_pll(&rcc_hse_25mhz_3v3[RCC_CLOCK_3V3_84MHZ]);
}
