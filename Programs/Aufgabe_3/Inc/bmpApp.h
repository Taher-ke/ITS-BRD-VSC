/**
 ******************************************************************************
 * @file    bmpApp.h
 * @author  Taher Keshavarzian
 * @date    22.11.2026
 * @brief   Steuerung der Bildverarbeitung: ein Aufruf = ein Bild.
 ******************************************************************************
 */

#ifndef _BMP_APP_H
#define _BMP_APP_H

/**
 * @brief  Fordert die naechste Datei vom PC an und stellt sie dar:
 *           1. Datei oeffnen, Header lesen und pruefen
 *           2. Bildgroesse pruefen (max. 5 x Displaygroesse)
 *           3. Farbtabelle lesen, zum Anfang der Pixeldaten springen
 *           4. Pixeldaten dekodieren (RLE8 oder unkomprimiert) und ausgeben
 *         Fehler werden ueber die Makros aus errorhandler.h auf dem LCD
 *         gemeldet und als Fehlercode zurueckgegeben.
 * @param  None
 * @retval EOK oder Fehlercode (errorhandler.h)
 */
extern int handle_picture(void);

#endif /* _BMP_APP_H */
// EOF
