#include "c12880ma.h"
#include "hardware/timer.h" // Required for precise busy_wait_us delays
#include "hardware/sync.h"

void c12880ma_init(const c12880ma_config_t *config) {
    // Set desired pins to OUTPUT
    gpio_init(config->clk_pin);
    gpio_set_dir(config->clk_pin, GPIO_OUT);
    
    gpio_init(config->st_pin);
    gpio_set_dir(config->st_pin, GPIO_OUT);

    if (config->led_pin != 255) {
        gpio_init(config->led_pin);
        gpio_set_dir(config->led_pin, GPIO_OUT);
        gpio_put(config->led_pin, 0);
    }

    // Initialize default states to signal start
    gpio_put(config->clk_pin, 1); // Set SPEC_CLK High
    gpio_put(config->st_pin, 0);  // Set SPEC_ST Low

    // Initialize ADC for the VIDEO pin
    adc_init();
    adc_gpio_init(config->video_pin);
}

void c12880ma_set_led(const c12880ma_config_t *config, bool on) {
    if (config->led_pin != 255) {
        gpio_put(config->led_pin, on ? 1 : 0);
    }
}

void c12880ma_read(const c12880ma_config_t *config, uint16_t *data_buffer) {
    const uint32_t delay_time = 1;

    // DISABLE INTERRUPTS to prevent BLE from causing noise/frequency shifts
    uint32_t irq_status = save_and_disable_interrupts(); 

    gpio_put(config->clk_pin, 0);
    busy_wait_us(delay_time);
    gpio_put(config->clk_pin, 1);
    busy_wait_us(delay_time);
    gpio_put(config->clk_pin, 0);
    
    gpio_put(config->st_pin, 1);
    busy_wait_us(delay_time);

    for (int i = 0; i < 15; i++) {
        gpio_put(config->clk_pin, 1); busy_wait_us(delay_time);
        gpio_put(config->clk_pin, 0); busy_wait_us(delay_time);
    }

    gpio_put(config->st_pin, 0);

    for (int i = 0; i < 85; i++) {
        gpio_put(config->clk_pin, 1); busy_wait_us(delay_time);
        gpio_put(config->clk_pin, 0); busy_wait_us(delay_time);
    }

    gpio_put(config->clk_pin, 1);
    busy_wait_us(delay_time);
    gpio_put(config->clk_pin, 0);
    busy_wait_us(delay_time);

    adc_select_input(config->video_pin - 26);

    for (int i = 0; i < C12880MA_CHANNELS; i++) {
        data_buffer[i] = adc_read();
        gpio_put(config->clk_pin, 1); busy_wait_us(delay_time);
        gpio_put(config->clk_pin, 0); busy_wait_us(delay_time);
    }

    gpio_put(config->st_pin, 1);

    for (int i = 0; i < 7; i++) {
        gpio_put(config->clk_pin, 1); busy_wait_us(delay_time);
        gpio_put(config->clk_pin, 0); busy_wait_us(delay_time);
    }

    gpio_put(config->clk_pin, 1);
    busy_wait_us(delay_time);

    // RESTORE INTERRUPTS
    restore_interrupts(irq_status); 
}