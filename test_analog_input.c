#include "hysteresis.h"
#include "init.h"
#include "analog_input.h"
#include "7sddriver.h"

uint32_t num = 0;

int main(void)
{
    init();
    init_analog_input();
    init_7sd_driver();
    start_7sd_driver();

    while(1)
    {
        display_value = num = hysteresis_loop(400, num, get_analog_input() / 0.4095);
        //display_value = get_analog_input();
    }

    return 0;
}
