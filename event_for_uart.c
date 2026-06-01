#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/cm3/nvic.h>

void hendel_event_for_uart();

void init_event_for_uart()
{
    rcc_periph_clock_enable(RCC_TIM4);
    
    nvic_enable_irq(NVIC_TIM4_IRQ);
    nvic_set_priority(NVIC_TIM4_IRQ, 1);
    timer_set_prescaler(TIM4, 8400 - 1);
    timer_set_period(TIM4, 1000 - 1);
    timer_enable_irq(TIM4, TIM_DIER_UIE);
    timer_enable_counter(TIM4);
}

void tim4_isr(void)
{
    if(timer_get_flag(TIM4, TIM_SR_UIF))
    {
        timer_clear_flag(TIM4, TIM_SR_UIF);   
        hendel_event_for_uart();
    }
}

