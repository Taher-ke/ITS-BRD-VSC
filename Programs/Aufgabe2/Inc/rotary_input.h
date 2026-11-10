/**
 * @file    rotary_input.h
 * @author  Taher Keshavarzian, Sönke Wulf
 * @date    09.11.2026
 * @brief   Eingabemodul: liest die Kanaele A/B des Drehgebers und die Taste S6.
 *
 *          Kanal A = PF0, Kanal B = PF1, Taste S6 = PF6 (low-aktiv).
 *          Die Pins werden bereits von initITSboard() als Eingaenge
 *          konfiguriert, deshalb braucht dieses Modul keine eigene
 *          Init-Funktion.
 */

#ifndef ROTARY_INPUT_H
#define ROTARY_INPUT_H

#include "rotary_states.h"
#include <stdbool.h>

/**
 * @brief  Liest die aktuelle Phase (PF0, PF1) mit Zeitstempel ein.
 *
 *         Beide Kanaele werden mit EINEM Zugriff auf GPIOF->IDR gelesen
 *         (konsistenter Snapshot). Direkt danach wird der Zeitstempel des
 *         Timers gelesen, damit er moeglichst nah am Einlesen liegt.
 *
 * @return Phase (Rohwert 0..3) und Zeitstempel
 */
Phase_t read_phase(void);

/**
 * @brief  Prueft, ob die Taste S6 gedrueckt ist.
 *
 *         S6 ist low-aktiv: Pin LOW = gedrueckt.
 *
 * @return true, wenn S6 gedrueckt ist, sonst false
 */
bool button_s6_pressed(void);

#endif /* ROTARY_INPUT_H */
// EOF
