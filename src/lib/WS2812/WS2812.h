#ifndef _WS2812_H_
#define _WS2812_H_

#include "pico/stdlib.h"

struct RGB {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};
struct HSV {
    uint8_t h;
    uint8_t s;
    uint8_t v;
};

RGB CRGB(uint32_t color_rgb);
RGB CRGB(uint8_t r, uint8_t g, uint8_t b);
HSV CHSV(uint8_t h, uint8_t s, uint8_t v);

class WS2812 {
   private:
    RGB* leds;
    uint8_t sm;
    uint8_t offset;
    uint8_t ledPin;
    uint8_t ledNum;
    uint8_t brightness;
    RGB HSV2RGB(HSV foo);
    HSV RGB2HSV(RGB foo);
    uint32_t pioPushData(RGB foo);
    float max(float a, float b, float c);
    float min(float a, float b, float c);

   public:
    WS2812(RGB* leds, uint8_t pin, uint8_t num);
    ~WS2812();
    void setBrightness(uint8_t bright);
    void setPixel(uint8_t posIndex, RGB foo);
    void setPixel(uint8_t posIndex, HSV foo);
    void fillSolid(RGB foo);
    void fillSolid(HSV foo);
    void clear();
    void show();
};

#endif