cmake_minimum_required(VERSION 3.13)

# Create the static library
add_library(c12880ma STATIC src/c12880ma.c)

# Make the include directory public so consumers get it automatically
target_include_directories(c12880ma PUBLIC include)

# Link the required standard Pico SDK hardware libraries
target_link_libraries(c12880ma PUBLIC 
    pico_stdlib 
    hardware_adc
)