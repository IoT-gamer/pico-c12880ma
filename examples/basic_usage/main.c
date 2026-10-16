#include <stdio.h>
#include "pico/stdlib.h"
#include "c12880ma.h"

int main() {
    stdio_init_all();

    c12880ma_config_t spec_cfg = {
        .clk_pin = 2,
        .st_pin = 3,
        .video_pin = 26, 
        .led_pin = 4,
        .integration_cycles = 85 // Adjust this value to control sensitivity
    };    

    c12880ma_init(&spec_cfg);
    c12880ma_set_led(&spec_cfg, true); // Turn on the super bright white LED

    uint16_t spectral_data[C12880MA_CHANNELS];

    while (true) {
        c12880ma_read(&spec_cfg, spectral_data);

        // Print as CSV for plotting
        for (int i = 0; i < C12880MA_CHANNELS; i++) {
            printf("%d,", spectral_data[i]);
        }
        printf("\n");

        sleep_ms(10);
    }
}