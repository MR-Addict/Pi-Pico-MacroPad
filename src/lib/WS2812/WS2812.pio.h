// -------------------------------------------------- //
// This file is autogenerated by pioasm; do not edit! //
// -------------------------------------------------- //

#pragma once

#if !PICO_NO_HARDWARE
#include "hardware/pio.h"
#endif

// ------ //
// WS2812 //
// ------ //

#define WS2812_wrap_target 0
#define WS2812_wrap 3

#define WS2812_T1 2
#define WS2812_T2 5
#define WS2812_T3 3

static const uint16_t WS2812_program_instructions[] = {
            //     .wrap_target
    0x6221, //  0: out    x, 1            side 0 [2] 
    0x1123, //  1: jmp    !x, 3           side 1 [1] 
    0x1400, //  2: jmp    0               side 1 [4] 
    0xa442, //  3: nop                    side 0 [4] 
            //     .wrap
};

#if !PICO_NO_HARDWARE
static const struct pio_program WS2812_program = {
    .instructions = WS2812_program_instructions,
    .length = 4,
    .origin = -1,
};

static inline pio_sm_config WS2812_program_get_default_config(uint offset) {
    pio_sm_config c = pio_get_default_sm_config();
    sm_config_set_wrap(&c, offset + WS2812_wrap_target, offset + WS2812_wrap);
    sm_config_set_sideset(&c, 1, false, false);
    return c;
}

#include "hardware/clocks.h"
static inline void WS2812_program_init(PIO pio, uint8_t sm, uint8_t offset, uint8_t pin) {
    pio_gpio_init(pio, pin);
    pio_sm_set_consecutive_pindirs(pio, sm, pin, 1, true);
    pio_sm_config config = WS2812_program_get_default_config(offset);
    sm_config_set_sideset_pins(&config, pin);
    sm_config_set_out_shift(&config, false, true, 24);
    sm_config_set_fifo_join(&config, PIO_FIFO_JOIN_TX);
    int cycles_per_bit = WS2812_T1 + WS2812_T2 + WS2812_T3;
    float div = clock_get_hz(clk_sys) / (800000 * cycles_per_bit);
    sm_config_set_clkdiv(&config, div);
    pio_sm_init(pio, sm, offset, &config);
    pio_sm_set_enabled(pio, sm, true);
}

#endif

