#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/adc.h>
#include <libopencm3/stm32/dma.h>

#include <stdint.h>

volatile uint16_t adc_value;

void init_analog_input()
{
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_ADC1);
    rcc_periph_clock_enable(RCC_DMA2);

    gpio_mode_setup(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO2);

    dma_stream_reset(DMA2, DMA_STREAM0);
    dma_set_peripheral_address(DMA2, DMA_STREAM0, (uint32_t)&ADC_DR(ADC1));
    dma_set_memory_address(DMA2, DMA_STREAM0, (uint32_t)&adc_value);
    dma_set_number_of_data(DMA2, DMA_STREAM0, 1);
    dma_set_peripheral_size(DMA2, DMA_STREAM0, DMA_SxCR_PSIZE_16BIT);
    dma_set_memory_size(DMA2, DMA_STREAM0, DMA_SxCR_MSIZE_16BIT);
    dma_enable_circular_mode(DMA2, DMA_STREAM0);
    dma_set_transfer_mode(DMA2, DMA_STREAM0, DMA_SxCR_DIR_PERIPHERAL_TO_MEM);
    dma_set_priority(DMA2, DMA_STREAM0, DMA_SxCR_PL_HIGH);
    dma_enable_stream(DMA2, DMA_STREAM0);

    adc_power_off(ADC1);
    adc_set_clk_prescale(ADC_CCR_ADCPRE_BY4);
    adc_set_resolution(ADC1, ADC_CR1_RES_12BIT);
    adc_set_right_aligned(ADC1);
    adc_set_continuous_conversion_mode(ADC1);
    adc_disable_scan_mode(ADC1);
    adc_disable_external_trigger_regular(ADC1);
    adc_set_sample_time(ADC1, 2, ADC_SMPR_SMP_84CYC);
    uint8_t channel[] = {2};
    adc_set_regular_sequence(ADC1, 1, channel);
    adc_enable_dma(ADC1);
    adc_power_on(ADC1);
    ADC_CR2(ADC1) |= ADC_CR2_DDS;
    adc_start_conversion_regular(ADC1);
}

uint16_t get_analog_input()
{
    return adc_value;
}
