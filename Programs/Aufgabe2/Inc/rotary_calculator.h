/**
 * @file    rotary_calculator.h
 * @author  Taher Keshavarzian, Sönke Wulf
 * @date    09.11.2026
 * @brief   Encodermodul: Phasenwechsel, Phasenzaehler, Winkel und
 *          Winkelgeschwindigkeit.
 *
 *          Das Modul speichert seinen Zustand in statischen Variablen
 *          (letzte Phase, Phasenzaehler mit Zeitstempel, Zeitfenster der
 *          Geschwindigkeitsberechnung). Es greift NICHT auf Hardware zu und
 *          kann deshalb ohne Board getestet werden.
 */

#ifndef ROTARY_CALCULATOR_H
#define ROTARY_CALCULATOR_H

#include "rotary_states.h"
#include <stdbool.h>
#include <stdint.h>

/**
 * @brief  Bestimmt aus zwei Phasen das Ergebnis des Phasenwechsels (FSM).
 *
 * @param  lastPhase  zuvor eingelesene Phase (Rohwert 0..3)
 * @param  currPhase  aktuell eingelesene Phase (Rohwert 0..3)
 * @return STATE_FORWARD, STATE_BACKWARDS, STATE_NOCHANGE oder STATE_ERROR
 *         (auch bei ungueltigen Parametern > 3)
 */
int rotary_determine_state(uint32_t last_phase, uint32_t curr_phase);

/**
 * @brief  Setzt das Modul zurueck (Programmstart und Taste S6).
 *
 * @param  startPhase  aktuelle Phase mit Zeitstempel als neuer Startpunkt
 */
void rotary_reset(Phase_t start_phase);

/**
 * @brief  Verarbeitet die in diesem Super-Loop-Durchlauf eingelesene Phase.
 *
 *         Erkennt den Phasenwechsel, aktualisiert Phasenzaehler und dessen
 *         Zeitstempel und merkt sich die Phase fuer den naechsten Durchlauf.
 *
 * @param  phase  eingelesene Phase mit Zeitstempel
 * @return STATE_FORWARD, STATE_BACKWARDS, STATE_NOCHANGE oder STATE_ERROR
 */
int rotary_update(Phase_t phase);

/**
 * @brief  Liefert den Phasenzaehler (vorwaerts positiv).
 */
int32_t rotary_get_counter(void);

/**
 * @brief  Liefert den Drehwinkel in Grad (Phasenzaehler * 0,3 Grad).
 */
double rotary_get_angle(void);

/**
 * @brief  Berechnet die Winkelgeschwindigkeit, wenn ein Zeitfenster
 *         abgeschlossen ist.
 *
 *         Ein Fenster endet, wenn mindestens 300 ms vergangen sind UND in
 *         diesem Durchlauf ein Phasenwechsel erkannt wurde, oder spaetestens
 *         nach 3 s. Muss in jedem Durchlauf nach rotary_update() aufgerufen
 *         werden.
 *
 * @param  now    Zeitstempel des aktuellen Durchlaufs
 * @param  speed  Ausgabe: Winkelgeschwindigkeit in Grad/s
 *                (nur gueltig, wenn true zurueckgegeben wird)
 * @return true, wenn ein neuer Wert berechnet wurde, sonst false
 */
bool rotary_get_speed(uint32_t now, double *speed);

#endif /* ROTARY_CALCULATOR_H */
// EOF
