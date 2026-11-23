/**
 ******************************************************************************
 * @file    lineHandling.h
 * @author  Taher Keshavarzian
 * @date    22.11.2026
 * @brief   Bindeglied zwischen Dekoder und LCD: sammelt die dekodierten Pixel
 *          zeilenweise, verkleinert sie bei Bedarf (Box-Algorithmus) und gibt
 *          fertige Zeilen ueber lcdOutput aus.
 *
 *          Der Dekoder (compressedBMP / uncompressedBMP) liefert die Pixel in
 *          Dateireihenfolge: Zeile 0 ist die UNTERSTE Bildzeile (bottom-up).
 *          Er ruft fuer Pixel lh_put_run() und am Zeilenende lh_next_rows()
 *          auf. Wie das Bild auf das Display kommt, kennt nur dieses Modul.
 *
 *          Box-Algorithmus: Jedes Display-Pixel ist der Mittelwert einer
 *          Kachel (Box) von Bildpixeln. Statt der letzten 5 Bildzeilen werden
 *          pro Display-Spalte nur die Summen von Rot, Gruen und Blau gehalten
 *          (3 x 480 Werte). Ist die Kachelzeile vollstaendig, werden die
 *          Mittelwerte gebildet und als eine LCD-Zeile ausgegeben.
 ******************************************************************************
 */

#ifndef _LINE_HANDLING_H
#define _LINE_HANDLING_H

#include "BMP_types.h"
#include <stdint.h>

/**
 * @brief  Bereitet die Ausgabe eines neuen Bilds vor: berechnet die
 *         Zielgroesse auf dem Display und setzt die Summen zurueck.
 * @param  width   Bildbreite in Pixeln (1..MAX_IMG_WIDTH, bereits geprueft)
 * @param  height  Bildhoehe in Pixeln (1..MAX_IMG_HEIGHT, bereits geprueft)
 * @retval None
 */
extern void lh_begin(int32_t width, int32_t height);

/**
 * @brief  Traegt count gleichfarbige Pixel der aktuellen Bildzeile ab
 *         Spalte x ein. Der Aufrufer garantiert x + count <= width.
 * @param  x      erste Spalte (0 = links)
 * @param  count  Anzahl Pixel
 * @param  rgb    Farbe der Pixel
 * @retval None
 */
extern void lh_put_run(int32_t x, int32_t count, RGBTRIPLE rgb);

/**
 * @brief  Schaltet n Bildzeilen weiter (Zeilenende bzw. Delta im RLE).
 *         Ist dadurch eine Display-Zeile fertig, wird sie ausgegeben.
 * @param  n  Anzahl Zeilen (>= 1)
 * @retval None
 */
extern void lh_next_rows(int32_t n);

/**
 * @brief  Beendet das Bild: gibt die zuletzt begonnene Display-Zeile aus.
 * @param  None
 * @retval None
 */
extern void lh_finish(void);

#endif /* _LINE_HANDLING_H */
// EOF
