#ifndef APP_FLASHLIGHT_H
#define APP_FLASHLIGHT_H

#ifdef ENABLE_FLASHLIGHT

#include <stdint.h>

#ifdef ENABLE_FLASHLIGHT_ONOFF_ONLY
enum FlashlightMode_t {
    FLASHLIGHT_OFF = 0,
    FLASHLIGHT_ON
};
#else
enum FlashlightMode_t {
    FLASHLIGHT_OFF = 0,
    FLASHLIGHT_ON,
    FLASHLIGHT_BLINK,
    FLASHLIGHT_SOS
};
#endif

extern enum FlashlightMode_t gFlashLightState;
#ifndef ENABLE_FLASHLIGHT_ONOFF_ONLY
extern volatile uint16_t     gFlashLightBlinkCounter;
#endif

void FlashlightTimeSlice(void);
void ACTION_FlashLight(void);

#endif

#endif
