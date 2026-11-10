/**
 * @file    main.c
 * @author  Taher Keshavarzian, Sönke Wulf
 * @date    09.11.2026
 * @brief   Drehgeber mit Polling: Super-Loop nach Direct Digital Control.
 *
 *          1. Einlesen:   Phase (mit Zeitstempel) und Taste S6
 *          2. Berechnen:  Phasenwechsel, Phasenzaehler, Winkel, Geschwindigkeit
 *          3. Ausgeben:   LEDs und LCD (ein Zeichen pro Durchlauf)
 *
 *          Nach dem Einlesen wird ausschliesslich mit den eingelesenen
 *          Werten gearbeitet.
 */

#include "init.h"
#include "rotary_calculator.h"
#include "rotary_display.h"
#include "rotary_input.h"
#include "rotary_led.h"
#include "rotary_states.h"
#include "timer.h"
#include <stdbool.h>
#include <stdint.h>
#include <stm32f4xx_hal.h>

/*
 * Zeitmessung fuer die Analyse mit dem Oszilloskop: auf 1 setzen.
 * PE0 (LED D16, in dieser Aufgabe frei) ist HIGH waehrend der LCD-Ausgabe.
 */
#define MEASURE_TIMING 0
#define MEASURE_PIN 0U
#define MEASURE_HIGH (0x01U << MEASURE_PIN)
#define MEASURE_LOW (0x01U << (MEASURE_PIN + 16U))

/**
 * @brief Zentrale Fehlerbehandlung (einheitliches Fehlerkonzept).
 *
 * @param errorCode  Fehlercode aus rotary_states.h (negativ)
 * @param errorState Zeiger auf den Fehlerzustand des Programms
 */
static void handle_error(int error_code, bool *error_state) {
  if (error_code != EOK) {
    set_error_led();
    *error_state = true;
  }
}

/**
 * @brief Setzt alle Programmzustaende zurueck (Start und Taste S6).
 */
static void reset_program(Phase_t phase, double *angle, double *angle_per_sec,
                         bool *error_state) {
  rotary_reset(phase); // Zaehler 0, neues Zeitfenster ab jetzt
  *angle = 0.0;
  *angle_per_sec = 0.0;
  *error_state = false;
  clear_error_led();
  clear_direction_leds();
  set_led_counter(0);
}

int main(void) {
  double angle = 0.0;
  double angle_per_sec = 0.0;
  bool error_state = false;

  initITSboard();
  init_display();
  initTimer();

  /* Startphase einlesen, damit der erste Vergleich mit der echten Phase
   * erfolgt (sonst z. B. Start in Phase c -> sofort Fehler a->c) */
  reset_program(read_phase(), &angle, &angle_per_sec, &error_state);

  while (1) {
    // Einlesen
    Phase_t phase = read_phase();
    bool s6_pressed = button_s6_pressed();

    // Berechnen
    if (s6_pressed) {
      // Reset ist jederzeit moeglich und loescht den Fehlerzustand
      reset_program(phase, &angle, &angle_per_sec, &error_state);
    } else if (!error_state) {
      int state = rotary_update(phase);

      if (state == STATE_ERROR) {
        handle_error(ERR_PHASE_SKIPPED, &error_state);
      } else {
        angle = rotary_get_angle();
        double new_speed = 0.0;
        if (rotary_get_speed(phase.timestamp, &new_speed)) {
          angle_per_sec = new_speed;
        }
        set_led_counter(rotary_get_counter());
        set_led_direction(state);
      }
    } else {
      // Fehlerzustand: Werte und LEDs bleiben eingefroren bis S6
    }

    // Ausgeben
    /* Auch im Fehlerfall weiter aufrufen: angle/angle_per_sec sind eingefroren,
     * ein angefangener Ausgabezyklus wird beendet und danach aendert sich
     * kein Zeichen mehr. So bleibt keine halb geschriebene Zahl stehen */
#if MEASURE_TIMING
    GPIOE->BSRR = MEASURE_HIGH;
#endif
    update_display(angle, angle_per_sec);
#if MEASURE_TIMING
    GPIOE->BSRR = MEASURE_LOW;
#endif
  }
}
// EOF
