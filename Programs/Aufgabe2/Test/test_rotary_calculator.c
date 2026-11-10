/**
 * @file    test_rotary_calculator.c
 * @author  Taher Keshavarzian
 * @date    09.11.2026
 * @brief   Host-Test des Moduls rotary_calculator (ohne ITS-Board).
 *
 *          Moeglich, weil rotary_calculator keine Hardware benutzt.
 *          Uebersetzen und ausfuehren auf dem PC:
 *            gcc -std=c11 -Wall -IInc Test/test_rotary_calculator.c \
 *                Src/rotary_calculator.c -o test && ./test
 */

#include "rotary_calculator.h"
#include <math.h>
#include <stdio.h>

static int failures = 0;

#define CHECK(cond, msg)                                                       \
  do {                                                                         \
    if (!(cond)) {                                                             \
      printf("FAIL: %s\n", msg);                                               \
      failures++;                                                              \
    } else {                                                                   \
      printf("ok:   %s\n", msg);                                               \
    }                                                                          \
  } while (0)

#define TICKS_PER_SEC 90000000UL

/* Vorwaerts-Reihenfolge der Phasen: a, b, c, d */
static const uint32_t FWD[4] = {PHASE_A, PHASE_B, PHASE_C, PHASE_D};

static Phase_t ph(uint32_t value, uint32_t ts) {
  Phase_t p = {value, ts};
  return p;
}

/* T1: Alle 16 Uebergaenge gegen Tabelle 1 der Aufgabe pruefen */
static void testTransitions(void) {
  int ok = 1;
  for (int i = 0; i < 4; i++) {
    uint32_t cur = FWD[i];
    if (rotary_determineState(cur, cur) != STATE_NOCHANGE) {
      ok = 0;
    }
    if (rotary_determineState(cur, FWD[(i + 1) % 4]) != STATE_FORWARD) {
      ok = 0;
    }
    if (rotary_determineState(cur, FWD[(i + 3) % 4]) != STATE_BACKWARDS) {
      ok = 0;
    }
    if (rotary_determineState(cur, FWD[(i + 2) % 4]) != STATE_ERROR) {
      ok = 0;
    }
  }
  CHECK(ok, "T1 alle 16 Phasenuebergaenge korrekt");
  CHECK(rotary_determineState(7U, 0U) == STATE_ERROR,
        "T1 ungueltiger Parameter -> STATE_ERROR");
}

/* T2: 1200 Schritte vorwaerts = 360,0 Grad, danach 1 zurueck */
static void testAngle(void) {
  rotary_reset(ph(PHASE_A, 0U));
  for (int i = 1; i <= 1200; i++) {
    (void)rotary_update(ph(FWD[i % 4], (uint32_t)i));
  }
  CHECK(rotary_getCounter() == 1200, "T2 Zaehler nach 1 Umdrehung = 1200");
  CHECK(fabs(rotary_getAngle() - 360.0) < 1e-9, "T2 Winkel = 360,0 Grad");

  rotary_reset(ph(PHASE_A, 0U));
  (void)rotary_update(ph(PHASE_D, 1U));
  CHECK(rotary_getCounter() == -1, "T2 ein Schritt rueckwaerts = -1");
}

/* T3: konstante Geschwindigkeit 90 Grad/s -> ein Schritt alle 1/300 s */
static void testSpeed(uint32_t startTs, const char *name) {
  rotary_reset(ph(PHASE_A, startTs));
  const uint32_t stepTicks = (uint32_t)(TICKS_PER_SEC / 300UL); /* 300 000 */
  double speed = 0.0;
  int computed = 0;
  for (int i = 1; i <= 200; i++) {
    uint32_t ts = startTs + (uint32_t)i * stepTicks; /* darf ueberlaufen */
    (void)rotary_update(ph(FWD[i % 4], ts));
    double s = 0.0;
    if (rotary_getSpeed(ts, &s)) {
      speed = s;
      computed++;
    }
  }
  CHECK(computed > 0 && fabs(speed - 90.0) < 0.05, name);
}

/* T4: Stillstand -> erst nach 3 s ein Wert, und der ist 0 */
static void testTimeout(void) {
  rotary_reset(ph(PHASE_A, 0U));
  double s = -1.0;
  bool early = rotary_getSpeed((uint32_t)(2.9 * TICKS_PER_SEC), &s);
  (void)rotary_update(ph(PHASE_A, (uint32_t)(3.0 * TICKS_PER_SEC)));
  bool late = rotary_getSpeed((uint32_t)(3.0 * TICKS_PER_SEC), &s);
  CHECK(!early && late && s == 0.0,
        "T4 Stillstand: Ausgabe erst nach 3 s, 0,0 Grad/s");
}

/* T5: 0,1 Grad/s -> ein Schritt alle 3 s, Anzeige 0,1 und nicht 0 */
static void testSlowSpeed(void) {
  rotary_reset(ph(PHASE_A, 0U));
  double s = 0.0;
  uint32_t t1 = (uint32_t)(3.0 * TICKS_PER_SEC);
  (void)rotary_update(ph(PHASE_B, t1));
  bool got = rotary_getSpeed(t1, &s);
  CHECK(got && fabs(s - 0.1) < 1e-9, "T5 langsame Drehung 0,1 Grad/s erkannt");
}

int main(void) {
  testTransitions();
  testAngle();
  testSpeed(0U, "T3 90 Grad/s korrekt berechnet");
  testSpeed(0xFFFFFFFFU - 30000000U, "T3 90 Grad/s auch ueber Timer-Ueberlauf");
  testTimeout();
  testSlowSpeed();
  printf("\n%s (%d Fehler)\n",
         failures == 0 ? "ALLE TESTS OK" : "TESTS FEHLGESCHLAGEN", failures);
  return failures == 0 ? 0 : 1;
}
