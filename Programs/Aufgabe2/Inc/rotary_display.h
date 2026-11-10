/**
 * @file    rotary_display.h
 * @author  Taher Keshavarzian, Sönke Wulf
 * @date    09.11.2026
 * @brief   Anzeigemodul: Winkel und Winkelgeschwindigkeit auf dem LCD.
 *
 *          Die feststehenden Texte werden nur einmal in initDisplay()
 *          ausgegeben. updateDisplay() gibt pro Aufruf hoechstens EIN
 *          Zeichen aus, damit ein Super-Loop-Durchlauf kurz bleibt.
 */

#ifndef ROTARY_DISPLAY_H
#define ROTARY_DISPLAY_H

/** @brief Initialisiert das LCD und gibt die statischen Texte aus. */
void init_display(void);

/**
 * @brief Gibt pro Aufruf hoechstens ein geaendertes Zeichen aus.
 *
 *        Am Anfang eines Ausgabezyklus werden beide Werte in Puffer
 *        formatiert (Snapshot). Danach wird pro Aufruf eine Pufferposition
 *        bearbeitet: 0..8 Winkel, 9..17 Geschwindigkeit. Ein Zeichen wird
 *        nur geschrieben, wenn es sich gegenueber der Anzeige geaendert hat.
 *
 * @param angle        Winkel in Grad
 * @param anglePerSec  Winkelgeschwindigkeit in Grad/s
 */
void update_display(double angle, double angle_per_sec);

#endif /* ROTARY_DISPLAY_H */
// EOF
