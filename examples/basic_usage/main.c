#include <stdio.h>
#include "pico/stdlib.h"
#include "c12880ma.h"

int main() {
    stdio_init_all();
    
    while (!stdio_usb_connected()) {
        sleep_ms(100);
    }

    // Configure the pins matching the wiring table
    c12880ma_config_t spec_cfg = {
        .clk_pin = 2,
        .st_pin = 3,
        .video_pin = 26, // Must be GPIO 26, 27, 28, or 29
        .led_pin = 4,    // Set to 255 if unused
        .integration_time = 100 // Target integration time in milliseconds
    };

    // Initialize GPIOs and ADC
    c12880ma_init(&spec_cfg);
    c12880ma_set_led(&spec_cfg, true); 

    uint16_t spectral_data[C12880MA_CHANNELS];

    while (true) {
        c12880ma_read(&spec_cfg, spectral_data);

        for (int i = 0; i < C12880MA_CHANNELS; i++) {
            printf("%d", spectral_data[i]);
            if (i < C12880MA_CHANNELS - 1) printf(",");
        }
        printf("\n");

        sleep_ms(spec_cfg.integration_time);
    }
}