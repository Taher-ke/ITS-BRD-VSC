/**
 * @file    rotary_calculator.c
 * @author  Taher Keshavarzian, Sönke Wulf
 * @date    09.11.2026
 * @brief   Zustandsautomat (FSM), Phasenzaehler, Winkel und
 *          Winkelgeschwindigkeit des Drehgebers.
 */

#include "rotary_calculator.h"
#include <stddef.h>

// 300 Schlitze * 4 Phasen = 1200 Phasenwechsel pro Umdrehung -> 0,3 Grad
#define PHASE_CHANGES_PER_REV 1200.0
#define DEGREE_PER_STEP (360.0 / PHASE_CHANGES_PER_REV)

/* Timer der ITS_BRD_LIB: 90 Ticks pro Mikrosekunde */
#define TICKS_PER_SECOND 90000000.0
#define TICKS_PER_MS 90000U
#define WINDOW_TICKS (300U * TICKS_PER_MS)   // 300 ms Zeitfenster
#define TIMEOUT_TICKS (3000U * TICKS_PER_MS) // 3 s ohne Phasenwechsel

/**
 * @brief Phasenzaehler zusammen mit dem Zeitstempel des letzten
 *        Phasenwechsels, der zu seinem Wert beigetragen hat.
 */
typedef struct {
  int32_t value;
  uint32_t timestamp;
} Counter_t;

// Zustand des Moduls
static uint32_t prev_phase = PHASE_A; // Phase des letzten Durchlaufs
static Counter_t counter = {0, 0U};  // aktueller Phasenzaehler
static Counter_t window = {0, 0U};   // Zaehlerstand am Fensteranfang
static bool changed_now = false;      // Phasenwechsel in diesem Durchlauf

/*
 * Uebergangsmatrix: Zeile = letzte Phase, Spalte = aktuelle Phase.
 * Die Indizes sind die Werte der Phasen. Durch die Designated
 * Initializers ([PHASE_x] = ...) passt jede Zeile garantiert zur Phase,
 * obwohl die Werte nicht in der Reihenfolge a, b, c, d liegen.
 */
static const int TRANSITION[NUM_PHASES][NUM_PHASES] = {
    [PHASE_A] = {[PHASE_A] = STATE_NOCHANGE,
                 [PHASE_B] = STATE_FORWARD,
                 [PHASE_C] = STATE_ERROR,
                 [PHASE_D] = STATE_BACKWARDS},
    [PHASE_B] = {[PHASE_A] = STATE_BACKWARDS,
                 [PHASE_B] = STATE_NOCHANGE,
                 [PHASE_C] = STATE_FORWARD,
                 [PHASE_D] = STATE_ERROR},
    [PHASE_C] = {[PHASE_A] = STATE_ERROR,
                 [PHASE_B] = STATE_BACKWARDS,
                 [PHASE_C] = STATE_NOCHANGE,
                 [PHASE_D] = STATE_FORWARD},
    [PHASE_D] = {[PHASE_A] = STATE_FORWARD,
                 [PHASE_B] = STATE_ERROR,
                 [PHASE_C] = STATE_BACKWARDS,
                 [PHASE_D] = STATE_NOCHANGE},
};

int rotary_determine_state(uint32_t last_phase, uint32_t curr_phase) {
  if ((last_phase >= NUM_PHASES) || (curr_phase >= NUM_PHASES)) {
    return STATE_ERROR; // Schutz vor Zugriff ausserhalb der Matrix
  }
  return TRANSITION[last_phase][curr_phase];
}

void rotary_reset(Phase_t start_phase) {
  prev_phase = start_phase.value;
  counter.value = 0;
  counter.timestamp = start_phase.timestamp;
  window = counter;
  changed_now = false;
}

int rotary_update(Phase_t phase) {
  int state = rotary_determine_state(prev_phase, phase.value);
  prev_phase = phase.value;
  changed_now = false;

  switch (state) {
  case STATE_FORWARD:
    counter.value++;
    counter.timestamp = phase.timestamp;
    changed_now = true;
    break;
  case STATE_BACKWARDS:
    counter.value--;
    counter.timestamp = phase.timestamp;
    changed_now = true;
    break;
  default:
    // STATE_NOCHANGE und STATE_ERROR veraendern den Zaehler nicht
    break;
  }
  return state;
}

int32_t rotary_get_counter(void) { return counter.value; }

double rotary_get_angle(void) { return counter.value * DEGREE_PER_STEP; }

bool rotary_get_speed(uint32_t now, double *speed) {
  if (speed == NULL) {
    return false;
  }

  /* uint32_t-Differenz ist auch nach einem Timer-Ueberlauf korrekt, solange
   * weniger als ca. 47 s dazwischen liegen. Das Fenster ist maximal 3 s lang.
   */
  uint32_t elapsed = now - window.timestamp;
  bool window_done = changed_now && (elapsed >= WINDOW_TICKS);
  bool timeout = elapsed >= TIMEOUT_TICKS;

  if (!window_done && !timeout) {
    return false;
  }

  /* Normalfall: Fensterende = Zeitstempel des gerade erkannten
   * Phasenwechsels (= counter.timestamp). Timeout: Fensterende = jetzt. */
  Counter_t end = counter;
  if (!window_done) {
    end.timestamp = now;
  }

  double seconds =
      (double)(end.timestamp - window.timestamp) / TICKS_PER_SECOND;
  *speed = (double)(end.value - window.value) * DEGREE_PER_STEP / seconds;

  window = end; // neues Fenster beginnt am Ende des alten
  return true;
}
// EOF
