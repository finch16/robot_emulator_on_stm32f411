#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/spi.h>
#include <libopencm3/cm3/nvic.h>
#include <stdint.h>

const uint8_t digits[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};

extern uint32_t display_value;

uint16_t spi_tx_buffer[4];

void prepare_buffer(uint32_t num)
{
    spi_tx_buffer[0] = ((digits[(num / 1000) % 10] & 0xFF) << 8) | 0x08;
    spi_tx_buffer[1] = ((digits[(num / 100 ) % 10] & 0x7F) << 8) | 0x04;
    spi_tx_buffer[2] = ((digits[(num / 10  ) % 10] & 0xFF) << 8) | 0x02;
    spi_tx_buffer[3] = ((digits[(num / 1   ) % 10] & 0xFF) << 8) | 0x01;
}

void start_send(uint8_t cnt)
{
    SPI_DR(SPI1) = spi_tx_buffer[cnt];
}

void spi1_isr(void)
{
    static uint8_t cnt = 0;

    if (SPI_SR(SPI1) & SPI_SR_RXNE)
    {
        SPI_DR(SPI1);

        gpio_set(GPIOA, GPIO4);
        gpio_clear(GPIOA, GPIO4);
        
        ++cnt; if (cnt >= 4) cnt = 0;
        if(cnt == 0)
        {
            prepare_buffer(display_value);
        }
        start_send(cnt);
    }
}

void init_7sd_driver()
{
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_SPI1);

    gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO4);
    gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO5 | GPIO7);
    gpio_set_af(GPIOA, GPIO_AF5, GPIO5 | GPIO7);

    spi_init_master
    (
        SPI1, SPI_CR1_BAUDRATE_FPCLK_DIV_256, SPI_CR1_CPOL_CLK_TO_0_WHEN_IDLE,
        SPI_CR1_CPHA_CLK_TRANSITION_1, SPI_CR1_DFF_16BIT, SPI_CR1_MSBFIRST
    ); 
    spi_set_unidirectional_mode(SPI1);
    spi_enable_rx_buffer_not_empty_interrupt(SPI1);
    spi_enable(SPI1);
    
    nvic_enable_irq(NVIC_SPI1_IRQ);
    nvic_set_priority(NVIC_SPI1_IRQ, 1);
}

void start_7sd_driver()
{
    prepare_buffer(0);
    start_send(0);
}
