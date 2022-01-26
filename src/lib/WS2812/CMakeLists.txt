add_library(WS2812
    WS2812.h
    WS2812.cpp
)

target_link_libraries(WS2812
    pico_stdlib
    hardware_pio
)
