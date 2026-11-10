/**
 * @file    rotary_states.h
 * @author  Taher Keshavarzian, Sönke Wulf
 * @date    09.11.2026
 * @brief   Gemeinsame Definitionen fuer alle Drehgeber-Module.
 *
 *          Enthaelt die Kodierung der vier Phasen (Rohwert aus GPIOF->IDR),
 *          die Ergebnisse eines Phasenwechsels (STATE_*) und die einheitlichen
 *          Fehlercodes. Alle Module binden nur diese Datei ein und muessen
 *          sich deshalb nicht gegenseitig kennen.
 */

#ifndef ROTARY_STATES_H
#define ROTARY_STATES_H

#include <stdint.h>

/* ---------------------------------------------------------------------------
 * Phasen: Kanal A = PF0 (Bit 0), Kanal B = PF1 (Bit 1)
 * Wert = (B << 1) | A  -> pro Schritt aendert sich genau 1 Bit
 *
 *   Phase | A | B | Wert
 *     a   | 0 | 0 |   0
 *     b   | 1 | 0 |   1
 *     c   | 1 | 1 |   3
 *     d   | 0 | 1 |   2
 * ------------------------------------------------------------------------- */
#define PHASE_A 0x0U // A = 0, B = 0
#define PHASE_B 0x1U // A = 1, B = 0
#define PHASE_C 0x3U // A = 1, B = 1
#define PHASE_D 0x2U // A = 0, B = 1

#define NUM_PHASES 4U // Anzahl der Phasen = Groesse der FSM-Matrix

// Ergebnis eines Phasenwechsels (Ausgabe des Zustandsautomaten)
#define STATE_FORWARD 1   // Vorwaertslauf: Phasenzaehler + 1
#define STATE_BACKWARDS 2 // Rueckwaertslauf: Phasenzaehler - 1
#define STATE_NOCHANGE 3  // gleiche Phase wie im letzten Durchlauf
#define STATE_ERROR 4     // Phase uebersprungen (a<->c oder b<->d)

// Einheitliche Fehlercodes (Konzept: 0 = OK, negativ = Fehler)
#define EOK 0                  // kein Fehler */
#define ERR_PHASE_SKIPPED (-1) // ungueltiger Phasenuebergang erkannt
#define ERR_INVALID_PARAM (-2) // ungueltiger Parameter (z. B. Phase > 3)

/**
 * @brief Eine eingelesene Phase zusammen mit ihrem Zeitstempel.
 *
 *        Phase und Zeitpunkt des Einlesens gehoeren zusammen und werden
 *        deshalb in einer struct gespeichert (siehe Aufgabenstellung).
 */
typedef struct {
  uint32_t value;     // Wert der Phase (PHASE_A ... PHASE_D)
  uint32_t timestamp; // Timerwert beim Einlesen (90 Ticks pro us)
} Phase_t;

#endif /* ROTARY_STATES_H */
// EOF
