/**
 * @file    rotary_led.c
 * @author  Taher Keshavarzian, Sönke Wulf
 * @date    09.11.2026
 * @brief   Implementierung der LED-Ausgaben ueber BSRR.
 *
 *          BSRR: Bit n (0..15) setzt Pin n, Bit n+16 setzt Pin n zurueck,
 *          0-Bits lassen den Pin unveraendert.
 */

#include "rotary_led.h"
#include "rotary_states.h"
#include <stm32f4xx_hal.h>

#define BSRR_RESET_OFFSET 16U
#define COUNTER_MASK 0xFFU /**< D8-D15 = PD0-PD7 */

#define LED_ERROR_PIN 5U /**< D21 = PE5 */
#define LED_BWD_PIN 6U   /**< D22 = PE6 */
#define LED_FWD_PIN 7U   /**< D23 = PE7 */

#define SET(pin) (0x01U << (pin))
#define RESET(pin) (0x01U << ((pin) + BSRR_RESET_OFFSET))

void set_led_counter(int32_t phase_counter) {
  // Zweierkomplement: -1 -> 0xFF, -2 -> 0xFE, ...
  uint32_t on = (uint32_t)phase_counter & COUNTER_MASK;
  uint32_t off = (~on) & COUNTER_MASK;
  // Ein Schreibzugriff setzt alle 8 LEDs, PD8-PD15 bleiben unberuehrt
  GPIOD->BSRR = on | (off << BSRR_RESET_OFFSET);
}

void set_led_direction(int state) {
  switch (state) {
  case STATE_FORWARD:
    GPIOE->BSRR = SET(LED_FWD_PIN) | RESET(LED_BWD_PIN);
    break;
  case STATE_BACKWARDS:
    GPIOE->BSRR = SET(LED_BWD_PIN) | RESET(LED_FWD_PIN);
    break;
  default:
    // keine Aenderung: letzte Richtung bleibt sichtbar
    break;
  }
}

void clear_direction_leds(void) {
  GPIOE->BSRR = RESET(LED_FWD_PIN) | RESET(LED_BWD_PIN);
}

void set_error_led(void) { GPIOE->BSRR = SET(LED_ERROR_PIN); }

void clear_error_led(void) { GPIOE->BSRR = RESET(LED_ERROR_PIN); }
// EOF
