#include "frame_codec.h"
#include "hysteresis.h"
#include "init.h"
#include "crc.h"
#include "key.h"
#include "uart_driver.h"
#include "analog_input.h"
#include "7sddriver.h"
#include "event_for_uart.h"

#define MINVALUE 0
#define SCALE 2.4418
#define DELTA 0.375f

//int hysteresis_data = 0;
float data = 0.0f;
int num = 0;
float recv_data[6];
float send_data[6];

void hendel_key_event()
{
    num++;
    if(num >= 6) num = 0;
    display_value = recv_data[num] * 100;
}

void handler_uart(uint8_t data)
{
    static uint8_t buf[29];
    static int cnt = 0;

    if(data == '\0')
    {
        if(cnt == 29)
        {
            frame_decode(buf, 29, (uint8_t*)recv_data, 24);
            display_value = recv_data[num] * 100;
        }
        cnt = 0;
    }
    else
    {
        if(cnt >= 29) cnt = 0;
        buf[cnt++] = data;
    }
}

void hendel_event_for_uart()
{
    //hysteresis_data = hysteresis_loop(50, hysteresis_data, get_analog_input());
    //send_data[num] = hysteresis_data / 40.951;
    data = data * (1.0f - DELTA) + get_analog_input() * DELTA;
    send_data[num] = data / 40.951;
    uint8_t buf[30];
    if(frame_encode((uint8_t*)send_data, 24, buf, 30))
    {
        uart_send_data(buf, 30);
    }
}

int main(void)
{
    init();
    init_crc();
    init_key_event();
    init_uart_driver();
    init_analog_input();
    init_7sd_driver();
    start_7sd_driver();
    init_event_for_uart();

    while(1)
    {
        //send_data[num] = hysteresis_loop(100 * SCALE, send_data[num] * 100, get_analog_input() * SCALE + MINVALUE) / 100.0f;
        //send_data[num] = hysteresis_loop(1, send_data[num], get_analog_input());
    }

    return 0;
}

