#include "Encoder.h"

Button::Button(uint8_t buttonPin) {
    button_pin = buttonPin;
    gpio_init(button_pin);
    gpio_set_dir(button_pin, GPIO_IN);
    gpio_pull_up(button_pin);
}

Encoder::Encoder(uint8_t pinA, uint8_t pinB) {
    pin_A = pinA;
    pin_B = pinB;
    gpio_init(pin_A);
    gpio_init(pin_B);
    gpio_set_dir(pin_A, GPIO_IN);
    gpio_set_dir(pin_B, GPIO_IN);
    gpio_pull_up(pin_A);
    gpio_pull_up(pin_B);
    init_status = gpio_get(pin_A);
}

bool Button::isPressed() {
    if (time_us_32() - last_time >= 2e5 && !gpio_get(button_pin)) {
        last_time = time_us_32();
        return true;
    }
    return false;
}

int8_t Encoder::getDirection() {
    int8_t return_value = 0;
    bool current_status = gpio_get(pin_A);
    if (current_status != init_status && time_us_32() - last_time > 1e5) {
        if (!gpio_get(pin_B))
            return_value = 1;
        else
            return_value = -1;
        last_time = time_us_32();
    }
    return return_value;
}