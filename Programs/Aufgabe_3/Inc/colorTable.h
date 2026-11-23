/**
 ******************************************************************************
 * @file    colorTable.h
 * @author  Taher Keshavarzian
 * @date    22.11.2026
 * @brief   Farbtabelle (Palette) eines 8-Bit-BMP.
 *
 *          ct_read() liest die Farbtabelle, die direkt hinter den beiden
 *          Headern steht, und ueberspringt danach alle Bytes bis zum Beginn
 *          der Pixeldaten (bfOffBits). ct_lookup() uebersetzt einen
 *          Farbindex in eine RGB-Farbe.
 ******************************************************************************
 */

#ifndef _COLOR_TABLE_H
#define _COLOR_TABLE_H

#include "BMP_types.h"

/**
 * @brief  Liest die Farbtabelle (nur bei 8 Bit pro Pixel) und springt zum
 *         Anfang der Pixeldaten.
 *
 *         Anzahl der Eintraege: biClrUsed, bei biClrUsed == 0 alle 256.
 *         Danach werden die Bytes bis bfOffBits ueberlesen.
 * @param  file_hdr_p  gepruefter BITMAPFILEHEADER
 * @param  info_hdr_p  gepruefter BITMAPINFOHEADER
 * @retval EOK, COL_TAB_ERR (Tabelle unvollstaendig / ragt in die Pixeldaten)
 *         oder READ_ERR (Datei endet zu frueh)
 */
extern int ct_read(const BITMAPFILEHEADER *file_hdr_p, const BITMAPINFOHEADER *info_hdr_p);

/**
 * @brief  Uebersetzt einen Farbindex in eine RGB-Farbe.
 * @param  idx    Farbindex aus den Pixeldaten (0..255)
 * @param  rgb_p  Ausgabe: Farbe als RGBTRIPLE
 * @retval EOK oder COL_TAB_ERR, wenn idx nicht in der Tabelle liegt
 */
extern int ct_lookup(int idx, RGBTRIPLE *rgb_p);

#endif /* _COLOR_TABLE_H */
// EOF
