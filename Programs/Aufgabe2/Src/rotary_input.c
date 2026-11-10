/**
 * @file    rotary_input.c
 * @author  Taher Keshavarzian, Sönke Wulf
 * @date    09.11.2026
 * @brief   Implementierung des Eingabemoduls (PF0, PF1, PF6).
 */

#include "rotary_input.h"
#include "timer.h"
#include <stm32f4xx_hal.h>

#define ENCODER_MASK 0x03U /**< Bit 0 = Kanal A, Bit 1 = Kanal B */
#define S6_PIN 6U          /**< Taste S6 liegt an PF6 */
#define S6_MASK (0x01U << S6_PIN)

Phase_t read_phase(void) {
  Phase_t phase;
  /* Ein einziger Lesezugriff: A und B stammen aus demselben Zeitpunkt. */
  phase.value = GPIOF->IDR & ENCODER_MASK;
  /* Zeitstempel unmittelbar nach dem Einlesen der Phase. */
  phase.timestamp = getTimeStamp();
  return phase;
}

bool button_s6_pressed(void) {
  /* low-active: Bit = 0 bedeutet gedrueckt */
  return (GPIOF->IDR & S6_MASK) == 0U;
}
// EOF
