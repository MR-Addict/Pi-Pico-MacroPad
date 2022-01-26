#include "WS2812.H"
#include "WS2812.pio.H"
#include "hardware/pio.h"
#include "math.h"

RGB CRGB(uint32_t color_rgb) {
    RGB foo;
    foo.r = (color_rgb & 0xff0000) >> 16;
    foo.g = (color_rgb & 0xff00) >> 8;
    foo.b = (color_rgb & 0xff);
    return foo;
}

RGB CRGB(uint8_t r, uint8_t g, uint8_t b) {
    RGB foo = {r, g, b};
    return foo;
}

HSV CHSV(uint8_t h, uint8_t s, uint8_t v) {
    HSV foo = {h, s, v};
    return foo;
}

WS2812::WS2812(RGB* leds, uint8_t pin, uint8_t num) {
    ledPin = pin;
    ledNum = num;
    brightness = 255;
    this->leds = leds;
    sm = pio_claim_unused_sm(pio0, true);
    offset = pio_add_program(pio0, &WS2812_program);
    WS2812_program_init(pio0, sm, offset, ledPin);
    clear();
}

WS2812::~WS2812() {}

float WS2812::max(float a, float b, float c) {
    float maxValue = a >= b ? a : b;
    maxValue = maxValue >= c ? maxValue : c;
    return maxValue;
}

float WS2812::min(float a, float b, float c) {
    float minValue = a <= b ? a : b;
    minValue = minValue <= c ? minValue : c;
    return minValue;
}

RGB WS2812::HSV2RGB(HSV foo) {
    float H = (float)foo.h / 255 * 360;
    float S = (float)foo.s / 255;
    float V = (float)foo.v / 255;
    float C = S * V;
    float X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
    float m = V - C;
    float r, g, b;
    if (H < 60) {
        r = C, g = X, b = 0;
    } else if (H < 120) {
        r = X, g = C, b = 0;
    } else if (H < 180) {
        r = 0, g = C, b = X;
    } else if (H < 240) {
        r = 0, g = X, b = C;
    } else if (H < 300) {
        r = X, g = 0, b = C;
    } else {
        r = C, g = 0, b = X;
    }
    r = (r + m) * 255;
    g = (g + m) * 255;
    b = (b + m) * 255;
    RGB foo2 = {(uint8_t)r, (uint8_t)g, (uint8_t)b};
    return foo2;
}

HSV WS2812::RGB2HSV(RGB foo) {
    float R = (float)foo.r / 255;
    float G = (float)foo.g / 255;
    float B = (float)foo.b / 255;
    float Cmax = max(R, G, B);
    float Cmin = min(R, G, B);
    float C = Cmax - Cmin;
    float H, S, V;
    if (C == 0) {
        H = 0;
    } else if (Cmax == R) {
        H = (G - B) / C / 6 * 255;
    } else if (Cmax == G) {
        H = ((B - R) / C + 2) / 6 * 255;
    } else if (Cmax == B) {
        H = ((R - G) / C + 4) / 6 * 255;
    }
    if (Cmax == 0) {
        S = 0;
    } else {
        S = C / Cmax * 255;
    }
    V = Cmax * 255;
    HSV foo2 = {(uint8_t)H, (uint8_t)S, (uint8_t)V};
    return foo2;
}

void WS2812::setBrightness(uint8_t bright) {
    brightness = bright;
}

void WS2812::setPixel(uint8_t posIndex, RGB foo) {
    if (posIndex < ledNum) {
        leds[posIndex] = foo;
    }
}

void WS2812::setPixel(uint8_t posIndex, HSV foo) {
    if (posIndex < ledNum) {
        leds[posIndex] = HSV2RGB(foo);
    }
}

void WS2812::fillSolid(RGB foo) {
    for (uint8_t i = 0; i < ledNum; i++) {
        setPixel(i, foo);
    }
}

void WS2812::fillSolid(HSV foo) {
    for (uint8_t i = 0; i < ledNum; i++) {
        setPixel(i, foo);
    }
}

uint32_t WS2812::pioPushData(RGB foo) {
    return ((uint32_t)(foo.r) << 8) | ((uint32_t)(foo.g) << 16) |
           (uint32_t)(foo.b);
}

void WS2812::clear() {
    for (uint8_t i = 0; i < ledNum; i++) {
        leds[i] = CRGB(0);
    }
}

void WS2812::show() {
    for (uint8_t i = 0; i < ledNum; i++) {
        HSV foo = RGB2HSV(leds[i]);
        foo.v = brightness;
        uint32_t pushData = pioPushData(HSV2RGB(foo)) << 8u;
        pio_sm_put_blocking(pio0, sm, pushData);
    }
}
