#ifndef C12880MA_H
#define C12880MA_H

#include "pico/stdlib.h"
#include "hardware/adc.h"

#ifdef __cplusplus
extern "C" {
#endif

// The C12880MA has a resolution of 288 pixels 
#define C12880MA_CHANNELS 288

typedef struct {
    uint clk_pin;    // Clock pin mapping
    uint st_pin;     // START (ST) pin mapping
    uint video_pin;  // Must be a Pico ADC capable pin (26, 27, 28, or 29)
    uint led_pin;    // Pin to control the super bright white LED (use 255 if unused)
} c12880ma_config_t;

// Initialize the GPIOs and the ADC peripheral for the spectrometer
void c12880ma_init(const c12880ma_config_t *config);

// Read the spectrometer data into the provided array
// The data_buffer must have space for at least 288 uint16_t elements
void c12880ma_read(const c12880ma_config_t *config, uint16_t *data_buffer);

// Turn the onboard white LED on (true) or off (false)
void c12880ma_set_led(const c12880ma_config_t *config, bool on);

#ifdef __cplusplus
}
#endif

#endif // C12880MA_H