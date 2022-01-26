#ifndef _ENCODER_H_
#define _ENCODER_H_

#include "pico/stdlib.h"

class Button {
   private:
    uint8_t button_pin;
    uint32_t last_time;

   public:
    Button(uint8_t buttonPin);
    bool isPressed();
};

class Encoder {
   private:
    uint8_t pin_A;
    uint8_t pin_B;
    bool init_status;
    uint32_t last_time;

   public:
    Encoder(uint8_t pinA, uint8_t pinB);
    int8_t getDirection();
};

#endif