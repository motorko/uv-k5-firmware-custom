#ifdef ENABLE_FLASHLIGHT

#include "driver/gpio.h"
#include "bsp/dp32g030/gpio.h"
#include "flashlight.h"

#ifdef ENABLE_FLASHLIGHT_ONOFF_ONLY

enum FlashlightMode_t gFlashLightState = FLASHLIGHT_OFF;

void FlashlightTimeSlice(void)
{
	if (gFlashLightState == FLASHLIGHT_ON) {
		GPIO_SetBit(&GPIOC->DATA, GPIOC_PIN_FLASHLIGHT);
	} else {
		GPIO_ClearBit(&GPIOC->DATA, GPIOC_PIN_FLASHLIGHT);
	}
}

void ACTION_FlashLight(void)
{
	if (gFlashLightState == FLASHLIGHT_OFF) {
		gFlashLightState = FLASHLIGHT_ON;
		GPIO_SetBit(&GPIOC->DATA, GPIOC_PIN_FLASHLIGHT);
	} else {
		gFlashLightState = FLASHLIGHT_OFF;
		GPIO_ClearBit(&GPIOC->DATA, GPIOC_PIN_FLASHLIGHT);
	}
}

#else

enum FlashlightMode_t gFlashLightState = FLASHLIGHT_OFF;
volatile uint16_t gFlashLightBlinkCounter = 0;

void FlashlightTimeSlice(void)
{
	if (gFlashLightState == FLASHLIGHT_BLINK && (gFlashLightBlinkCounter & 15u) == 0) {
		GPIO_FlipBit(&GPIOC->DATA, GPIOC_PIN_FLASHLIGHT);
		return;
	}

	if (gFlashLightState == FLASHLIGHT_SOS) {
		const uint16_t u = 15;
		static uint8_t c;
		static uint16_t next;

		if (gFlashLightBlinkCounter - next > 7 * u) {
			c = 0;
			next = gFlashLightBlinkCounter + 1;
			return;
		}

		if (gFlashLightBlinkCounter == next) {
			if (c==0) {
				GPIO_ClearBit(&GPIOC->DATA, GPIOC_PIN_FLASHLIGHT);
			} else {
				GPIO_FlipBit(&GPIOC->DATA, GPIOC_PIN_FLASHLIGHT);
			}

			if (c >= 18) {
				next = gFlashLightBlinkCounter + 7 * u;
				c = 0;
			} else if(c==7 || c==9 || c==11) {
				next = gFlashLightBlinkCounter + 3 * u;
			} else {
				next = gFlashLightBlinkCounter + u;
			}
			c++;
		}
	}
}

void ACTION_FlashLight(void)
{
	switch (gFlashLightState) {
		case FLASHLIGHT_OFF:
			gFlashLightState++;
			GPIO_SetBit(&GPIOC->DATA, GPIOC_PIN_FLASHLIGHT);
			break;
		case FLASHLIGHT_ON:
		case FLASHLIGHT_BLINK:
			gFlashLightState++;
			break;
		case FLASHLIGHT_SOS:
		default:
			gFlashLightState = 0;
			GPIO_ClearBit(&GPIOC->DATA, GPIOC_PIN_FLASHLIGHT);
	}
}

#endif

#endif
