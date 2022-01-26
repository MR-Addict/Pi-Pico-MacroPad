#include <stdio.h>

#include "lib/Encoder/Encoder.h"
#include "lib/WS2812/WS2812.h"
#include "pico/stdlib.h"

#include "bsp/board.h"
#include "lib/tinyusb/usb_descriptors.h"
#include "tusb.h"

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"

#define LED_PIN 2
#define LED_LENGTH 8

// Encoder and button
Button button[LED_LENGTH] = {3, 6, 4, 9, 13, 5, 10, 12};
uint8_t buttonkey[LED_LENGTH] = {HID_KEY_S,      HID_KEY_Z,  HID_KEY_C,
                                 HID_KEY_X,      HID_KEY_V,  HID_KEY_HOME,
                                 HID_KEY_DELETE, HID_KEY_END};
// WS2812 setup
uint8_t colorIndex;
RGB leds[LED_LENGTH];
WS2812 LEDStrip(leds, LED_PIN, LED_LENGTH);

// Task setup
void hid_task(void* parameter);
void led_task(void* parameter);

int main() {
    // TinyUsb setup
    board_init();
    tusb_init();

    // WS2812 setup
    LEDStrip.setBrightness(50);

    // ReeRTOS setup
    xTaskCreate(hid_task, "hid_task", 128, NULL, 1, NULL);
    xTaskCreate(led_task, "led_task", 128, NULL, 1, NULL);
    vTaskStartScheduler();

    return 0;
}

void hid_task(void* parameter) {
    while (true) {
        tud_task();
        if (tud_hid_ready()) {
            int8_t keyid = -1;
            for (uint8_t i = 0; i < LED_LENGTH; i++) {
                if (button[i].isPressed()) {
                    keyid = i;
                    break;
                }
            }
            if (keyid != -1) {
                uint8_t keycode[6] = {0};
                if (keyid < 5)
                    keycode[0] = HID_KEY_CONTROL_LEFT;
                keycode[1] = buttonkey[keyid];
                tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, keycode);
            } else
                tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
        }
    }
}

void led_task(void* parameter) {
    while (true) {
        LEDStrip.fillSolid(CHSV(colorIndex++, 255, 255));
        LEDStrip.show();
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

uint16_t tud_hid_get_report_cb(uint8_t instance,
                               uint8_t report_id,
                               hid_report_type_t report_type,
                               uint8_t* buffer,
                               uint16_t reqlen) {
    (void)report_id;
    (void)report_type;
    (void)buffer;
    (void)reqlen;
    return 0;
}

void tud_hid_set_report_cb(uint8_t instance,
                           uint8_t report_id,
                           hid_report_type_t report_type,
                           uint8_t const* buffer,
                           uint16_t bufsize) {
    (void)report_id;
    (void)report_type;
    (void)buffer;
    (void)bufsize;
}
