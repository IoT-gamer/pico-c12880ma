# Pico C12880MA Spectrometer Library

A lightweight Raspberry Pi Pico C/C++ SDK library for the Hamamatsu C12880MA MEMS micro-spectrometer, specifically designed for use with the [GroupGets C12880MA Breakout Board v2](https://groupgets.com/products/c12880ma-c16767ma-breakout-board).

This library provides a simple API to handle the precise timing sequences required to read the 288-channel spectral data, and is packaged to be easily included in existing CMake projects using `FetchContent`.

## Features
* **FetchContent Ready:** Easily integrate into your Pico CMake projects without manually copying files.
* **Configurable Pins:** Define your own GPIOs for Clock, Start, LED, and Video (ADC) connections.
* **Precise Timing:** Uses the Pico's hardware timers to manage the strict microsecond-level clock pulse requirements.
* **LED Control:** Helper function to toggle the breakout board's onboard super-bright white LED for fluorescence applications.

## Hardware Setup
Standard Wiring Example:

| C12880MA Breakout | Raspberry Pi Pico | Notes |
| :--- | :--- | :--- |
| **3V3** | **3V3(OUT)** (Pin 36) | Power the board using Pico's 3.3V output |
| **5V** | *Leave Unconnected* | Not needed when using 3.3V |
| **GND** | **GND** (Pin 38) | Any Pico ground pin works |
| **GND** | *Leave Unconnected* | Redundant ground |
| **TRG** | *Leave Unconnected* | Trigger pin (unused in default read sequence) |
| **EOS** | *Leave Unconnected* | End of Scan (unused in default read sequence) |
| **CLK** | **GPIO 2** | Clock signal |
| **START** | **GPIO 3** | Start pulse signal |
| **VIDEO** | **GPIO 26 (ADC0)** | Analog signal (Must be a Pico ADC pin: 26, 27, 28, or 29) |
| **LED** | **GPIO 4** | Controls the onboard white LED |

## Installation via CMake FetchContent
You do not need to download this repository manually. Simply add the following to your project's `CMakeLists.txt` file:

```cmake
include(FetchContent)

# Fetch the C12880MA library
FetchContent_Declare(
    c12880ma_lib
    GIT_REPOSITORY https://github.com/IoT-gamer/pico_c12880ma.git
    GIT_TAG        main # Or a specific release tag/commit hash
)
FetchContent_MakeAvailable(c12880ma_lib)

# Link it to your executable
add_executable(my_app main.c)

target_link_libraries(my_app 
    pico_stdlib 
    c12880ma # <--- Add the library here
)
```

## Quick Start Example
Here is a basic `main.c` that initializes the spectrometer, turns on the illumination LED, and continuously reads and prints the spectral data as comma-separated values (CSV).

```c
#include <stdio.h>
#include "pico/stdlib.h"
#include "c12880ma.h"

int main() {
    stdio_init_all();
    
    // Wait for USB connection (optional)
    while (!stdio_usb_connected()) {
        sleep_ms(100);
    }

    // Configure the pins matching the wiring table
    c12880ma_config_t spec_cfg = {
        .clk_pin = 2,
        .st_pin = 3,
        .video_pin = 26, // Must be GPIO 26, 27, 28, or 29
        .led_pin = 4     // Set to 255 if unused
    };

    // Initialize GPIOs and ADC
    c12880ma_init(&spec_cfg);
    
    // Turn on the super bright white LED for illumination
    c12880ma_set_led(&spec_cfg, true); 

    // Buffer to hold the 288 pixel readings
    uint16_t spectral_data[C12880MA_CHANNELS];

    while (true) {
        // Perform the sensor read
        c12880ma_read(&spec_cfg, spectral_data);

        // Print output as CSV for easy plotting (e.g., in Serial Plotter)
        for (int i = 0; i < C12880MA_CHANNELS; i++) {
            printf("%d", spectral_data[i]);
            if (i < C12880MA_CHANNELS - 1) {
                printf(",");
            }
        }
        printf("\n");

        sleep_ms(10);
    }
}
```
## API Reference
* `void c12880ma_init(const c12880ma_config_t *config)`: Sets up GPIO directions, initial pin states, and initializes the Pico ADC.
* `void c12880ma_read(const c12880ma_config_t *config, uint16_t *data_buffer)`: Executes the bit-bang timing sequence and fills the provided array with 12-bit analog readings (0-4095).
* `void c12880ma_set_led(const c12880ma_config_t *config, bool on)`: Toggles the onboard white LED.

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments
The bit-banging sequence and timing logic in this library are based on the original [Arduino example](https://github.com/groupgets/c12880ma) provided by Pure Engineering and GroupGets for the C12880MA Breakout Board.