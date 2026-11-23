/**
 ******************************************************************************
 * @file    lcdOutput.h
 * @author  Taher Keshavarzian
 * @date    22.11.2026
 * @brief   Ausgabe fertiger Bildzeilen auf dem LCD.
 *
 *          Einzige Stelle der Anwendung, die Pixel auf das Display schreibt.
 *          Je nach USE_LINE_OUTPUT (bmp_config.h) Pixel fuer Pixel mit
 *          GUI_drawPoint (Teilaufgabe a) oder als ganze Zeile mit
 *          GUI_WriteLine (Teilaufgabe b).
 ******************************************************************************
 */

#ifndef _LCD_OUTPUT_H
#define _LCD_OUTPUT_H

#include "LCD_GUI.h" /* COLOR */
#include <stdint.h>

/**
 * @brief  Wandelt eine 24-Bit-Farbe in das 16-Bit-Format des LCD (RGB565):
 *         Bit 15..11 rot, Bit 10..5 gruen, Bit 4..0 blau.
 * @param  red    Rotanteil 0..255
 * @param  green  Gruenanteil 0..255
 * @param  blue   Blauanteil 0..255
 * @retval Farbe im RGB565-Format
 */
extern COLOR rgb_to_lcd(uint8_t red, uint8_t green, uint8_t blue);

/**
 * @brief  Gibt eine Bildzeile ab Spalte 0 auf dem LCD aus.
 * @param  y     LCD-Zeile 0..DISPLAY_HEIGHT-1 (0 = oben)
 * @param  line  Farbwerte der Zeile (RGB565)
 * @param  len   Anzahl Pixel, 1..DISPLAY_WIDTH
 * @retval None
 */
extern void lcd_write_line(int32_t y, COLOR *line, int32_t len);

#endif /* _LCD_OUTPUT_H */
// EOF
