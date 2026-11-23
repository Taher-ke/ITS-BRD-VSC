/**
 ******************************************************************************
 * @file    compressedBMP.h
 * @author  Taher Keshavarzian
 * @date    22.11.2026
 * @brief   Dekoder fuer 8-Bit-BMP mit RLE-Kompression (BI_RLE8).
 *
 *          Liest die Pixeldaten byteweise (nxt_chr) und reicht die Pixel an
 *          lineHandling weiter. Unterstuetzt Encoded Mode, Absolute Mode und
 *          die Escapes End of line, End of bitmap und Delta.
 ******************************************************************************
 */

#ifndef _COMPRESSED_BMP_H
#define _COMPRESSED_BMP_H

#include <stdint.h>

/**
 * @brief  Dekodiert die RLE8-Pixeldaten des aktuellen Bilds. Der Lesezeiger
 *         steht am Anfang der Pixeldaten (bfOffBits), lh_begin() ist gerufen.
 * @param  width   Bildbreite in Pixeln
 * @param  height  Bildhoehe in Pixeln
 * @retval EOK, FORMAT_ERR (Zeile zu lang, zu viele Zeilen), COL_TAB_ERR
 *         (Farbindex ausserhalb der Tabelle) oder READ_ERR (Datei zu kurz)
 */
extern int decode_rle8(int32_t width, int32_t height);

#endif /* _COMPRESSED_BMP_H */
// EOF
