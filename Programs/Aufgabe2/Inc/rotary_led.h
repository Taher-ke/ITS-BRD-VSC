/**
 * @file    rotary_led.h
 * @author  Taher Keshavarzian, Sönke Wulf
 * @date    09.11.2026
 * @brief   LED-Ausgabemodul fuer den Drehgeber.
 *
 *          - D8-D15 (PD0-PD7): untere 8 Bit des Phasenzaehlers
 *          - D21 (PE5): Fehler
 *          - D22 (PE6): letzte Richtung rueckwaerts
 *          - D23 (PE7): letzte Richtung vorwaerts
 *
 *          Alle Zugriffe erfolgen ueber BSRR (atomar, kein Read-Modify-Write).
 */

#ifndef ROTARY_LED_H
#define ROTARY_LED_H

#include <stdint.h>

/** @brief Gibt die unteren 8 Bit des Phasenzaehlers auf D8-D15 aus. */
void set_led_counter(int32_t phase_counter);

/**
 * @brief Zeigt die Drehrichtung an (D23 vorwaerts, D22 rueckwaerts).
 *        STATE_NOCHANGE und STATE_ERROR lassen die Anzeige unveraendert,
 *        damit die zuletzt erkannte Richtung sichtbar bleibt.
 */
void set_led_direction(int state);

/** @brief Schaltet D22 und D23 aus (Reset). */
void clear_direction_leds(void);

/** @brief Schaltet die Fehler-LED D21 ein. */
void set_error_led(void);

/** @brief Schaltet die Fehler-LED D21 aus. */
void clear_error_led(void);

#endif /* ROTARY_LED_H */
// EOF
